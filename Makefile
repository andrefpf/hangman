all: server

server:
	gcc -I include/ src/server.c -o server

clean:
	rm server