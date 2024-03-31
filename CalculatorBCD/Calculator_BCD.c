#include <stdio.h>
#include "Calculator_BCD.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

int main()
{
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
        printf("[main]: PLUS\n");
        printf("%llu", sum_BCD(first_number, second_number));
    } 
    else if (operation_simbol == '-') 
    {
        printf("[main]: subtract_BCD returned: %llu\n", sum_BCD(first_number, -second_number));
        printf("[main]: MINUS\n");
    } 
    else{
        printf("[main]: unsuported operation!\n");
    }
    return 0;
}

uint64_t sum_BCD(int first_number, int second_number)
{
    uint64_t first_number_to_BCD = 0;
    uint64_t second_number_to_BCD = 0;
    if(second_number < 0)
    {
        second_number_to_BCD = prep_for_subtract(second_number);
    }
    else
    {
        uint64_t second_number_to_BCD = convert_to_BCD(second_number);
    }
    first_number_to_BCD = convert_to_BCD(first_number);
    uint64_t result = 0;
    uint64_t carry = 0;
    uint64_t mask = 0x0F; // Mask to isolate each BCD digit
    for (int i = 0; i < 64; i += 4) {
        // Extract each digit from both numbers
        uint64_t digit1 = (first_number_to_BCD >> i) & mask;
        uint64_t digit2 = (second_number_to_BCD >> i) & mask;
        
        // Add the digits along with the carry
        uint64_t sum = digit1 + digit2 + carry;
        
        // Check if there's a carry for the next iteration
        if (sum > 9) {
            carry = 1;
            sum -= 10;
        } else {
            carry = 0;
        }
        
        // Place the sum digit in the result
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
        printf("[convert_to_BCD]: digit: %d \n", digit);
        valueToBCD = digitToBinary(digit, valueToBCD, iteration);
        iteration +=4;
        dec_value = dec_value / 10;  
    }
    return valueToBCD;
}

uint64_t prep_for_subtract(int number)
{
    uint64_t result = 0;
    number = -number;
    printf("[sum_BCD]: second_number: %d\n", number);
    correct_for_subtraction(&number);
    result = convert_to_BCD(number);
    printf("[sum_BCD]: result: %llu\n", result);
    int number_lenght = getMSB(result);
        for (int i = 0; i < 64; i += 4)
        {
            if (i > number_lenght)
            {
                result |= 9 << i;
                printf("[subtract_BCD]: result: %llu\n",result);
                break;
            }
        }
    return result;
}

uint64_t digitToBinary(int digit, uint64_t result, int iteration)
{
    result |= digit << iteration;
    printf("result: %llu  iteration %d \n", result, iteration);
    return result;
}

void correct_for_subtraction(int *number){
    printf("correct_for_subtraction: number: %d \n", *number);
    *number =  (pow(10, findIntLength(*number)) -1) - *number + 1;
    printf("corrected number in dec: %d \n", *number);
}

int findIntLength(int num) {
    printf("findIntLength: enter");
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
    printf("length: %d \n", length);
    
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
    int first_one_found = 0;
    for (int i = 63; i >= 0; i--) {
        uint64_t bit = (number >> i) & 1;
        if (bit == 1 || first_one_found || i == 0) {
            printf("%llu", bit);
            first_one_found = 1;
            if ((i % 4 == 0) && (i != 0)) 
            {
                printf(" ");
            }
        }
        
    }
    printf("\n");
}