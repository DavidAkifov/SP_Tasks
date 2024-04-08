#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <ctype.h>
#include <wctype.h>
#include <string.h>
#include "Corrector.h"

#define INITIAL_SENTENCE_LENGTH 500
#define INCREMENT_SIZE 200

wchar_t ch;
int number_of_dots = 0;

// Array of function pointers representing state functions
StateFunction stateFunctions[NUM_STATES] = {
    state_initial,
    state_word,
    state_space,
    state_dot_counting,
    state_dot_removal
};

// Current state variable
State currentState = STATE_INITIAL;
State previousState = STATE_INITIAL;

char *sentence;
int index = 0;


int main(int argc, char* argv[])
{
    // Set the locale to support UTF-8
    setlocale(LC_CTYPE, "en_US.UTF-8");

    FILE *file;

    // Read from the file
    char* input_file = argv[1];
    char* output_file = argv[2];

    // Open the file
    file = fopen(input_file, "r");

    // Check if the file exists
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    // Print each sentence in the console
    printSentences(file);

    // Close the file
    fclose(file);


    return 0;
}

// Function to print each sentence in the console
void printSentences(FILE *file) {
    sentence = (char *)malloc(INITIAL_SENTENCE_LENGTH * sizeof(char));
    int sentenceLength = INITIAL_SENTENCE_LENGTH;

    // Read characters from the file until EOF
    while ((ch = fgetwc(file)) != EOF) {
        // Add the character to the sentence array
        sentence[index++] = ch;
        stateFunctions[currentState]();

        // Check if the character is a sentence-ending punctuation mark
        if (ch == '.' || ch == '?' || ch == '!') {
            // Print the sentence and reset index
            printf("%.*s\n", index, sentence); // Print the sentence directly using printf

            // We have the sentence here
            // Do the correction

            index = 0; // Reset index
        }

        // Check if sentence array is full and realloc
        if (index >= sentenceLength) {
            sentenceLength += INCREMENT_SIZE;
            sentence = (char *)realloc(sentence, sentenceLength * sizeof(char));
            if (sentence == NULL) {
                printf("Memory reallocation failed\n");
                exit(1);
            }
        }
    }

    // Print the last sentence if it doesn't end with a punctuation mark
    if (index > 0) {
        printf("%.*s\n", index, sentence); // Print the last sentence
    }

    // Free dynamically allocated memory
    free(sentence);
}

void state_initial()
{
    printf("Current state: INITIAL\n");

    if (iswalpha(ch)) 
    {
        printf("[%lc] is a letter.\n\n", ch);
        currentState = STATE_WORD;
    }
    else if (iswdigit(ch)) 
    {
        printf("[%lc] is a number.\n\n", ch);
        currentState = STATE_WORD;
    }
    else 
    {
        printf("[%lc] is neither a letter nor a number.\n\n", ch);
    }
}

void state_word()
{
    printf("Current state: WORD\n");

    if (iswalpha(ch)) 
    {
        printf("[%lc] is a letter.\n\n", ch);
        currentState = STATE_WORD;
    }
    else if (iswdigit(ch)) 
    {
        printf("[%lc] is a number.\n\n", ch);
        currentState = STATE_WORD;
    }
    else if (iswspace(ch))
    {
        printf("[%lc] is a space.\n\n", ch);
        currentState = STATE_SPACE;
    }
    else 
    {
        printf("[%lc] is neither a letter nor a number.\n\n", ch);
    }
}

void state_space()
{
    printf("Current state: SPACE\n");
    if (iswalpha(ch)) 
    {
        printf("[%lc] is a letter.\n\n", ch);
        currentState = STATE_WORD;
    }
    else if (iswdigit(ch)) 
    {
        printf("[%lc] is a number.\n\n", ch);
        currentState = STATE_WORD;
    }
    else if (iswspace(ch))
    {
        printf("[%lc] is a space.\n\n", ch);
        currentState = STATE_SPACE;
    }
    else if (ch == '.')
    {
        printf("[%lc] is a dot.\n\n", ch);
        number_of_dots += 1;
        currentState = STATE_DOT_COUNTING;
    }
    else 
    {
        printf("[%lc] is neither a letter nor a number.\n\n", ch);
    }
}

void state_dot_counting()
{
    printf("Current state: DOT_COUNTING\n");
    if (ch == '.')
    {
        printf("[%lc] is a dot.\n\n", ch);
        number_of_dots += 1;
        currentState = STATE_DOT_COUNTING;
    }
    else 
    {
        printf("[%lc] is not a dot.\n", ch);
        currentState = STATE_INITIAL;

        printf("There are %d [.]\n\n", number_of_dots);

        char* new_sentence = (char*) malloc(INITIAL_SENTENCE_LENGTH * sizeof(char));
        strcpy(new_sentence, sentence);
        memmove(&new_sentence[index], &new_sentence[index + 1], strlen(new_sentence) - index);
        printf("Original sentence: %s\n", sentence);
        printf("Modified sentence: %s\n", new_sentence);

        if (number_of_dots > 3)
        {
            // Correct the number of dots to 3
        }
        else if (number_of_dots == 2)
        {
            // Correct the number of dots to 1
        }
    }
}

void state_dot_removal()
{
    printf("Current state: DOT_REMOVAL\n");
}

