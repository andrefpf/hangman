all: server client

server:
	gcc -I include/ src/server.c -o server

client:
	gcc -I include/ src/client.c -o client

clean:
	rm server -f
	rm client -f