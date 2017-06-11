.PHONY: all clean 

all: main

clean:
	rm -f *.o *.so *.a binary
	
main.o: main.c
	gcc -c main.c

main: main.o
	gcc -o main main.o -lpthread
	
