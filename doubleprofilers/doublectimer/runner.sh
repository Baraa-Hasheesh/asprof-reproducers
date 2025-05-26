#!/bin/bash

if ! [[ $1 == "single" ]] && ! [[ $1 == "multi" ]]; then
    echo "Usage: $0 [single|multi]"
    exit 1
fi

java -agentpath:$(pwd)/libLocal.so=start,event=ctimer,signal=40,file=output.jfr -Djava.library.path="$(pwd)" DoubleProfiler.java DoubleProfiler &

JAVA_PID=$!

if [ $1 == "multi" ]; then
    sleep 1
    ASPROF=$(realpath "${PATH_TO_ASYNC_PROFILER_LIB_DIR}/../bin/asprof")
    $ASPROF start -e ctimer --signal 41 -f output_attach.jfr ${JAVA_PID}
fi

wait $JAVA_PID