.PHONY: all clean 

all: 
	gcc -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux DoubleProfiler.c -shared -o libDoubleProfiler.so

clean:
	rm -f libDoubleProfiler.so