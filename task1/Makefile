.PHONY: all clean libs

TARGET = hello
LIB_HELLO = libhello
LIB_BYE = libgoodbye

all: hello

clean:
	rm -f *.o *.so *.a $(TARGET)

libs: $(LIB_HELLO) $(LIB_BYE)

$(LIB_HELLO): 	$(LIB_HELLO).c world.h
				gcc -c $(LIB_HELLO).c -o $(LIB_HELLO).o
				ar cr $(LIB_HELLO).a $(LIB_HELLO).o

$(LIB_BYE): 	$(LIB_BYE).c world.h
				gcc -c $(LIB_BYE).c -o $(LIB_BYE).o
				ar cr $(LIB_BYE).a $(LIB_BYE).o

$(TARGET): 		$(TARGET).c $(LIB_HELLO) $(LIB_BYE)
				gcc -c $(TARGET).c
				gcc -o $(TARGET) $(TARGET).o -L. -lhello -L. -lgoodbye 
