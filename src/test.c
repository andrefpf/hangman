#include <stdio.h>
#include <string.h>

#include "hangman.c"

int main() {
    int a;
    char c;
    struct Hangman * hangman = create_game();

    printf("%s \n", hangman->word);
    printf("%s \n", hangman->guess);


    while (1) {
        c = getc(stdin);
        a = evaluate(c, hangman);
        
        printf("%s \n", hangman->guess);

        printf("%d \n", a);

        if (a == CORRECT) {
            printf("CORRECT \n");
        }

        if (a == WRONG) {
            printf("WRONG");
        }
     
        if (a == WIN) {
            printf("WIN \n");
            break;
        } 

        if (a == LOSE) {
            printf("LOSE \n");
            break;
        }
    }

    free(hangman);

    return 0;
}