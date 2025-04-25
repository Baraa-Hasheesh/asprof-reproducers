#!/bin/sh

LD_PRELOAD=./my_lib.so LD_LIBRARY_PATH=${PATH_TO_ASYNC_PROFILER_LIB_DIR} ./main.o $*