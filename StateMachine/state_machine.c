#include <stdio.h>
#include <string.h> // Include string.h for strstr function
#include <stdbool.h>
#include <ctype.h> // For isdigit function

// Enum representing the states of the FSM
typedef enum {
    START,
    INTEGER,
    SUFFIX,
    ZERO,
    OCTAL,
    OCTAL_WITH_SIGN,
    HEX_PREFIX,
    HEXADECIMAL,
    ACCEPT,
    REJECT
} State;

// // Function to print debug information
// void printDebug(const char *message, const char *str, State currentState, char currentChar) {
//     printf("%s: Current State: %d, Current Char: %c\n", message, currentState, currentChar);
// }

// Function to validate integer representation using FSM
bool isValidInteger(const char *str) {
    State currentState = START;
    bool isNegative = false; // Flag to track if the number is negative
    
    // Iterate through the input string
    while (*str != '\0') {
        char currentChar = *str;

        // Determine the next state based on the current state and input character
        switch (currentState) {
            case START:
                if (currentChar == '+' || currentChar == '-') {
                    isNegative = (currentChar == '-');
                    currentState = INTEGER;
                } else if (currentChar == '0') {
                    currentState = ZERO;
                } else if (isdigit(currentChar)) {
                    currentState = INTEGER;
                } else {
                    currentState = REJECT;
                }
                break;
            case INTEGER:
                if (isdigit(currentChar)) {
                    currentState = INTEGER;
                } else if (currentChar == 'u' || currentChar == 'U' || currentChar == 'l' || currentChar == 'L') {
                    currentState = SUFFIX;
                } else if (isspace(currentChar)) {
                    currentState = ACCEPT;
                } else {
                    currentState = REJECT;
                }
                break;
            case SUFFIX:
                if (isspace(currentChar)) {
                    currentState = ACCEPT;
                } else {
                    currentState = REJECT;
                }
                break;
            case ZERO:
                if (currentChar == 'x' || currentChar == 'X') {
                    currentState = HEX_PREFIX;
                } else if (isdigit(currentChar) || currentChar == '+' || currentChar == '-') {
                    currentState = OCTAL_WITH_SIGN;
                } else if (isspace(currentChar)) {
                    currentState = ACCEPT;
                } else {
                    currentState = REJECT;
                }
                break;
            case OCTAL:
                if (isdigit(currentChar)) {
                    currentState = OCTAL;
                } else if (currentChar == 'u' || currentChar == 'U' || currentChar == 'l' || currentChar == 'L') {
                    currentState = SUFFIX;
                } else if (isspace(currentChar)) {
                    currentState = ACCEPT;
                } else {
                    currentState = REJECT;
                }
                break;
            case OCTAL_WITH_SIGN:
                if (isdigit(currentChar)) {
                    currentState = OCTAL;
                } else {
                    currentState = REJECT;
                }
                break;
            case HEX_PREFIX:
                if (isdigit(currentChar)) {
                    currentState = HEXADECIMAL;
                } else {
                    currentState = REJECT;
                }
                break;
            case HEXADECIMAL:
                if (isdigit(currentChar)) {
                    currentState = HEXADECIMAL;
                } else if (currentChar == 'u' || currentChar == 'U' || currentChar == 'l' || currentChar == 'L') {
                    currentState = SUFFIX;
                } else if (isspace(currentChar)) {
                    currentState = ACCEPT;
                } else {
                    currentState = REJECT;
                }
                break;
            case ACCEPT:
            case REJECT:
                // Terminal states, no transitions
                break;
        }

        
        // If the current state is REJECT, no need to continue
        if (currentState == REJECT) {
            break;
        }

        // Move to the next character in the input string
        str++;
    }

    // Return true if the FSM ends in the ACCEPT state
    return currentState == ACCEPT && !isNegative; // Ensure negative numbers are not considered valid
}




int main() {
    // Open the file for reading
    FILE *file = fopen("tests.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }

    int validIntegerCount = 0; // Counter for valid integers

    // Read and process each line from the file
    char line[256]; // Assume max line length is 255 characters
    while (fgets(line, sizeof(line), file)) {
        // Ignore lines starting with '#' (comments)
        if (line[0] == '#' || line[0] == '/') {
            printf("Skipping comment line: %s", line);
            continue;
        }
        
        // Check if the line contains 'VALID INTS:' or 'INVALID INTS:'
        
        printf("Found header: %s", line);
        // Read the next line containing integer representations
        while (fgets(line, sizeof(line), file)) {
            // Stop reading if the line is empty or starts with '#'
            if (line[0] == '\n' || line[0] == '#') {
                printf("End of integer representations.\n");
                break;
            }
            
            // Trim trailing newline character
            line[strcspn(line, "\n")] = '\0';
            
            // Validate the integer representation
            if (isValidInteger(line)) {
                printf("%s: Valid\n", line);
                validIntegerCount++; // Increment counter for valid integers
            } else {
                printf("%s: Invalid\n", line);
            }
        }
    }

    // Close the file
    fclose(file);

    // Print the total count of valid integers
    printf("Total valid integers: %d\n", validIntegerCount);

    return 0;
}
