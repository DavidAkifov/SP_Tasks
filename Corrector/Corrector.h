#ifndef CORRECTOR_H
#define CORRECTOR_H

#include <stdio.h>

// Define states
typedef enum {
    STATE_INITIAL,
    STATE_WORD,
    STATE_SPACE,
    STATE_DOT_COUNTING,
    STATE_DOT_REMOVAL,
    STATE_SENTENCE_END,
    NUM_STATES // Number of states
} State;

// Function pointer type for state functions
typedef void (*StateFunction)();

void printSentences(FILE *file);

// Function prototypes for state functions
void state_initial();
void state_word();
void state_space();
void state_dot_counting();
void state_dot_removal();
void state_setnence_end();

#endif // CORRECTOR_H