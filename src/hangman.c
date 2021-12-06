#include <string.h>
#include <stdlib.h>

#include "hangman.h"


struct Hangman * create_game() {
    struct Hangman * hangman = malloc(sizeof(struct Hangman));
    hangman->mistakes = 0;
    choose_word(NUM_WORDS, RANDOM_WORDS, hangman->word);
    fill_gaps(hangman->guess, hangman->word);    
}

int evaluate(char c, struct Hangman * hangman) {
    if (hangman->mistakes >= MAX_VALUES) {
        return 0;
    }


}

void choose_word(int size, char ** vector, char * word) {
    int opt = random() % size;
    strcpy(word, vector[opt]);
}

void fill_gaps(char * guess, char * word) {
    int size = strlen(word);

    for (int i = 0; i < size; i++) {
        if (word[i] == ' ') {
            guess[i] = ' ';
        } else {
            guess[i] = '_';
        }
    }

    guess[size] = '\0'; // end char
}

int belongs(char c, char * word) {
    for (int i = 0; i < strlen(word); i++) {
        if (word[i] == c) {
            return 1;
        }
    }
    return 0;
}

int complete_word(char c, char * guess, char * word) {
    for (int i = 0; i < strlen(word); i++) {
        if (word[i] == c) {
            guess[i] = c;
        }
    }
}