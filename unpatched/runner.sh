#!/bin/sh

LD_LIBRARY_PATH=$(pwd) gdb --ex "set pagination off" --ex "handle SIGSEGV pass nostop noprint" \
  --ex "set print thread-events off" --args java \
  -agentpath:${PATH_TO_ASYNC_PROFILER_LIB_DIR}/libasyncProfiler.so=start,nativemem=1,total,cstack=dwarf,file=output.jfr \
  -Djava.library.path="$(pwd)" Main.java unpatched.Main
