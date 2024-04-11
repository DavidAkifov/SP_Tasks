#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <ctype.h>
#include <wctype.h>
#include <string.h>
#include "Corrector.h"
#include "Dictionary.h"

#define INITIAL_SENTENCE_LENGTH 500
#define INCREMENT_SIZE 200

#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN_BG   "\x1b[42m"
#define ANSI_COLOR_RED_BG     "\x1b[41m"
#define ANSI_COLOR_RESET   "\x1b[0m"

wchar_t ch;

int number_of_dots = 0;

// Array of function pointers representing state functions
StateFunction state_functions[NUM_STATES] = {
    state_alphanumeric,
    state_first_punct
};

// Current state variable
State currentState = STATE_ALPHANUMERIC;

wchar_t *sentence;
int current_index = 0;
FILE *file;

int main(int argc, char* argv[]) {
    // Set the locale to support UTF-8
    setlocale(LC_CTYPE, "en_US.UTF-8");

    // Read from the file
    char* input_file = "simple.txt";
    char* output_file = argv[2];

    // Open the file
    file = fopen(input_file, "r+, ccs=UTF-8");

    // Check if the file exists
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    // Print each sentence in the console
    print_sentences();

    // Close the file
    fclose(file);


    return 0;
}

// Function to print each sentence in the console
void print_sentences() {
    sentence = (wchar_t *)malloc(INITIAL_SENTENCE_LENGTH * sizeof(wchar_t));
    int sentenceLength = INITIAL_SENTENCE_LENGTH;

    // Read characters from the file until EOF
    while ((ch = fgetwc(file)) != WEOF) {
        state_functions[currentState]();

        // [V] Copy the old in the new         

        // [ ] Perform the correction on the new array

        // [ ] Compare the two arrays with the function we have

        // [ ] Print it colorfully

        // [ ] Write the corrected sentence to the output file


        // Check if sentence array is full and realloc
        if (current_index >= sentenceLength) {
            sentenceLength += INCREMENT_SIZE;
            sentence = (wchar_t *)realloc(sentence, sentenceLength * sizeof(wchar_t));
            if (sentence == NULL) {
                printf("Memory reallocation failed\n");
                exit(1);
            }
        }
    }
    // Last sentence check
    if (current_index > 0) {
        wprintf(L"L[%ls]\n", sentence);
    }

    // Free dynamically allocated memory
    free(sentence);
}

void state_alphanumeric()
{
    // printf("Current state: ALPHANUMERIC\n");

    if (is_punct(ch)/* && ch != L','*/) 
    {
        currentState = STATE_FIRST_PUNCT;
        sentence[current_index++] = ch;
    }
    else 
    {
        currentState = STATE_ALPHANUMERIC;
        sentence[current_index++] = ch;
    }
}

void state_first_punct()
{
    // printf("Current state: FIRST_PUNCT\n");

    if (iswspace(ch) || is_punct(ch) || ch == '-')
    {
        currentState = STATE_FIRST_PUNCT;
        sentence[current_index++] = ch;
    }
    else 
    {
        currentState = STATE_ALPHANUMERIC;
        // seek back one character
        fpos_t position;
        // Get the current position in the file
        int res = fseek(file, -2, SEEK_CUR);
        if (res != 0) {
            perror("Error seeking back one character");
            fclose(file);
        }
        // Print the sentence and reset current_index
        wprintf(L"\nR[%ls]\n", sentence);
        // Copy the sentence to another array
        wchar_t *dest = (wchar_t *)malloc(INITIAL_SENTENCE_LENGTH * sizeof(wchar_t));
        wcscpy(dest, sentence); 
        // wprintf(L"C[%ls]\n", dest);


        remove_space_before_dot(dest);
        count_consecutive_dots(dest);
        remove_comma_after_starting_word(dest);
        remove_comma_before_and_after_punct(dest);
        add_comma_after_starting_word(dest);
        remove_comma_after_che(dest);
        add_comma_after_za_syjalenie(dest);
        add_space_after_coma_before_alphanum(dest);
        wprintf(L"C[%ls]\n", dest);
        printDiff(sentence, dest);
        // printDiffForward(sentence, dest);
        write_corrected_sentence_to_file(dest);

        memset(sentence, 0, INITIAL_SENTENCE_LENGTH);
        // printf("Current index: %d\n", current_index);
        current_index = 0;
    }
}

uint8_t is_punct(wchar_t ch)
{
    return (ch == '.' || ch == '?' || ch == '!' || ch == ',' || ch == ':');
}

