#include <stdio.h>
#include "Calculator_BCD.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

int main()
{
    // Въртим функцията в loop докато не бъде спряна от потребителя
    while(1)
    {
        printf("-------------------------------------------------------------\n");
        int first_number, second_number; //Инициализираме двете стойности, които ще бъдат въведени от потребителя
        char operation_simbol; //И чар за знак
        printf("Enter your first number: ");
        scanf("%d", &first_number); // Чакаме за първото число
        printf("Enter your second number: ");
        scanf("%d", &second_number); // после за второто
        printf("\nFirst Number: ");
        print_bcd_in_binary(convert_to_bcd(first_number),0);
        printf("\nSecond Number: ");
        print_bcd_in_binary(convert_to_bcd(second_number),0);
        printf("Enter Operation (+/-): ");
        fflush(stdin); //чистим стдин зашото иначе сканф взема последния еднтер
        scanf("%c", &operation_simbol);
        getchar(); // взимаме въведения знак
        if(operation_simbol == '+')  // определяме вида на операцията
        {
            uint64_t result = sum_bcd(convert_to_bcd(first_number),convert_to_bcd(second_number)); //смятаме
            printf("Result in BCD(binary): ");
            print_bcd_in_binary(result, 0);//принтим резултат
            printf("\n");
        } 
        else if (operation_simbol == '-')  
        {
            uint64_t result = subtract(first_number, second_number); //смятаме
            printf("Result in BCD(binary): ");
            if(first_number<second_number) //ако е отрицателно принтим с знак 9 
            {
                print_bcd_in_binary(result,1);
                continue;
            }
            print_bcd_in_binary(result,0); // ако е положително с знак 0
        } 
        else
        {
            printf("[main]: unsuported operation!\n");
        }
        char again;
        printf("Do you want to calculate again? (y/n): "); // промптваме потребителя за последващо калкулиране 
        fflush(stdin);
        scanf("%c", &again);
        getchar();
        if(again == 'n')
        {
            break;
        }
    }

    return 0;
}

uint64_t sum_bcd(uint64_t first_number, uint64_t second_number) 
{
    uint64_t result = 0;
    uint64_t carry = 0;
    const uint64_t mask = 0x0F;
    

    // за дълцината на uint64_t минаваме през всяка 4ка битове 
    for (int i = 0; i < 63; i += 4) 
    {
        uint64_t digit1 = (first_number >> i) & mask; //взимаме първите 4 бита от първото число
        uint64_t digit2 = (second_number >> i) & mask; //взимаме първите 4 бита от второто число
        
        uint64_t sum = digit1 + digit2 + carry; //записваме сумата и добавяме 1 на ум ако има.
        
        //ако получената сума е по голяма от 9, коригираме и записваме 1 на ум.
        if (sum > 9) 
        { 
            carry = 1; //запазваме 1 на ум
            sum -= 10; //тук коригираме sum -= 10 е равно на сумата = сумата - 0110
        } 
        else 
        {
            carry = 0;
        }
        
        result |= sum << i; // Добавяме резултата и пордължажаме със следващите 4 бита
    }
    return result; //връщаме сумата 
}


uint64_t convert_to_bcd(int dec_value)
{
    uint64_t valueToBCD = 0;
    int digit;
    int iteration = 0;
    while (dec_value > 0) // Минаваме през цялото число
    {
        digit = dec_value % 10; // вземаме последната цифра 
        valueToBCD |= (uint64_t)digit << iteration; // и я записваме в резултата
        iteration +=4; // преминавамв към следващите 4 бита 
        dec_value = dec_value / 10;  // премахваме последната цифра
    }
    return valueToBCD;
}

int findIntLength(int num) { // Спомагателна фиункция за намиране на дължината на инта
    int length = 0;

    if (num < 0) 
    { //ако цислото е отрицателно, връщаме 0
        return 0;
    }

    length = floor(log10(abs(num))) + 1; //пресмята дължината на инта (Решение намерено в нета, мисля че дори да те питат може да кажеш че си го намерил)
    return length;
}


int get_msb(uint64_t num) 
{
    //Функция за връщане на позицията на най-старши бит, минаваме през числото докато не намерим 1-ца и връщаме позицията и.
    for (int msb_position = 63; msb_position >= 0; msb_position--) 
    {
        if ((num >> msb_position) & 1) 
        {
            return msb_position;
        }
    }
    return -1;
}

