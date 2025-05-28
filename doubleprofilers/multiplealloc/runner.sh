#!/bin/bash

usage() {
    cat <<EOF
Usage: runner.sh [options]
Options:
  --api-profiler [wait-to-start] [run-duration]                           Add an API profiler (default disabled)
  --agent-profiler                                                        Add an Agent profiler (default disabled)
  --attach-profiler [wait-to-start] [run-duration]                        Add an asprof profiler (default disabled)
EOF
    exit 1
}

main() {
    AGENT_PROFILER="false"
    ATTACH_PROFILER=()
    API_PROFILER=()

    while [[ $# -gt 0 ]]; do
        KEY="$1"
        case "$KEY" in
            --api-profiler)
            shift
            API_PROFILER+=("$1")
            shift
            API_PROFILER+=("$1")
            shift
            ;;
            --attach-profiler)
            shift
            ATTACH_PROFILER+=("$1")
            shift
            ATTACH_PROFILER+=("$1")
            shift
            ;;
            --agent-profiler)
            AGENT_PROFILER="true"
            shift
            ;;
            *)
            echo "Unknown Option: $1"
            usage
            ;;
        esac
    done

    PARAMS=("${API_PROFILER[@]}" "${ATTACH_PROFILER[@]}")

    for i in "${PARAMS[@]}"; do
        if ! [[ $i  =~ ^[1-9][0-9]* ]]; then
            echo "--attach-profiler/--api-profiler Invalid argument $i"
            usage
        fi
    done

    if ! [[ ${#API_PROFILER[@]} == 2 || ${#API_PROFILER[@]} == 0 ]]; then
        echo "--api-profiler Invalid number of argument"
        usage
    fi

    if ! [[ ${#ATTACH_PROFILER[@]} == 2 || ${#ATTACH_PROFILER[@]} == 0 ]]; then
        echo "--attach-profiler Invalid number of argument"
        usage
    fi

    COMMAND=(
        "env"
        "API_DELAY_START=${API_PROFILER[0]}"
        "API_RUN_DURATION=${API_PROFILER[1]}"
        "PWD=$(pwd)"
        "java"
        "-cp"
        "async-profiler.jar"
        "-Djava.library.path=$(pwd)"
    )

    if [[ $AGENT_PROFILER == "true" ]]; then
        COMMAND+=("-agentpath:$(pwd)/libLocal.so=start,alloc=0,file=output_agent.jfr")
    fi

    COMMAND+=("MultipleProfilers.java" "MultipleProfilers")

    "${COMMAND[@]}" &

    JAVA_PID=$!

    ASPROF=$(realpath "${PATH_TO_ASYNC_PROFILER_LIB_DIR}/../bin/asprof")

    if  [[ ${#ATTACH_PROFILER[@]} == 2 ]]; then
        sleep "${ATTACH_PROFILER[0]}"
        $ASPROF start --alloc 0 -f output_attach.jfr ${JAVA_PID}
        sleep "${ATTACH_PROFILER[1]}"
        $ASPROF stop ${JAVA_PID}
    fi

    #gdb --ex "set pagination off" --ex "handle SIGSEGV pass nostop noprint" --ex "set print thread-events off" --pid $JAVA_PID
    wait $JAVA_PID

    JFRCONV=$(realpath "${PATH_TO_ASYNC_PROFILER_LIB_DIR}/../bin/jfrconv")

    if [ -f "output_agent.jfr" ]; then
        $JFRCONV --alloc "output_agent.jfr" "output_agent.jfr.html"
    fi

    if [ -f "output_api.jfr" ]; then
        $JFRCONV --alloc "output_api.jfr" "output_api.jfr.html"
    fi

    if [ -f "output_attach.jfr" ]; then
        $JFRCONV --alloc "output_attach.jfr" "output_attach.jfr.html"
    fi
}

main "$@"