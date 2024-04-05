#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

// Define states
enum State {
    START,
    POSITIVE_SIGN,
    NEGATIVE_SIGN,
    OCTAL,
    HEX,
    INTEGER,
    NEGATIVE_INTEGER,
    NEGATIVE_SUFFIX,
    SUFFIX_U,
    SUFFIX_L,
    ACCEPT,
    REJECT

};

char previousChar;
char suffix_checker;

// Function to determine the next state based on the current state and input character
enum State transition(enum State current, char input) {
    switch(current) {
        case START:
            if (isdigit(input) && input != '0') 
            {
                return INTEGER;
            }
            else if ( input == '-')
            {
                return NEGATIVE_SIGN;
            }
            else if (input == '+' )
            {
                return POSITIVE_SIGN;
            }
             else if (input == '0') 
            {
                return OCTAL;
            } 
            else
            {
                return REJECT;
            }
        case POSITIVE_SIGN:
            if (isdigit(input) && input != '0')
            {
                return INTEGER;
            }
            else if(input == '0')
            {
                return OCTAL;
            }
            else 
            {
                return REJECT;
            }
        case NEGATIVE_SIGN:
            if (input == '0') 
            {
                return REJECT;
            }
            else if (isdigit(input))
            {
                return NEGATIVE_INTEGER;
            }
        case OCTAL:
            if(input == '8' || input == '9')
            {
                return REJECT;
            }
            else if (isdigit(input))
            {
                return OCTAL;
            }
            else if (isspace(input) || input == '\0' || input == '/')
            {
                return ACCEPT;
            }
            else if (input == 'u' || input == 'U')
            {
                return SUFFIX_U;
            }
            else if (input == 'l' || input == 'L')
            {
                return SUFFIX_L;
            }
            else if (input == 'x' || input == 'X' )
            {
                return HEX;
            }
            else
            {
                return REJECT;
            }
        case HEX:
            if ((input >= 'A' && input <= 'F') || (input >= 'a' && input <= 'f') || (input >= '0' && input <= '9'))
            {
                return HEX;
            }
            else if (input == 'u' || input == 'U')
            {
                return SUFFIX_U;
            }
            else if (input == 'l' || input == 'L')
            {
                return SUFFIX_L;
            }
            else if ((previousChar == 'x' || previousChar == 'X' ) && (input) || input == '\0' || input == '/')
            {
                return REJECT;
            }
            else if (isspace(input) || input == '\0' || input == '/')
            {
                return ACCEPT;
            }
            else 
            {
                return REJECT;
            }
            
        case INTEGER:
            if (isdigit(input)) {
                return INTEGER;
            } else if (isspace(input) || input == '\0' || input == '/')
            {
                return ACCEPT;
            } 
            else if (input == 'u' || input == 'U')
            {
                return SUFFIX_U;
            }
            else if (input == 'l' || input == 'L')
            {
                return SUFFIX_L;
            }
            else
            {
                return REJECT;
            }
        case NEGATIVE_INTEGER:
            if (isdigit(input)) 
            {
                return NEGATIVE_INTEGER;
            }
            else if (isspace(input) || input == '\0' || input == '/') 
            {
                return ACCEPT;
            }
            else if (input == 'u' || input == 'U') 
            {
                return REJECT;
            }
            else if (input == 'l' || input == 'L') 
            {
                return NEGATIVE_SUFFIX;
            }
            else
            {
                return REJECT;
            }
        case SUFFIX_L:
            if ((previousChar == 'u' || previousChar == 'U') && (input == 'l'|| input == 'L'))
            {
                return REJECT;
            } 
            else if (input == 'l'  && previousChar == 'l')
            {
                return SUFFIX_L; 
            } 
            else if (input == 'L'  && previousChar == 'L')
            {
                return SUFFIX_L; 
            }
            else if ((input == 'u' || input == 'U') && (previousChar == 'l' || previousChar == 'L'))
            {
                return SUFFIX_L; 
            }
            else if (isspace(input) || input == '\0' || input == '/')
            {
                return ACCEPT;
            }
            else
            {
                return REJECT;
            } 
        case SUFFIX_U:
            if ((input == 'u'|| input == 'U'))
            {
                return REJECT;
            } 
            else if (input == 'l'  && previousChar == 'l')
            {
                return SUFFIX_U; 
            } 
            else if (input == 'L'  && previousChar == 'L')
            {
                return SUFFIX_U; 
            }
            else if (input == 'l' && (previousChar =='u' || previousChar =='U'))
            {
                return SUFFIX_U; 
            }
            else if (input == 'L' && (previousChar =='u' || previousChar =='U'))
            {
                return SUFFIX_U; 
            }
            else if (isspace(input) || input == '\0' || input == '/')
            {
                return ACCEPT;
            }
            else
            {
                return REJECT;
            }
        case NEGATIVE_SUFFIX:
            if (input == 'u' || input == 'U') 
            {
                return REJECT;
            } 
            else if (input == 'l'  && previousChar == 'l')
            {
                return NEGATIVE_SUFFIX; 
            } 
            else if (input == 'L'  && previousChar == 'L')
            {
                return NEGATIVE_SUFFIX; 
            }
            else if (isspace(input) || input == '\0' || input == '/')
            {
                return ACCEPT;
            }
            else 
            {
                return REJECT;
            }
        default:
            return REJECT;
    }
    
}

// Function to validate input against the FSM
bool validate_integer(const char *input) {
    enum State current = START;

    for (int i = 0; input[i] != '\0'; i++) {
        char current_char = input[i];
        if(input[i-1])
            previousChar = input[i-1];
        // // Convert 'l' or 'u' to uppercase
        // if (current_char == 'l' || current_char == 'u') {
        //     current_char = toupper(current_char);
        // }
        current = transition(current, current_char);
        if (current == REJECT) {
            printf("Rejected input '%s': ", input);
            switch (current_char) {
                case '+':
                    printf("Positive sign '+' at an unexpected position\n");
                    break;
                case '-':
                    printf("Negative sign '-' at an unexpected position\n");
                    break;
                case 'l':
                case 'L':
                case 'u':
                case 'U':
                    printf("Unexpected suffix '%c'\n", current_char);
                    break;
                default:
                    printf("Unexpected character '%c'\n", current_char);
                    break;
            }
            return false;
        }
    }

    return current == ACCEPT;
}

int main() {
    FILE *file = fopen("tests.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    char line[100]; // Assuming each line in the file is at most 100 characters long
    int valid_ints_count = 0;

    while (fgets(line, sizeof(line), file)) {
        // Trim trailing newline character if present
        char *newline_pos = strchr(line, '\n');
        if (newline_pos != NULL) {
            *newline_pos = '\0';
        }

        // Find the position of the comment symbol
        char *comment_pos = strchr(line, '/');
        if (comment_pos != NULL) {
            // Null terminate the string just before the comment symbol
            *comment_pos = '\0';
        }

        // Skip leading whitespace
        char *number_start = line;
        while (isspace(*number_start)) {
            number_start++;
        }

        // Validate the integer part before the comment
        if (validate_integer(number_start)) {
            valid_ints_count++;
            printf("Accepted input: %s\n", number_start);
        }
    }

    fclose(file);
    printf("Total valid integers: %d\n", valid_ints_count);
    return 0;
}