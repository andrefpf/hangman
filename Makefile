all: server client

server:
	gcc -I include/ src/server.c -o server.o

client:
	gcc -I include/ src/client.c -o client.o

clean:
	rm server.o -f
	rm client.o -f