.PHONY: all clean 

all: 
	g++ -I../ main.cpp -o main.o -ldl -lpthread -g
	gcc -fPIC lib.c -shared -o my_lib_1.so
	gcc -fPIC lib.c -shared -o my_lib.so

clean:
	rm -f my_lib_1.so my_lib.so main.o