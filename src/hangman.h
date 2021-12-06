#ifndef HANGMAN_H
#define HANGMAN_H

int NUM_WORDS = 4;

char * RANDOM_WORDS[] = {
    "Hello",
    "Word",
    "Mama",
    "Mia"
};

enum BODY_PARTS {
    HEAD,
    BODY,
    ARMS,
    MAX_VALUES
}

struct Hangman {
    char word[256];
    char guess[256];
    int mistakes;
};

struct Hangman * create_game();

void fill_gaps(char * guess, char * word);

int belongs(char c, char * word);

int complete_word(char c, char * guess, char * word);

#endif
