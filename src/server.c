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
#include <hangman.h>

struct Hangman * hangman[MAX_PLAYERS];
int game[MAX_PLAYERS];
int win[MAX_PLAYERS];

int game_loop_start(int fd, char* msg, int player) {
	char buffer[BUFFER_SIZE];
    char guess[BUFFER_SIZE];
	char temp[BUFFER_SIZE];
    char response[BUFFER_SIZE];
	int r;
    int mistakes;

	char msg_hello[] = "WELLCOME! \nType the character you think that is in the word. \n";
	char msg_guess[] = "\nYour guess: ";
	char msg_win[] = "WIN!\n";
	char msg_lose[] = "LOSE!\n";

    strcpy(guess, hangman[player]->guess); // server
	strcpy(temp, STICKERMAN[hangman[player]->mistakes]);
	strcpy(response, msg);
	char* teste = strncat(guess, msg_guess, 14);
	send(fd, temp, sizeof(temp), 0);
	send(fd, guess, sizeof(guess), 0);
}

int game_loop(int fd, char* msg, int player) {
    char buffer[BUFFER_SIZE];
    char guess[BUFFER_SIZE];
	char temp[BUFFER_SIZE];
    char response[BUFFER_SIZE];
	int r;
    int mistakes;

	char msg_hello[] = "WELLCOME! \nType the character you think that is in the word. \n";
	char msg_guess[] = "\nYour guess: ";
	char msg_win[] = "WIN!\n";
	char msg_lose[] = "LOSE!\n";


	printf("%s\n", hangman[player]->word);
	if (strlen(response) > 1) {
		r = evaluate(response[0], hangman[player]); // server
        strcpy(guess, hangman[player]->guess); // server
        mistakes = hangman[player]->mistakes;  // server

        switch (r) {
            case CORRECT:
                printf("Nice job. \n");
                break;
            case WRONG:
                printf("Oh no! \n");
                break;
            case WIN:
                printf("Congratulations my little grashooper! \n");
                break;
            case LOSE:
                printf("Better Luck next time. \n");
                break;
        }

        if (r == WIN) {
            win[player] = 1;
        } else if (r == LOSE) {
			win[player] = 0;
		}
	}

    strcpy(guess, hangman[player]->guess); // server
	strcpy(temp, STICKERMAN[hangman[player]->mistakes]);
	strcpy(response, msg);
	char* teste = strncat(guess, msg_guess, 14);


	send(fd, temp, sizeof(temp), 0);
	if (win[player] == 0) {
		send(fd, msg_lose, sizeof(msg_lose), 0);
		return 1;
	} else if (win[player] == 1) {
		send(fd, msg_win, sizeof(msg_win), 0);
		return 1;
	} else {
		send(fd, guess, sizeof(guess), 0);
	}

	return 0;
}

void startgame(int player) {
	game[player] = 1;
	win[player] = -1;
	hangman[player] = create_game();
}

void finishgame(int player) {
	game[player] = 0;
	win[player] = -1;
}

int start_server(void) {
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

int add_connection(struct pollfd *fds, int newsock, int *nfds) {
	for (int i = 0; i < POLL_LENGHT; i++) {
		if (fds[i].fd != 0) {
			continue;
		}
		fds[i].fd = newsock;
		fds[i].events = POLLIN;
		(*nfds)++;
		return i;
	}
}

void remove_connection(struct pollfd *fds, int i, int *nfds) {
	fds[i].fd = 0;
	(*nfds)--;
}

void handle_pollin(struct pollfd *fds, int i, int server_fd, int *nfds) {
	struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];
    memset(&buffer, 0, BUFFER_SIZE);
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
			int newplayer = 0;
			newplayer = add_connection(fds, newsock, nfds);
			printf("new player %d\n", newplayer);
			startgame(newplayer);
			game_loop_start(newsock, "", newplayer);
		}

		fflush(stdout);

		return;
	}

	/* operation request */
	int p = recv(fds[i].fd, &buffer, BUFFER_SIZE, 0);
	if (p > 0) {
        printf("%s", buffer);
		if (game_loop(newsock, buffer, i) != 0) {
			finishgame(i);
			close(fds[i].fd);
			remove_connection(fds, i, nfds);
		}
        //memset(&buffer, 0, 500);
        fflush(stdout);
	} else {
		finishgame(i);
		close(fds[i].fd);
		remove_connection(fds, i, nfds);
		printf("client disconnected\n");
	}
}

int main() {
	int server_fd;
	int nfds;
	struct pollfd fds[POLL_LENGHT] = {{.fd = 0, .events = POLLIN}};

	server_fd = start_server();
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
                    handle_pollin(fds, i, server_fd, &nfds);
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