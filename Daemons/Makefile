.PHONY: all clean 

all: server client

clean:
	rm -f *.o *.txt server client

server.o: server.c
	gcc -c server.c
	
server: server.o
	gcc -o server server.o -lrt

client.o: client.c
	gcc -c client.c
	
client: client.o
	gcc -o client client.o -lrt
