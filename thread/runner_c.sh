#!/bin/sh

LD_PRELOAD=${PATH_TO_ASYNC_PROFILER_LIB_DIR}/libasyncProfiler.so ASPROF_COMMAND=start,event=cpu,file=output_c.jfr ./main.o
