#!/bin/bash

usage() {
    cat <<EOF
Usage: runner.sh [options]
Options:
  --java-profiler <api|agent|none>    Specify Java profiler
  --attach-profiler                   Add an asprof profiler (default disabled)
  --lock-mode <sync|sema>             Specify lock mode to be synchronization or Semaphore
EOF
    exit 1
}

main() {
    JAVA_PROFILER=""
    ATTACH_PROFILER="false"
    LOCK_MODE=""

    while [[ $# -gt 0 ]]; do
        KEY="$1"
        case "$KEY" in
            --java-profiler)
            shift
            JAVA_PROFILER="$1"
            shift
            ;;
            --attach-profiler)
            ATTACH_PROFILER="true"
            shift
            ;;
            --lock-mode)
            shift
            LOCK_MODE+="$1"
            shift
            ;;
            *)
            echo "Unknown Option: $1"
            usage
            ;;
        esac
    done

    if ! [[ $JAVA_PROFILER == "api" ]] && ! [[ $JAVA_PROFILER == "agent" ]] && ! [[ $JAVA_PROFILER == "none" ]] ; then
        echo "Unknown --java-profiler: $JAVA_PROFILER"
        usage
    fi

    if ! [[ $LOCK_MODE == "sync" ]] && ! [[ $LOCK_MODE == "sema" ]] ; then
        echo "Unknown --lock-mode: $LOCK_MODE"
        usage
    fi

    JAVA_PROFILER_FILE_NAME="output_${JAVA_PROFILER}_${LOCK_MODE}_${ATTACH_PROFILER}.jfr"
    ATTACH_PROFILER_FILE_NAME="output_attach_${JAVA_PROFILER}_${LOCK_MODE}.jfr"

    COMMAND=(
        "env"
        "JAVA_PROFILER_FILE_NAME=$JAVA_PROFILER_FILE_NAME"
        "JAVA_PROFILER=$JAVA_PROFILER"
        "LOCK_MODE=$LOCK_MODE"
        "ATTACH_PROFILER=$ATTACH_PROFILER"
        "java"
        "-cp"
        "async-profiler.jar"
        "-Djava.library.path=$(pwd)"

    )

    if [[ $JAVA_PROFILER == "agent" ]]; then
        COMMAND+=("-agentpath:$(pwd)/libLocal.so=start,lock=0,file=${JAVA_PROFILER_FILE_NAME}")
    fi

    COMMAND+=("DoubleProfiler.java" "DoubleProfiler")

    "${COMMAND[@]}" &

    JAVA_PID=$!

    ASPROF=$(realpath "${PATH_TO_ASYNC_PROFILER_LIB_DIR}/../bin/asprof")

    if  [[ $ATTACH_PROFILER == "true" ]]; then
        sleep 1
        $ASPROF start --lock 0 -f ${ATTACH_PROFILER_FILE_NAME} ${JAVA_PID}
    fi

    #gdb --ex "set pagination off" --ex "handle SIGSEGV pass nostop noprint" --ex "set print thread-events off" --pid $JAVA_PID
    wait $JAVA_PID

    JFRCONV=$(realpath "${PATH_TO_ASYNC_PROFILER_LIB_DIR}/../bin/jfrconv")

    if [ -f "$JAVA_PROFILER_FILE_NAME" ]; then
        $JFRCONV --lock $JAVA_PROFILER_FILE_NAME "${JAVA_PROFILER_FILE_NAME}.html"
    fi

    if [ -f "$ATTACH_PROFILER_FILE_NAME" ]; then
        $JFRCONV --lock $ATTACH_PROFILER_FILE_NAME "${ATTACH_PROFILER_FILE_NAME}.html"
    fi
}

main "$@"