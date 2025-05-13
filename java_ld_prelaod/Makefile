.PHONY: all clean

all:
	g++ -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux preload.cpp -I.. -o main.o -ldl -g
	javac Preload.java

clean:
	rm -f *.o
	rm -f Preload.class