void print_bcd_in_binary(uint64_t number, uint8_t flag) 
{
    int first_one_found = 0;
    if(!flag) // ако цислото е положително принтим 0000 като знак, в резултата винаги първите 4 бита са знак 1001 за '-' и 0000 за '+'
    {
        printf("0000 ");
    }
    //отново минаваме през целия инт докато не намерим 1 
    for (int i = 60; i >= 0; i -= 4) 
    {
        uint64_t bit = (number >> i) & 0xF; // запазваме стойноста на бита
        if (bit > 0 || first_one_found || i == 0) 
        { //проверяваме дали не е 1, или вече не е минат първия бит, или I не е = 0 (за да принтне дори резултат 0)
            printf("%s ", decimal_to_binary_4_bits(bit));
            first_one_found = 1; // сетваме този флаг на 1 за да може след първия намерен бит да принтим всичко
            if ((i % 4 == 0) && (i != 0)) 
            {
                printf(" "); //разделяме за да може да е по четливо 
            }
        }
    }
    printf("\n");
}

const char* decimal_to_binary_4_bits(unsigned int num) 
{
    // спомагателна финкция за да принтираме по-лесно
    static const char* binaryLookup[10] = 
    {
        "0000", "0001", "0010", "0011", "0100",
        "0101", "0110", "0111", "1000", "1001"
    };

    return binaryLookup[num];
}

//Функция за апендване на интове 
int int_append(int to_append, uint16_t append) 
{
    to_append = to_append * 10 + append;
    return to_append;
}
// функция за добавяне на  9 като знак
void add_nine_for_sign(uint64_t *number) 
{
    int number_length = get_msb(*number);
    
    for (int i = 0; i < 64; i += 4) 
    {
        if (i > number_length) 
        {
            *number |= 9ULL << i; // добавяме 1001 преди моментния най-старши бит
            break;
        }
    }
}


// Функция за изваждане 
uint64_t subtract(int first_number, int second_number) 
{
    if (!second_number) 
    {
        return sum_bcd(convert_to_bcd(first_number), convert_to_bcd(second_number)); //хендълваме вход 0 на второ число
    } 
    else if (!first_number) 
    { // Хендълваме вход 0 на първо число
        uint64_t result = convert_to_bcd(second_number); // Конвертираме числото което не е 0
        add_nine_for_sign(&result); // Добавяме му 9, за знак
        return result; //и го връщаме за принт
    } 
    else
    {
        uint8_t result_flag = (first_number > second_number) ? 0 : 1; // ако първото е по голямо от второто връщаме 0, иначе връщаме 1.
        int length = findIntLength(second_number); // Взимаме дължината на инта, за да знаем с можем да правим nine's complement (тук има пример: https://en.wikipedia.org/wiki/Binary-coded_decimal#Subtraction
        second_number = (int)pow(10, length)  - second_number; // намираме съответното число 10 на x където х е length (пример: 432 е с дължина 3, следователно 10^3 е 1000, от него нашето число и получаваме 568) 
        uint64_t second_number_bcd = convert_to_bcd(second_number);// конвертираме 568 в БЦД
        add_nine_for_sign(&second_number_bcd);// и добавяме знак '-'
        
        uint64_t result_bcd = sum_bcd(convert_to_bcd(first_number), second_number_bcd); //извикваме функцията за събиране - (а + (-б)) : По примера по горе, връщаме 9925

        //този фор цикъл преобразува бцд-то в реално число като махаме първото число 9
        int first_one_found = 0;
        int decimal_value = 0;
        for (int i = 60; i >= 0; i -= 4) 
        {
            uint64_t bit = (result_bcd >> i) & 0xF;
            if (bit > 0 || first_one_found || i == 0) // не влизаме в иф-а докато не стигнем първия бит 1-ца....
            { //тук ще влезем за първи път чак когато бита стане 1 или и = 0 
                if (!first_one_found) //след това, само при първото влизане, когато е намерен първия бит но first_one_found сетнат
                {
                    uint64_t mask = ~(0xFULL << i); //замаскираме първата девятка // {в момвнта отговора е 1001 1001 0010 0101 } 
                    result_bcd &= mask;     // 1001 (това се занулява) | 1001 0010 0101
                    first_one_found = 1; // и си сетваме че сме намерили еденица
                    continue; //скипваме следващия стейтмънт
                }
                decimal_value = int_append(decimal_value, bit); //апендваме само следващите числя (1001 0010 0101 || 925)
            }
        }
        //и ако числото е отрицателно и не е 0 по същия начин го вадим от 10^n което в нашия случей е 3 1000 - 925 = 75
        if (result_flag && decimal_value) 
        {
            int tens_complement = pow(10, find_int_lenght(decimal_value)) - decimal_value;
            result_bcd = convert_to_bcd(tens_complement); // обръщаме го в БЦД отново
            add_nine_for_sign(&result_bcd); //добавяме знак защото е отрицателно число 
        }

        return result_bcd;// и връщаме резултат
    }
}


