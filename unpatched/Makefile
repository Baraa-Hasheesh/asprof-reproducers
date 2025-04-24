.PHONY: all clean

all:
	g++ -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux unpatched_Main.cpp -shared -o libUnpatched_Main.so
	gcc -fPIC lib.c -shared -o libMy_lib.so
	gcc -fPIC lib_dlopen.c -shared -o libMy_dlopen_lib.so

clean:
	rm -f *.so