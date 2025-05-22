#!/bin/sh

LD_PRELOAD=libasyncProfiler2.so ASPROF_COMMAND=start,event=cpu,file=output4.jfr LD_LIBRARY_PATH=$(pwd) ./main.o
