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

#include <hangman.h>
#include <client.h>

int ret, fd;

void startclient() {
	struct sockaddr_in addr;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	addr.sin_port = htons(SERVER_PORT);

	if ((ret = connect(fd, (struct sockaddr *) &addr, sizeof(addr))) < 0) {
		perror("unable to connect");
		exit(1);
	}
}

void startgame() {
    char buffer[BUFFER_SIZE];
    char guess[BUFFER_SIZE];
    char sair[] = "sair";
    while (1) {
        recv(fd, buffer, BUFFER_SIZE, 0);
        printf("%s\n", buffer);
        fflush(stdout);

        if (!strcmp(buffer, sair)) {
            printf("Você saiu do jogo.\n");
            return;
        }

        fgets(guess, BUFFER_SIZE, stdin);
        guess[strcspn(guess, "\n")] = '\0';
        fflush(stdin);
        //printf("%s\n", guess);
        send(fd, guess, sizeof(guess), 0);
    }
    return;
}

int game_loop() {
    char buffer[256];
    char guess[256];
    int response;
    int mistakes;

    struct Hangman * hangman = create_game(); // server
    strcpy(guess, hangman->guess); // server

    printf("WELLCOME! \n");
    printf("Type the character you think that is in the word. \n\n");
    printf("%s \n", STICKERMAN[0]);
    printf("%s \n", hangman->guess);

    while (1) {
        printf("\nYour guess: ");
        scanf("%s", buffer);
        printf("\n");

        response = evaluate(buffer[0], hangman); // server
        strcpy(guess, hangman->guess); // server
        mistakes = hangman->mistakes;  // server

        printf("\n\n\n");
        printf("%s \n", STICKERMAN[mistakes]);
        printf("%s \n", guess);

        switch (response) {
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

        if (response == WIN || response == LOSE) {
            break;
        }
    }

    // terminar a conexão
}



int main() {
    startclient();
    startgame(fd);
    close(fd);

    // if (fd < 0) {
    //     printf("Error while connecting to server \n");
    //     exit(0);
    // }

    // char message[] = "Hell World! \n";

    // send(fd, message, sizeof(message), 0);

    return 0;
}