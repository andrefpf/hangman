#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <assert.h>

#include <client.h>
#include <hangman.h>

int startclient() {
    int ret, fd;
	struct sockaddr_in6 addr;
	
	fd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
	inet_pton(AF_INET6, SERVER_IP, &addr.sin6_addr);
	addr.sin6_port = htons(SERVER_PORT);
	addr.sin6_family = AF_INET6;

	if ((ret = connect(fd, (struct sockaddr *) &addr, sizeof(addr))) < 0) {
		perror("unable to connect");
		exit(1);
	}

	return fd;
}

int main() {
    int fd = startclient();
    printf("%d \n", fd);

    if (fd < 0) {
        printf("Error while connecting to server \n");
        exit(0);
    }

    char message[] = "Hell World!";

    send(fd, message, sizeof(message), 0);

    return 0;
}