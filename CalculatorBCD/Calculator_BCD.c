#include <stdio.h>
#include "Calculator_BCD.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
int numberOfDigits(unsigned long long n) 
{
    if (n == 0)
        return 0;
    return floor( log10( abs( n ) ) ) + 1;
}

int main()
{
    int first_number, second_number;
    char operation_simbol;
    printf("Enter your first number: ");
    scanf("%d", &first_number);
    uint64_t first_number_to_BCD = convert_to_BCD(first_number);
    printf("Enter your second number: ");
    scanf("%d", &second_number);
    uint64_t second_number_to_BCD = convert_to_BCD(second_number);
    printf("[main]: convertToBCD %llu  \n",first_number_to_BCD);
    printf("[main]: convertToBCD %llu  \n",second_number_to_BCD);
    printf("Enter Operation (+/-): ");
    fflush(stdin);
    scanf("%c", &operation_simbol);
    getchar();
    if(operation_simbol == '+') 
    {
        printf("[main]: PLUS\n");
        printf("%llu", sum_BCD(first_number_to_BCD, second_number_to_BCD));
    } 
    else if (operation_simbol == '-') 
    {
        //second_number_to_BCD |= 9;
        int tmp1 = correct_for_subtraction(second_number);
        printf("[main]: tmp1: %d\n", tmp1);
        second_number_to_BCD = convert_to_BCD(tmp1);
        uint64_t result = subtract_BCD(first_number_to_BCD, second_number_to_BCD);
        


        printf("second_number_to_BCD: %llu ",second_number_to_BCD);
        printf("[main]: getMSB: %d\n",getMSB(second_number_to_BCD));
        printf("[main]: second_number_to_BCD: %llu\n",second_number_to_BCD);
        printf("[main]: subtract_BCD returned: %llu\n", result);
        printf("[main]: MINUS\n");
    } 
    else{
        printf("[main]: unsuported operation!\n");
    }
    return 0;
}

uint64_t sum_BCD(uint64_t first_num, uint64_t second_num)
{
    uint64_t result = 0;
    uint64_t carry = 0;
    uint64_t mask = 0x0F; // Mask to isolate each BCD digit
    for (int i = 0; i < 16; i += 4) {
        // Extract each digit from both numbers
        uint64_t digit1 = (first_num >> i) & mask;
        uint64_t digit2 = (second_num >> i) & mask;
        
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

uint64_t subtract_BCD(uint64_t first_num, uint64_t second_num)
{
    printf("subtract_BCD: first_num: %llu second_num: %llu \n", first_num, second_num);
    int number_lenght = getMSB(second_num);
        for (int i = 0; i < 16; i += 4)
        {
            if (i > number_lenght)
            {
                second_num |= 9 << i;
                break;
            }
        }
    uint64_t result = sum_BCD(first_num,second_num);

    return result;
}

uint64_t digitToBinary(int digit, uint64_t result, int iteration)
{
    result |= digit << iteration;
    printf("result: %llu  iteration %d \n", result, iteration);
    return result;
}

int correct_for_subtraction(int number){
    printf("correct_for_subtraction: number: %d \n", number);
    int corected_number =  (pow(10, findIntLength(number)) -1) - number + 1;
    printf("corrected number in dec: %d \n", corected_number);
    return corected_number;
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

void perform_tens_complement(uint64_t *number)
{

}