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

#include <server.h>


int startserver(void) {
	int sd = -1, on = 1;
	struct sockaddr_in serveraddr;

	sd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on,sizeof(on));

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port   = htons(PORT);
	serveraddr.sin_addr.s_addr   = htonl(INADDR_ANY);

	bind(sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

	listen(sd, POLL_LENGHT);

	return sd;
}

void add_connection(struct pollfd *fds, int newsock, int *nfds) {
	for (int i = 0; i < POLL_LENGHT; i++) {
		if (fds[i].fd != 0) {
			continue;
		}
		fds[i].fd = newsock;
		fds[i].events = POLLIN;
		(*nfds)++;
		break;
	}
}

void remove_connection(struct pollfd *fds, int i, int *nfds) {
	fds[i].fd = 0;
	(*nfds)--;
}

void handle_polling(struct pollfd *fds, int i, int server_fd, int *nfds) {
	struct sockaddr_in addr;
    char buffer[500];
    memset(&buffer, 0, 500);
	int socklen, newsock, bytes, ret;
	char op, str[INET_ADDRSTRLEN];

	/* new connection */
	if (fds[i].fd == server_fd) {
		socklen = sizeof(addr);
		newsock = accept(server_fd, NULL, NULL);
		getpeername(newsock, (struct sockaddr *)&addr, &socklen);
		inet_ntop(AF_INET, &addr.sin_addr, str, sizeof(str));
		if (*nfds == MAX_PLAYERS) {
			printf("new connection rejected %s %d\n", str, ntohs(addr.sin_port));
			close(newsock);
		} else {
			printf("new connection! %s %d\n", str, ntohs(addr.sin_port));
			add_connection(fds, newsock, nfds);
		}

		fflush(stdout);

		return;
	}

	/* operation request */
	int p = recv(fds[i].fd, &buffer, 500, 0);
	if (p > 0) {
        printf("%s", buffer);
        memset(&buffer, 0, 500);
        fflush(stdout);
	} else {
		close(fds[i].fd);
		remove_connection(fds, i, nfds);
		printf("client disconnected\n");
	}
}

int main() {
	int server_fd;
	int nfds;
	struct pollfd fds[POLL_LENGHT] = {{.fd = 0, .events = POLLIN}};

	server_fd = startserver();
	fds[0].fd = server_fd;
	nfds = 1;
	printf("listening\n");

	while (1) {
		poll(fds, nfds, -1);

		for (int i = 0; i < nfds; i++) {
			switch (fds[i].revents) {
                case 0:
                    break;
                case POLLIN:
                    handle_polling(fds, i, server_fd, &nfds);
                    break;
                case POLLNVAL:
                case POLLPRI:
                case POLLOUT:
                case POLLERR:
                case POLLHUP:
                default:
                    printf("Unespected revents\n");
                    close(fds[i].fd);
					remove_connection(fds, i, &nfds);
            }
		}
	}

	exit(0);
}