void printDiffForward(const wchar_t* str1, const wchar_t* str2) {
    size_t len1 = wcslen(str1);
    size_t len2 = wcslen(str2);

    int dp[INITIAL_SENTENCE_LENGTH][INITIAL_SENTENCE_LENGTH];

    // Initialize the dynamic programming table
    for (size_t i = 0; i <= len1; ++i) {
        for (size_t j = 0; j <= len2; ++j) {
            if (i == 0)
                dp[i][j] = j;  // If the first string is empty, all characters of second string are added
            else if (j == 0)
                dp[i][j] = i;  // If the second string is empty, all characters of first string are deleted
            else if (str1[i - 1] == str2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];  // Characters are same, no operation required
            else
                dp[i][j] = 1 + (dp[i - 1][j - 1] < dp[i - 1][j] ? (dp[i - 1][j - 1] < dp[i][j - 1] ? dp[i - 1][j - 1] : dp[i][j - 1]) : (dp[i - 1][j] < dp[i][j - 1] ? dp[i - 1][j] : dp[i][j - 1])); // Minimum of replace, delete, and insert operations
        }
    }

    // Trace back to find the edits
    wprintf(L"------------------------------\n");
    size_t i = 0, j = 0;
    while (i < len1 || j < len2) {
        if (i < len1 && j < len2 && str1[i] == str2[j]) 
        {
            wprintf(L"%lc", str1[i]);
            // wprintf(L"Unchanged: %lc\n", str1[i - 1]);
            i++;
            j++;
        } else if (j < len2 && (i == len1 || dp[i][j] == dp[i][j + 1] + 1)) 
        {
            wprintf(L"" ANSI_COLOR_GREEN_BG "%lc" ANSI_COLOR_RESET , str2[j]);
            j++;
        } else if (i < len1 && (j == len2 || dp[i][j] == dp[i + 1][j] + 1)) 
        {
            wprintf(L"" ANSI_COLOR_RED_BG "%lc" ANSI_COLOR_RESET , str1[i]);
            i++;
        } else 
        {
            wprintf(L"Replaced: " ANSI_COLOR_RED "%lc" ANSI_COLOR_RESET " with " ANSI_COLOR_GREEN "%lc" ANSI_COLOR_RESET "\n", str1[i], str2[j]);
            i++;
            j++;
        }
    }
    wprintf(L"\n------------------------------\n\n");
}

void printDiff(const wchar_t* str1, const wchar_t* str2) {
    size_t len1 = wcslen(str1);
    size_t len2 = wcslen(str2);

    int dp[INITIAL_SENTENCE_LENGTH][INITIAL_SENTENCE_LENGTH];

    // Initialize the dynamic programming table
    for (size_t i = 0; i <= len1; ++i) {
        for (size_t j = 0; j <= len2; ++j) {
            if (i == 0)
                dp[i][j] = j;  // If the first string is empty, all characters of second string are added
            else if (j == 0)
                dp[i][j] = i;  // If the second string is empty, all characters of first string are deleted
            else if (str1[i - 1] == str2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];  // Characters are same, no operation required
            else
                dp[i][j] = 1 + (dp[i - 1][j - 1] < dp[i - 1][j] ? (dp[i - 1][j - 1] < dp[i][j - 1] ? dp[i - 1][j - 1] : dp[i][j - 1]) : (dp[i - 1][j] < dp[i][j - 1] ? dp[i - 1][j] : dp[i][j - 1])); // Minimum of replace, delete, and insert operations
        }
    }

    // Trace back to find the edits
    wprintf(L"\n------------------------------\n");
    size_t i = len1, j = len2;
    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && str1[i - 1] == str2[j - 1]) 
        {
            wprintf(L"%lc", str1[i - 1]);
            // wprintf(L"Unchanged: %lc\n", str1[i - 1]);
            i--;
            j--;
        } else if (j > 0 && (i == 0 || dp[i][j] == dp[i][j - 1] + 1)) 
        {
            wprintf(L"" ANSI_COLOR_GREEN_BG "%lc" ANSI_COLOR_RESET , str2[j - 1]);
            j--;
        } else if (i > 0 && (j == 0 || dp[i][j] == dp[i - 1][j] + 1)) 
        {
            wprintf(L"" ANSI_COLOR_RED_BG "%lc" ANSI_COLOR_RESET , str1[i - 1]);
            i--;
        } else 
        {
            wprintf(L"Replaced: " ANSI_COLOR_RED "%lc" ANSI_COLOR_RESET " with " ANSI_COLOR_GREEN "%lc" ANSI_COLOR_RESET "\n", str1[i - 1], str2[j - 1]);
            i--;
            j--;
        }
    }
    wprintf(L"\n------------------------------\n");
}

