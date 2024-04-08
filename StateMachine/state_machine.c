#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

enum State {
    START,
    POSITIVE_SIGN,
    NEGATIVE_SIGN,
    OCTAL,
    CHECK_OCTAL,
    HEX,
    INTEGER,
    NEGATIVE_INTEGER,
    NEGATIVE_SUFFIX,
    SUFFIX_U,
    SUFFIX_L,
    CHECK_REAL,
    REAL_NUMBER_WITHOUT_LEADING,
    REAL_NUMBERS_WITHOUT_E,
    REAL_NUMBERS_WITH_E,
    SUFFIX_REAL_NUMBERS,
    ACCEPT,
    REJECT
};

char previousChar;
char suffix_checker;

enum State transition(enum State current, char input) {
    switch(current) {
        case START:
            if (isdigit(input) && input != '0') 
            {
                return INTEGER;
            }
            else if ( input == '.')
            {
                return REAL_NUMBER_WITHOUT_LEADING;
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
                return CHECK_OCTAL;
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
            else if(input == '.')
            {
                return REAL_NUMBER_WITHOUT_LEADING;
            }
            else 
            {
                return REJECT;
            }
        case NEGATIVE_SIGN:
            if (input == '0') 
            {
                return CHECK_REAL;
            }
            else if (input == '.')
            {
                return REAL_NUMBER_WITHOUT_LEADING;
            }
            else if (isdigit(input))
            {
                return NEGATIVE_INTEGER;
            }
            else 
            {
                return REJECT;
            }

        case CHECK_OCTAL:
            {
                if (input == '0')
                {
                    return CHECK_REAL;
                }
                else if (input > '0' && input < '8')
                {
                    return OCTAL;
                }
                else if (input == 'x' || input == 'X')
                {
                    return HEX;
                }
                else
                {
                    return REJECT;
                }
                
            }
        case OCTAL:
            if(input == '8' || input == '9')
            {
                return CHECK_REAL;
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
            else if (input == '.')
            {
                return REAL_NUMBERS_WITHOUT_E;
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
            else if (input == '.')
            {
                return REAL_NUMBERS_WITHOUT_E;
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
            else if (input == '.')
            {
                return REAL_NUMBERS_WITHOUT_E;
            }
            else if (input == 'e' || input == 'E')
            {
                return REAL_NUMBERS_WITH_E;
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
            else if ((input == 'l'  && previousChar == 'l') || (input == 'L'  && previousChar == 'L'))
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
        case CHECK_REAL: 
            if (input >= '0' && input <= '9')
            {
                return CHECK_REAL;
            }
            else if (input == '.')
            {
                return REAL_NUMBERS_WITHOUT_E;
            }
            else if (input == 'e' || input == 'E')
            {
                return REAL_NUMBERS_WITH_E;
            }
            else
            {
                return REJECT;
            }
        case REAL_NUMBERS_WITHOUT_E:
            if (isdigit(input))
            {
                return REAL_NUMBERS_WITHOUT_E;
            }
            else if (isspace(input) || input == '\0' || input == '/')
            {
                return ACCEPT;
            }
            else if (input =='e' || input == 'E')
            {
                return REAL_NUMBERS_WITH_E;
            }
            else if (input == 'f' || input == 'F' || input == 'l' || input == 'L')
            {
                return SUFFIX_REAL_NUMBERS;
            }
            else
            {
                return REJECT;
            }
        case REAL_NUMBERS_WITH_E:
            if (isdigit(input))
            {
                return REAL_NUMBERS_WITH_E;
            }
            else if (input == '-' || input == '+')
            {
                return REAL_NUMBERS_WITH_E;
            }
            else if ( (previousChar == '.' || previousChar == 'e' || previousChar == 'E' ) && (isspace(input) || input == '\0' || input == '/'))
            {
                return REJECT;
            }
            else if (isspace(input) || input == '\0' || input == '/')
            {
                return ACCEPT;
            }
            else if (input == 'f' || input == 'F' || input == 'l' || input == 'L')
            {
                return SUFFIX_REAL_NUMBERS;
            }
            else
            {
                return REJECT;
            }
        case REAL_NUMBER_WITHOUT_LEADING:
            if(isspace(input) || input == '\0' || input == '/')
            {
                return REJECT;
            }
            else if (input == 'e' || input == 'E')
            {
                return REAL_NUMBERS_WITH_E;
            }
            else if (isdigit(input))
            {
                return REAL_NUMBERS_WITHOUT_E;
            }
            else
            {
                return REJECT;
            }
        case SUFFIX_REAL_NUMBERS:
            if((previousChar != input) && (input == 'l' || input == 'L'))
            {
                return ACCEPT;
            }
            if((previousChar != input) && (input == 'f' || input == 'F'))
            {
                return ACCEPT;
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

bool validate_integer(const char *input) {
    enum State current = START;

    for (int i = 0; input[i] != '\0'; i++) {
        char current_char = input[i];
        if (input[i-1])
            previousChar = input[i-1];
        current = transition(current, current_char);
        if (current == REJECT) {
            printf("Rejected input '%s': ", input);
            switch (current) {
                case POSITIVE_SIGN:
                    printf("Positive sign '+' at an unexpected position\n");
                    break;
                case NEGATIVE_SIGN:
                    printf("Negative sign '-' at an unexpected position\n");
                    break;
                case OCTAL:
                    printf("Unexpected digit '%c' in octal representation\n", current_char);
                    break;
                case HEX:
                    printf("Invalid hexadecimal digit '%c'\n", current_char);
                    break;
                case SUFFIX_L:
                case SUFFIX_U:
                    printf("Unexpected suffix '%c'\n", current_char);
                    break;
                case REAL_NUMBER_WITHOUT_LEADING:
                    printf("Decimal point '.' at an unexpected position\n");
                    break;
                case REAL_NUMBERS_WITHOUT_E:
                    printf("Unexpected character '%c' in real number\n", current_char);
                    break;
                case REAL_NUMBERS_WITH_E:
                    printf("Unexpected character '%c' after 'e' in real number\n", current_char);
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

    char line[255];
    int valid_ints_count = 0;

    while (fgets(line, sizeof(line), file)) {
        char *newline_pos = strchr(line, '\n');
        if (newline_pos != NULL) {
            *newline_pos = '\0';
        }

        char *comment_pos = strchr(line, '/');
        if (comment_pos != NULL) {
            *comment_pos = '\0';
        }

        char *number_start = line;
        while (isspace(*number_start)) {
            number_start++;
        }

        if (validate_integer(number_start)) {
            valid_ints_count++;
            printf("Accepted input: %s\n", number_start);
        }
    }

    fclose(file);
    printf("Total valid integers: %d\n", valid_ints_count);
    return 0;
}