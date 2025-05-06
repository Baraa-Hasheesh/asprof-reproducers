.PHONY: all clean 

all: 
	gcc -g -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux -shared -fPIC thread_Main.cpp -o libthread_Main.so -ldl -lpthread
	gcc -g main.cpp -o main.o -ldl -lpthread

clean:
	rm libthread_Main.so main.o