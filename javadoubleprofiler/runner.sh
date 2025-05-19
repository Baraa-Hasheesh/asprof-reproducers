#!/bin/sh

if ! [[ $1 == "single" ]] && ! [[ $1 == "multi" ]]; then
    echo "Usage: $0 [single|multi]"
    exit 1
fi

java -agentpath:$(pwd)/libLocal.so=start,nativemem=1,total,file=output.jfr -Djava.library.path="$(pwd)" DoubleProfiler.java DoubleProfiler/run &

JAVA_PID=$!

if [ $1 == "multi" ]; then
    sleep 1
    ASPROF=$(realpath "${PATH_TO_ASYNC_PROFILER_LIB_DIR}/../bin/asprof")

    $ASPROF start -e cpu -f output_attach.jfr ${JAVA_PID}
    sleep 1
    $ASPROF stop ${JAVA_PID}
fi

wait $JAVA_PID