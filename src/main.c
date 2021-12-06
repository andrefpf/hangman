#include <stdio.h>
#include <string.h>

#include "hangman.c"

int main() {
    char word[256];
    
    for (int i = 0; i < 10; i++) {
        choose_word(4, RANDOM_WORDS, word);
        printf("%s \n", word);
    }

    return 0;
}