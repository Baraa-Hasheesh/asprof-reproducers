#!/bin/sh

java \
  -agentpath:"${PATH_TO_ASYNC_PROFILER_LIB_DIR}"/libasyncProfiler.so=start,event=cpu,file=output_java.jfr \
  -Djava.library.path="$(pwd)" Main.java thread.Main
