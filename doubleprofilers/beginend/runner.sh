#!/bin/bash

usage() {
    cat <<EOF
Usage: runner.sh [options]
Options:
  --api-profiler            Add an API profiler (default disabled)
  --attach-profiler         Add an asprof profiler (default disabled)
EOF
    exit 1
}

compile() {
    g++ -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux -shared -fPIC -o libBeginEnd.so begin_end_lib.cpp -ldl
}

main() {
    API_PROFILER="false"
    ATTACH_PROFILER="false"

    while [[ $# -gt 0 ]]; do
        KEY="$1"
        case "$KEY" in
            --attach-profiler)
            ATTACH_PROFILER="true"
            shift
            ;;
            --api-profiler)
            API_PROFILER="true"
            shift
            ;;
            *)
            echo "Unknown Option: $1"
            usage
            ;;
        esac
    done

    COMMAND=(
        "env"
        "API_PROFILER=${API_PROFILER}"
        "java"
        "-cp"
        "async-profiler.jar"
        "-Djava.library.path=$(pwd)"
        "BeginEnd.java"
        "BeginEnd"
    )

    "${COMMAND[@]}" &

    JAVA_PID=$!

    if  [[ ${ATTACH_PROFILER} == "true" ]]; then
        asprof start --alloc 0 -f output_attach.jfr --begin Java_beginend_BeginEnd_start1 --end Java_beginend_BeginEnd_stop ${JAVA_PID}
    fi

    #gdb --ex "set pagination off" --ex "handle SIGSEGV pass nostop noprint" --ex "set print thread-events off" --pid $JAVA_PID
    wait $JAVA_PID

    JFRCONV=$(realpath "${PATH_TO_ASYNC_PROFILER_LIB_DIR}/../bin/jfrconv")

    if [ -f "output_agent.jfr" ]; then
        $JFRCONV --alloc "output_agent.jfr" "output_agent.jfr.html"
    fi

    if [ -f "output_attach.jfr" ]; then
        $JFRCONV --alloc "output_attach.jfr" "output_attach.jfr.html"
    fi
}

compile
main "$@"