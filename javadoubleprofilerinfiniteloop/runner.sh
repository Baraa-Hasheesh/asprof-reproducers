#!/bin/bash

if ! [[ $1 == "single" ]] && ! [[ $1 == "multi" ]]; then
    echo "Usage: $0 [single|multi]"
    exit 1
fi

java -Djava.library.path="$(pwd)" -cp async-profiler.jar DoubleProfiler.java DoubleProfiler &

JAVA_PID=$!
echo "Java PID: ${JAVA_PID}"

if [ $1 == "multi" ]; then
    sleep 10
    ASPROF=$(realpath "${PATH_TO_ASYNC_PROFILER_LIB_DIR}/../bin/asprof")
    $ASPROF start -e cpu --nativemem 10 -f output_attach.jfr ${JAVA_PID}
    sleep 1
    $ASPROF stop ${JAVA_PID}
fi

wait $JAVA_PID