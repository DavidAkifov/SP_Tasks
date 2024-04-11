#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>


enum State {
    START, // 0
    POSITIVE_SIGN, // 1
    NEGATIVE_SIGN, // 2
    OCTAL,  // 3
    CHECK_OCTAL, // 4
    HEX, // 5
    INTEGER, // 6
    NEGATIVE_INTEGER, // 7
     NEGATIVE_SUFFIX_L, // 8
    SUFFIX_U, // 9
    SUFFIX_L, // 10
    CHECK_REAL, // 11
    REAL_NUMBER_WITHOUT_LEADING, // 12
    REAL_NUMBERS_WITHOUT_E, // 13
    REAL_NUMBERS_WITH_E, // 14
    SUFFIX_REAL_NUMBERS, // 15
    ACCEPT, // 16
    REJECT // 17
};

char previousChar;
char suffix_checker;
int state_count[18] = {0};

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
                return REAL_NUMBERS_WITH_E;
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
                else if (input == '.' )
                {  
                    return REAL_NUMBERS_WITHOUT_E;
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
             
             
            else 
            {
                return REJECT;
            }
            
        case INTEGER:
            if (isdigit(input) || input == '\n') {
                return INTEGER;
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
            if (isdigit(input) || input == '\n') 
            {
                return NEGATIVE_INTEGER;
            }
            else if (input == 'u' || input == 'U') 
            {
                return REJECT;
            }
            else if (input == 'l' || input == 'L') 
            {
                return  NEGATIVE_SUFFIX_L;
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
            else if ((input == 'l'  && previousChar == 'l') || (input == 'L'  && previousChar == 'L') || input == '\n')
            {
                return SUFFIX_L; 
            } 
            else if ((input == 'u' || input == 'U') && (previousChar == 'l' || previousChar == 'L')|| input == '\n')
            {
                return SUFFIX_L; 
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
            else if (input == 'l'  && previousChar == 'l' || input == '\n')
            {
                return SUFFIX_U; 
            } 
            else if (input == 'L'  && previousChar == 'L' || input == '\n')
            {
                return SUFFIX_U; 
            }
            else if (input == 'l' && (previousChar =='u' || previousChar =='U' || input == '\n'))
            {
                return SUFFIX_U; 
            }
            else if (input == 'L' && (previousChar =='u' || previousChar =='U') || input == '\n')
            {
                return SUFFIX_U; 
            }
             
             
            else
            {
                return REJECT;
            }
        case  NEGATIVE_SUFFIX_L:
            if (input == 'u' || input == 'U') 
            {
                return REJECT;
            } 
            else if (input == 'l'  && previousChar == 'l' || input == '\n')
            {
                return  NEGATIVE_SUFFIX_L; 
            } 
            else if (input == 'L'  && previousChar == 'L' || input == '\n')
            {
                return  NEGATIVE_SUFFIX_L; 
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
            if (isdigit(input) || input == '\n')
            {
                return REAL_NUMBERS_WITH_E;
            }
            else if (input == '-' || input == '+')
            {
                return REAL_NUMBERS_WITH_E;
            }
            else if ( (previousChar == '.' || previousChar == 'e' || previousChar == 'E' ) && (input == '\0' || input == '/'))
            {
                return REJECT;
            }
             
            else if (input == 'f' || input == 'F' || input == 'l' || input == 'L' || input == '\n')
            {
                return SUFFIX_REAL_NUMBERS;
            }
            else
            {
                return REJECT;
            }
        case REAL_NUMBER_WITHOUT_LEADING:
            if(input == '\0' || input == '/')
            {
                return REJECT;
            }
            else if (input == 'e' || input == 'E' )
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
                return SUFFIX_REAL_NUMBERS;
            }
            if((previousChar != input) && (input == 'f' || input == 'F'))
            {
                return SUFFIX_REAL_NUMBERS;
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
    
    if (strlen(input) == 1)
    {
        state_count[6]++;
        return 1;
    }

    for (int i = 0; input[i] != '\0'; i++) {
        char current_char = input[i];
        if (input[i-1])
            previousChar = input[i-1];
        current = transition(current, current_char);
        
        // if (current == REJECT) {
        //     printf("Rejected input '%s': ", input);
        //     switch (current) {
        //         case POSITIVE_SIGN:
        //             printf("Positive sign '+' at an unexpected position\n");
        //             break;
        //         case NEGATIVE_SIGN:
        //             printf("Negative sign '-' at an unexpected position\n");
        //             break;
        //         case OCTAL:
        //             printf("Unexpected digit '%c' in octal representation\n", current_char);
        //             break;
        //         case HEX:
        //             printf("Invalid hexadecimal digit '%c'\n", current_char);
        //             break;
        //         case SUFFIX_L:
        //         case SUFFIX_U:
        //             printf("Unexpected suffix '%c'\n", current_char);
        //             break;
        //         case REAL_NUMBER_WITHOUT_LEADING:
        //             printf("Decimal point '.' at an unexpected position\n");
        //             break;
        //         case REAL_NUMBERS_WITHOUT_E:
        //             printf("Unexpected character '%c' in real number\n", current_char);
        //             break;
        //         case REAL_NUMBERS_WITH_E:
        //             printf("Unexpected character '%c' after 'e' in real number\n", current_char);
        //             break;
        //         default:
        //             printf("Unexpected character '%c'\n", current_char);
        //             break;
        //     }
        //     return false;
        // }
    }
    if (current != REJECT)
    {
        state_count[current]++;
        return 1;
    }
    return 0;
}


int main() {
    FILE *file = fopen("test_FSM.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    char line[255] = "";
    int valid_ints_count = 0;

    // I want to get the first number from the file and validate it
    
    
    

    while (fgets(line, sizeof(line), file)) {


    // Tokenize the string by space
    char* token = strtok(line, " ");
    
    // Print each token
    while (token != NULL) 
    {

        if (validate_integer(token)) {
            valid_ints_count++;
        }

        //printf("token: [%s]\n", token);
        token = strtok(NULL, " ");
        

    }
    }
    printf("POSITIVE_INTEGER: %d\n", state_count[6]);
    printf("NEGATIVE_INTEGER: %d\n", state_count[7]);
    printf("OCTAL: %d\n", state_count[3]);
    printf("HEX: %d\n", state_count[5]);
    printf("NEGATIVE_SUFFIX_L  %d\n", state_count[8]);
    printf("SUFFIX_L: %d\n", state_count[10]);
    printf("SUFFIX_U: %d\n", state_count[9]);
    printf("REAL_NUMBERS_WITHOUT_E: %d\n", state_count[13]);
    printf("REAL_NUMBERS_WITH_E: %d\n", state_count[14]);
    printf("SUFFIX_REAL_NUMBERS: %d\n", state_count[15]);


    fclose(file);
    return 0;
}