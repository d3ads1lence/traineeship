.PHONY: all clean libs

all: hello

clean:
	rm -f *.o *.so *.a binary

libs: libhello.a libgoodbye.a

libhello.a: libhello.o
	ar cr libhello.a libhello.o

libgoodbye.a: libgoodbye.o
	ar cr libgoodbye.a libgoodbye.o

libhello.o: libhello.c
	gcc -c libhello.c

libgoodbye.o: libgoodbye.c
	gcc -c libgoodbye.c
	
hello.o: hello.c
	gcc -c hello.c

hello: hello.o libhello.so libgoodbye.a
	gcc -o hello hello.o -L. -lhello -L. -lgoodbye 
	