// 1, 4, 5, 8
void remove_space_before_dot(wchar_t* dest)
{
    // Iterate through the wide characters of the sentence
    for (int i = 0; dest[i] != L'\0'; i++) 
    {
        // Check for a space followed by a dot
        if (dest[i] == L' ' && (is_punct(dest[i + 1]) || iswspace(dest[i + 1]))) 
        {
            // wprintf(L"Found a space followed by a dot at index %d.\n", i);
            // Shift characters after the space and dot combination to the left
            int j;
            for (j = i; dest[j] != L'\0'; j++) 
            {
                dest[j] = dest[j + 1];
            }
            // Update the loop control variable to recheck the same position
            i--;
        }
    }
    // wprintf(L"145[%ls]\n", dest);
}

// 7
void add_space_after_coma_before_alphanum(wchar_t* dest)
{

    size_t length = wcslen(dest);
    // printf("Length before: %zu\n", length);

    // Check if the last character is a wide comma
    // printf("Last character: %lc\n", dest[length - 1]);
    if (dest[length - 1] == L',') 
    {
        // printf("Found a wide comma at the end of the sentence.\n");
        // Allocate memory for the new sentence with an additional space
        wchar_t* new_sentence = realloc((wchar_t*)dest, (length + 4) * sizeof(wchar_t));

        if (new_sentence)
        {
            printf("Memory reallocated successfully\n");
            // Add a space after the wide comma
            new_sentence[length] = L' '; // Add the space after the comma
            new_sentence[length + 1] = L'\0'; // Null-terminate the string
            // wprintf(L"New sentence: %ls\n", new_sentence);

            dest = new_sentence;         //deallocation using free has been done assuming that ptr and ptr1 do not point to the same address                     
        }
        else
        {
            free(dest);          //to deallocate the previous memory block pointed by ptr so as not to leave orphaned blocks of memory when ptr=ptr1 executes and ptr moves on to another block
            // Handle the case where realloc fails
            wprintf(L"Memory reallocation failed\n");
            exit(1);
        }
    }
    // printf("Length after: %lu\n", wcslen(dest));
    // wprintf(L"7[%ls]\n", dest);
}

void count_consecutive_dots(wchar_t* dest) 
{
    int consecutive_dots = 0;

    // Iterate through the wide characters of the string
    for (int i = 0; dest[i] != L'\0'; i++) {
        // Check if the character is a dot
        if (dest[i] == L'.') 
        {
            consecutive_dots++;
        }
        else 
        {
            consecutive_dots = 0;
        }
        if (consecutive_dots > 3) 
        {
            // wprintf(L"Found more than 3 consecutive dots at index %d.\n", i);

            // Shift characters after the space and dot combination to the left
            int j;
            for (j = i; dest[j] != L'\0'; j++) 
            {
                dest[j] = dest[j + 1];
            }
            // Update the loop control variable to recheck the same position
            i--;
        }
    }
}

void count_two_consecutive_dots(wchar_t* dest) 
{
    int consecutive_dots = 0;

    // Iterate through the wide characters of the string
    for (int i = 0; dest[i] != L'\0'; i++) {
        // Check if the character is a dot
        if (dest[i] == L'.') 
        {
            consecutive_dots++;
        } 
        else 
        {
            consecutive_dots = 0;
        }
        if (consecutive_dots == 2 && (dest[i+1] != L'.')) 
        {
            // wprintf(L"Found more than 3 consecutive dots at index %d.\n", i);

            // Shift characters after the space and dot combination to the left
            int j;
            for (j = i; dest[j] != L'\0'; j++) 
            {
                dest[j] = dest[j + 1];
            }
            // Update the loop control variable to recheck the same position
            i--;
        }
    }

    count_consecutive_dots(dest);
}

// 9
void remove_comma_after_starting_word(wchar_t* dest) 
{
    int max_length = 0;

    // Iterate over each word sequence
    for (int i = 0; i < NDGR1; i++) {
        const wchar_t* sequence = ndgr1[i];
        int sequence_length = wcslen(dest);

        // Check if the sequence is present in the sentence
        const wchar_t* found = wcsstr(dest, sequence);
        if (found != NULL) {
            // Calculate the length of the sequence found in the sentence
            int length = wcslen(found);

            // Update max_length if the current sequence is longer
            if (length > max_length) {
                max_length = length;
            }
        }
    }
    if(dest[max_length-2] == L',')
    {
        // wprintf(L"dest[max_length]: %lc\n", dest[max_length]);
        // wprintf(L"Found a comma after the starting word at index %d.\n", max_length);
        // Shift characters after the space and dot combination to the left
        int j;
        for (j = max_length-2; dest[j] != L'\0'; j++) 
        {
            dest[j] = dest[j + 1];
        }
        // Update the loop control variable to recheck the same position
        max_length--;
    }
    // printf("Max length: %d\n", max_length);
}

