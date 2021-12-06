#ifndef HANGMAN_H
#define HANGMAN_H

#define WORD_SIZE 256
#define NUM_BODY_PARTS 4

int NUM_WORDS = 4;
char * RANDOM_WORDS[] = {
    "Hello",
    "Word",
    "Mia",
    "Mama"
};

enum BODY_PARTS {
    NONE,
    HEAD,
    BODY,
    ARMS,
    LEGS
};

enum RESULTS {
    CORRECT,
    WRONG,
    WIN,
    LOSE
};

struct Hangman {
    char word[WORD_SIZE];
    char guess[WORD_SIZE];
    int mistakes;
};

struct Hangman * create_game();

int evaluate(char c, struct Hangman * hangman);

int get_body_part(struct Hangman * hangman);

void choose_word(char * word);

void fill_gaps(char * guess, char * word);

int belongs(char c, char * word);

int complete_word(char c, char * guess, char * word);

#endif
