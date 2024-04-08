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

char previousChar; // Глобална променлива за предходния символ

enum State transition(enum State current, char input) // Функция за преминаване от един състояние в друго
{
    switch(current) // Проверяваме текущото състояние
    {
        case START: // Ако сме в началото
            if (isdigit(input) && input != '0') // Проверяваме дали входа е цифра и различна от 0
            {
                return INTEGER; // Ако е връщаме цяло число
            }
            else if ( input == '.') // Проверяваме дали входа е точка
            {
                return REAL_NUMBER_WITHOUT_LEADING; // Ако е отиваме в състояние реално число без начална цифра
            }
            else if ( input == '-') // Проверяваме дали входа е минус
            {
                return NEGATIVE_SIGN; // Ако е отиваме в състояние отрицателно число
            }
            else if (input == '+' ) // Проверяваме дали входа е плюс
            {
                return POSITIVE_SIGN; // Ако е отиваме в състояние положително число
            }
            else if (input == '0') // Проверяваме дали входа е 0
            {
                return CHECK_OCTAL; // Ако е отиваме в състояние проверка за осмично число
            }
            else
            {
                return REJECT; // Ако не отиваме в състояние отхвърляне
            }
        case POSITIVE_SIGN: // Ако сме в състояние положително число
            if (isdigit(input) && input != '0') // Проверяваме дали входа е цифра и различна от 0
            {
                return INTEGER; // Ако е връщаме цяло число
            }
            else if(input == '0') //Проверяваме дали входа е 0
            {
                return OCTAL; // Ако е връщаме осмично число
            }
            else if(input == '.') // Проверяваме дали входа е точка
            {
                return REAL_NUMBER_WITHOUT_LEADING; // Ако е връщаме реално число без начална цифра
            }
            else 
            {
                return REJECT; // Ако не отиваме в състояние отхвърляне
            }
        case NEGATIVE_SIGN: // Ако сме в състояние отрицателно число
            if (input == '0') // Проверяваме дали входа е 0
            {
                return CHECK_REAL; // Ако е връщаме състояние проверка за реално число
            }
            else if (input == '.') // Проверяваме дали входа е точка            
            {
                return REAL_NUMBER_WITHOUT_LEADING; // Ако е връщаме състояние реално число без начална цифра
            }
            else if (isdigit(input)) // Проверяваме дали входа е цифра
            {
                return NEGATIVE_INTEGER; // Ако е връщаме отрицателно число
            }
            else 
            {
                return REJECT; // Ако не отиваме в състояние отхвърляне
            }

        case CHECK_OCTAL: // Ако сме в състояние проверка за осмично число
            {
                if (input == '0') // Проверяваме дали входа е 0
                {
                    return CHECK_REAL;
                }
                else if (input > '0' && input < '8') // Проверяваме дали входа е между 1 и 7
                {
                    return OCTAL; // Ако е връщаме осмично число
                }
                else if (input == 'x' || input == 'X') // Проверяваме дали входа е x или X
                {
                    return HEX; // Ако е връщаме шестнадесетично число
                }
                else
                {
                    return REJECT; // Ако не отиваме в състояние отхвърляне
                }
                
            }
        case OCTAL: // Ако сме в състояние осмично число
            if(input == '8' || input == '9') // Проверяваме дали входа е 8 или 9
            {
                return CHECK_REAL; // Ако е връщаме състояние проверка за реално число
            }
            else if (isdigit(input)) // Проверяваме дали входа е цифра            
            {
                return OCTAL; // Ако е връщаме осмично число
            }
            else if (isspace(input) || input == '\0' || input == '/') // Проверяваме дали входа е интервал, край на ред или /
            {
                return ACCEPT; // В този случай приемаме числото
            }
            else if (input == 'u' || input == 'U') // Проверяваме дали входа е u или U
            {
                return SUFFIX_U; // Ако е връщаме състояние суфикс u
            }
            else if (input == 'l' || input == 'L') // Проверяваме дали входа е l или L
            {
                return SUFFIX_L; // Ако е връщаме състояние суфикс l
            }
            else if (input == 'x' || input == 'X' ) // Проверяваме дали входа е x или X
            {
                return HEX; // Ако е връщаме шестнадесетично число
            }
            else if (input == '.') // Проверяваме дали входа е точка
            {
                return REAL_NUMBERS_WITHOUT_E; // Ако е връщаме състояние реално число без eкспонента
            }
            else
            {
                return REJECT; // Ако не отиваме в състояние отхвърляне
            }
        case HEX:
            if ((input >= 'A' && input <= 'F') 
            || (input >= 'a' && input <= 'f') 
            || (input >= '0' && input <= '9')) // Проверяваме дали входа е между A и F, a и f или 0 и 9
            {
                return HEX; // Ако е връщаме шестнадесетично число
            }
            else if (input == 'u' || input == 'U') // Проверяваме дали входа е u или U
            {
                return SUFFIX_U; // Ако е връщаме състояние суфикс u
            }
            else if (input == 'l' || input == 'L') // Проверяваме дали входа е l или L
            {
                return SUFFIX_L; // Ако е връщаме състояние суфикс l
            }
            else if ((previousChar == 'x' || previousChar == 'X' ) && (input) || input == '\0' || input == '/') // Проверяваме дали предходния символ е x или X и дали входа е интервал, край на ред или /
            {
                return REJECT; // Ако е връщаме състояние отхвърляне
            }
            else if (isspace(input) || input == '\0' || input == '/') // Проверяваме дали входа е интервал, край на ред или /            
            {
                return ACCEPT; // В този случай приемаме числото
            }
            else 
            {
                return REJECT; // Ако не отиваме в състояние отхвърляне
            }
            
        case INTEGER:
            if (isdigit(input)) 
            {
                return INTEGER; 
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

bool validate_integer(const char *input) // Функция за валидиране на цяло число
{
    enum State current = START; // Задаваме начално състояние

    for (int i = 0; input[i] != '\0'; i++) // Цикъл за обхождане на входа
    {
        char current_char = input[i]; // Взимаме текущия символ
        if (input[i-1]) // Проверяваме дали има предходен символ
            previousChar = input[i-1]; // Ако има го запазваме в глобалната променлива за предходния символ
        current = transition(current, current_char); // Преминаваме от едно състояние в друго
        if (current == REJECT) // Проверяваме дали сме в състояние отхвърляне
        {
            printf("Rejected input '%s': ", input);
            switch (current) // Проверяваме в кое състояние сме
            {
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

    return current == ACCEPT; // Връщаме дали сме в състояние приемане
}


int main() {
    FILE *file = fopen("tests.txt", "r"); // Отваряме файла за четене
    if (file == NULL) { // Проверяваме дали файла е отворен успешно
        perror("Error opening file"); // Ако не е изписваме грешка
        return 1; // Излизаме от програмата
    }

    char line[255]; // Създаваме масив от символи с дължина 255
    int valid_ints_count = 0; // Брояч за валидните числа

    while (fgets(line, sizeof(line), file)) { // Четем ред по ред от файла
        char *newline_pos = strchr(line, '\n'); // Намираме позицията на новия ред
        if (newline_pos != NULL) { // Проверяваме дали новия ред съществува
            *newline_pos = '\0'; // Ако съществува го заменяме с терминираща нула
        }

        char *comment_pos = strchr(line, '/'); // Намираме позицията на коментара
        if (comment_pos != NULL) // Проверяваме дали коментарът съществува
        {
            *comment_pos = '\0'; // Ако съществува го заменяме с терминираща нула
        }

        char *number_start = line; // Започваме от началото на реда
        while (isspace(*number_start)) // Прескачаме всички интервали
        { 
            number_start++; // Увеличаваме указателя
        }

        if (validate_integer(number_start)) // Проверяваме дали числото е валидно
        {
            valid_ints_count++; // Увеличаваме брояча на валидните числа
            printf("Accepted input: %s\n", number_start); // Изписваме валидното число
        }
    }

    fclose(file); // Затваряме файла
    printf("Total valid integers: %d\n", valid_ints_count); // Изписваме общия брой на валидните числа
    return 0; // Излизаме от програмата
}