// 6
void remove_comma_before_and_after_punct(wchar_t* dest) 
{
    // Iterate through the wide characters of the sentence
    for (int i = 0; dest[i] != L'\0'; i++) 
    {
        // Check for a comma followed by a dot
        if (dest[i] == L',' && (is_punct(dest[i + 1]) || is_punct(dest[i - 1]))) 
        {
            // wprintf(L"Found a comma followed by a dot at index %d.\n", i);
            // Shift characters after the space and dot combination to the left
            int j;
            for (j = i; dest[j] != L'\0'; j++) 
            {
                dest[j] = dest[j + 1];
            }
            // Update the loop control variable to recheck the same position
            i--;
        }
    }
    count_two_consecutive_dots(dest);
    // wprintf(L"6[%ls]\n", dest);
}

// 10
void add_comma_after_starting_word(wchar_t* dest) 
{
    int length = 0;
    int sequence_length = wcslen(dest);

    // Iterate over each word sequence
    for (int i = 0; i < NDGR2; i++) {
        const wchar_t* sequence = ndgr2[i];

        // Check if the sequence is present in the sentence
        const wchar_t* found = wcsstr(dest, sequence);
        if (found != NULL) {
            // Calculate the length of the sequence found in the sentence
            length = wcslen(ndgr2[i]);
        }
    }
    if(dest[length] == L' ')
    {
        // printf("Found a wide comma at the end of the sentence.\n");
        // Allocate memory for the new sentence with an additional space
        wchar_t* new_sentence = realloc((wchar_t*)dest, (sequence_length + 4) * sizeof(wchar_t));

        if (new_sentence)
        {
            size_t array_length = wcslen(new_sentence);
            printf("Memory reallocated successfully\n");
            // Shift all characters after the first word to the right
            for (size_t i = array_length - 1; i > length; i--) 
            {
                new_sentence[i] = new_sentence[i - 1];
            }

            // Add a space after the wide comma
            new_sentence[length] = L','; // Add the space after the comma
            // wprintf(L"New sentence: %ls\n", new_sentence);

            dest = new_sentence;
        }
        else
        {
            free(dest);
            // Handle the case where realloc fails
            wprintf(L"Memory reallocation failed\n");
            exit(1);
        }
    }
}

// 19
void remove_comma_after_che(wchar_t *dest) {
    // Find the position of the substring "че"
    wchar_t *che_pos = wcsstr(dest, L"че");

    if (che_pos != NULL) {
        // Calculate the index of the comma following the substring "че"
        size_t comma_index = che_pos - dest + 2;

        // Check if there is a comma following the substring "че"
        if (dest[comma_index] == L',') {
            // Shift all characters after the comma to the left
            for (size_t i = comma_index; dest[i] != L'\0'; i++) {
                dest[i] = dest[i + 1];
            }
        }
    }
    // wprintf(L"New sentence: %ls\n", dest);
}

// 16
void add_comma_after_za_syjalenie(wchar_t* dest) 
{
    int length = 0;
    int sequence_length = wcslen(dest);
    wchar_t* sorry = L"за съжаление";

    // Find the position of the substring in the array
    wchar_t *substr_pos = wcsstr(dest, sorry);

    if (substr_pos != NULL) {
        // Calculate the index of the character after the substring
        size_t index = substr_pos - dest + wcslen(sorry);

        wchar_t* new_sentence = (wchar_t *)realloc(dest, (wcslen(dest) + 4) * sizeof(wchar_t));
        if (new_sentence == NULL) {
            fprintf(stderr, "Memory reallocation failed\n");
            exit(1);
        }
        // Insert a comma after the substring
        for (size_t i = wcslen(new_sentence) + 1; i > index; i--) {
            new_sentence[i] = new_sentence[i - 1];
        }
        new_sentence[index] = L',';

        dest = new_sentence;
    }

    // wprintf(L"16New sentence: %ls\n", dest);
}

void write_corrected_sentence_to_file(wchar_t* dest)
{
    // Open a file for writing in wide character mode
    FILE *file = fopen("corrected.txt", "a, ccs=UTF-8");
    if (file == NULL) {
        wprintf(L"Failed to open file for writing.\n");
    }

    fwprintf(file, L"%ls\n", dest);

    // Close the file
    fclose(file);
}