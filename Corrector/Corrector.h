#ifndef CORRECTOR_H
#define CORRECTOR_H

#include <stdio.h>
#include <wchar.h>
#include <stdint.h>

// Define states
typedef enum {
    STATE_ALPHANUMERIC,
    STATE_FIRST_PUNCT,
    NUM_STATES // Number of states
} State;

// Function pointer type for state functions
typedef void (*StateFunction)();

void print_sentences();

// Function prototypes for state functions
void state_alphanumeric();
void state_first_punct();

uint8_t is_punct(wchar_t ch);
void printDiff(const wchar_t* str1, const wchar_t* str2);
void remove_space_before_dot(wchar_t* dest);
void add_space_after_coma_before_alphanum(wchar_t* dest);
void count_consecutive_dots(wchar_t* dest);

#endif // CORRECTOR_H