#include <stdio.h>
#include "Calculator_BCD.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

int main()
{

    while(1)
    {
    printf("-------------------------------------------------------------\n");
    int first_number, second_number;
    char operation_simbol;
    printf("Enter your first number: ");
    scanf("%d", &first_number);
    printf("Enter your second number: ");
    scanf("%d", &second_number);
    printf("Enter Operation (+/-): ");
    fflush(stdin);
    scanf("%c", &operation_simbol);
    getchar();
    if(operation_simbol == '+') 
    {
        // printf("[main]: PLUS\n");
        uint64_t result = sum_BCD(convert_to_BCD(first_number),convert_to_BCD(second_number));
        printf("Result in BCD(binary): ");
        print_bcd_in_binary(result);
        printf("\n");
        printf("Result in BCD(dec): %d\n", bcd_to_decimal(result));
    } 
    else if (operation_simbol == '-') 
    {
        uint64_t result = subtract(first_number, second_number);
        printf("Result in BCD(binary): ");
        print_bcd_in_binary(result);
        printf("\n");
        printf("Result in BCD(dec): %d\n", bcd_to_decimal(result));
        // printf("[main]: subtract_BCD returned: %llu\n", result);
        // printf("[main]: MINUS\n");
    } 
    else{
        printf("[main]: unsuported operation!\n");
    }
    char again;
    printf("Do you want to calculate again? (y/n): ");
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

uint64_t sum_BCD(uint64_t first_number, uint64_t second_number) //200 300
{

    uint64_t result = 0;
    uint64_t carry = 0;
    uint64_t mask = 0x0F; // Mask to isolate each BCD digit
    for (int i = 0; i < 64; i += 4) {
        // Extract each digit from both numbers
        uint64_t digit1 = (first_number >> i) & mask;
        uint64_t digit2 = (second_number >> i) & mask;
        
        // Add the digits along with the carry
        uint64_t sum = digit1 + digit2 + carry;
        
        // Check if there's a carry for the next iteration
        if (sum > 9) {
            carry = 1;
            sum -= 10;
        } else {
            carry = 0;
        }
        
        result |= sum << i;
    }
    return result;
}

uint64_t convert_to_BCD(int dec_value)
{
    uint64_t valueToBCD = 0;
    int digit;
    int iteration = 0;
    while (dec_value > 0) 
    {
        digit = dec_value % 10;
        valueToBCD = digitToBinary(digit, valueToBCD, iteration);
        iteration +=4;
        dec_value = dec_value / 10;  
    }
    return valueToBCD;
}

uint64_t digitToBinary(int digit, uint64_t result, int iteration)
{
    result |= digit << iteration;
    return result;
}

void correct_for_subtraction(int *number){
    *number =  (pow(10, findIntLength(*number)) -1) - *number + 1;
}

int findIntLength(int num) {
    // printf("findIntLength: enter");
    int length = 0;

    // If the number is negative, count the negative sign and make it positive
    if (num < 0) {
        length++; // Count the negative sign
        num = -num; // Make the number positive
    }

    // Count digits until the number becomes zero
    do {
        length++;
        num /= 10;
    } while (num != 0);
    // printf("length: %d \n", length);
    
    return length;
}

int getMSB(uint64_t num) {
    int msb_position = 63; // MSB position for a 64-bit unsigned integer
    while (msb_position >= 0) {
        if ((num >> msb_position) & 1) {
            return msb_position;
        }
        msb_position--;
    }
    return -1; // If num is zero
}

void print_bcd_in_binary(uint64_t number)
{
    
    // printf("print_bcd_in_binary: number: %llu\n", number);
    int first_one_found = 0;
    for (int i = 60; i >= 0; i-=4) {
        uint64_t bit = (number >> i & 0xF);
        if (bit > 0 || first_one_found || i == 0) {
            printf("%s ", decimal_to_binary_4_bits(bit));
            first_one_found = 1;
            if ((i % 4 == 0) && (i != 0)) 
            {
                printf(" ");
            }
        }
        
    }
    printf("\n");
}

char* decimal_to_binary_4_bits(unsigned int num) 
{
    // Array to hold the binary representations of numbers 0 to 9
    char* binaryLookup[10] = {
        "0000", // 0
        "0001", // 1
        "0010", // 2
        "0011", // 3
        "0100", // 4
        "0101", // 5
        "0110", // 6
        "0111", // 7
        "1000", // 8
        "1001"  // 9
    };

    // Return the binary representation from the lookup table
    return binaryLookup[num];
}

int int_append(int to_append, uint16_t append)
{
    to_append = to_append * 10 + append;
    // printf("[int_append]: %d\n", to_append);
    return to_append;
}

int bcd_to_decimal(uint64_t bcd) {
    int num = 0, sum = 0, mul = 1, rev = 0, check = 0, check0 = 0;
    
    // Iterating through the bits backwards
    for (int i = 63; i >= 0; i--) {
        // Forming the equivalent digit (0 to 9) from the group of 4
        sum += ((bcd >> i) & 1) * mul;
        mul *= 2;
        check++;
        
        // Reinitialize all variables and compute the number
        if (check == 4 || i == 0) {
            if (sum == 0 && check0 == 0) {
                num = 1;
                check0 = 1;
            } else {
                // Update the answer
                num = num * 10 + sum;
            }
            
            check = 0;
            sum = 0;
            mul = 1;
        }
    }
   // Reverse the number formed
    while (num > 0) {
        rev = rev * 10 + (num % 10);
        num /= 10;
    }
    
    if (check0 == 1)
        return rev - 1;
    
    return rev;
}

void add_nine_for_sign(uint64_t *number)
{
    int number_lenght = getMSB(*number);
        for (int i = 0; i < 64; i += 4)
        {
            if (i > number_lenght)
            {
                // printf("[subtract_BCD]: number before: %llu\n",*number);
                *number |= 9 << i;
                // printf("[subtract_BCD]: number after before: %llu\n",*number);
                break;
            }
        }
}


uint64_t subtract(int first_number, int second_number)
{   
    if(!second_number){
        return sum_BCD(convert_to_BCD(first_number), convert_to_BCD(second_number));
    }
    else if(!first_number)
    {
        uint64_t result = convert_to_BCD(first_number);
        add_nine_for_sign(&result);
        return result;
    }
    else {
        uint8_t result_flag;
        if(first_number>second_number)
        {
            result_flag = 0;
        }
        else 
        {
            result_flag = 1;
        }
        uint64_t second_number_bcd = 0;
        // printf("[sum_BCD]: second_number: %d\n", second_number);
        correct_for_subtraction(&second_number); //568
        // printf("second_number: %d\n",second_number);
        second_number_bcd = convert_to_BCD(second_number);
        add_nine_for_sign(&second_number_bcd);
        uint64_t result_bcd = sum_BCD(convert_to_BCD(first_number),second_number_bcd);
        // int tmp = bcd_to_decimal(result_bcd);
        // printf("[subtract] result: %d\n", tmp);

        int first_one_found = 0;
        int decValue = 0;
        for (int i = 60; i >= 0; i-=4) {
            uint64_t bit = (result_bcd >> i & 0xF);
            if (bit > 0 || first_one_found ||i == 0) {
                if (!first_one_found)
                {
                    // printf("[subtract] SHOULD SEE ONLY ONCE\n");
                    // decValue = bit;
                    // printf("[subtract] bit: %llu ", bit);
                    uint64_t mask = ~(0xFULL << i);
                    result_bcd = result_bcd & mask;
                    // printf("[subtract] result:%llu\n", result);
                    first_one_found = 1;
                    continue;
                }            
                decValue = int_append(decValue, bit);
                // printf("[subtract] bit: %llu ", bit);
            }
            
        }
        if (result_flag && decValue)
        {
            // int tmp= pow(10, findIntLength(decValue));
            int tens_complement= ((pow(10, findIntLength(decValue))) - decValue);
            // printf("tmp %d tens_complement: %d\n", tmp, tens_complement);
            result_bcd = convert_to_BCD(tens_complement);
            // printf("tmp %d result_bcd: %llu\n", tmp, result_bcd);
            add_nine_for_sign(&result_bcd);
            // printf("tmp %d result_bcd: %llu\n", tmp, result_bcd);
            // print_bcd_in_binary(result_bcd);
        }
        
        // printf("[subtract] decValue %d\n", decValue);

        return result_bcd;
    }
}