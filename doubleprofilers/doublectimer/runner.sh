#!/bin/bash

if ! [[ $1 == "single" ]] && ! [[ $1 == "multi" ]]; then
    echo "Usage: $0 [single|multi]"
    exit 1
fi

MODE=$1

java -agentpath:$(pwd)/libLocal.so=start,event=ctimer,signal=40,file=output_${MODE}.jfr -Djava.library.path="$(pwd)" DoubleProfiler.java DoubleProfiler &

JAVA_PID=$!

if [ $MODE == "multi" ]; then
    sleep 1
    ASPROF=$(realpath "${PATH_TO_ASYNC_PROFILER_LIB_DIR}/../bin/asprof")
    $ASPROF start -e ctimer --signal 41 -f output_attach.jfr ${JAVA_PID}
fi

wait $JAVA_PID

JFRCONV=$(realpath "${PATH_TO_ASYNC_PROFILER_LIB_DIR}/../bin/jfrconv")
$JFRCONV output_${MODE}.jfr output_${MODE}.html

if [ $MODE == "multi" ]; then
    $JFRCONV output_attach.jfr output_attach.html
fi