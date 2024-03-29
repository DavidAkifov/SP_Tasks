#include <stdio.h>
#include "Calculator_BCD.h"
#include <string.h>
#include <stdlib.h>

int main()
{
    int first_number, second_number;
    char operation_simbol;
    printf("Enter your first number: ");
    scanf("%d", &first_number);
    unsigned int firstNumberToBCD = convertToBCD(first_number);
    printf("Enter your second number: ");
    scanf("%d", &second_number);
    unsigned int second_numberToBCD = convertToBCD(second_number);
    printf("convertToBCD %d  \n",firstNumberToBCD);
    printf("convertToBCD %d  \n",second_numberToBCD);
    printf("Enter Operation (+/-): ");
    fflush(stdin);
    scanf("%c", &operation_simbol);
    getchar();
    if(operation_simbol == '+') 
    {
        printf("PLUS\n");
    } 
    else if (operation_simbol == '-') 
    {
        printf("MINUS\n");
    } 
    else{
        printf("unsuported operation!\n");
    }
    return 0;
}

unsigned int convertToBCD(int dec_value)
{
    unsigned int valueToBCD = 0;
    int digit;
    int iteration = 0;
    while (dec_value > 0) 
    {
        digit = dec_value % 10;
        printf("digit: %d \n", digit);
        valueToBCD = digitToBinary(digit, valueToBCD, iteration);
        iteration +=4;
        dec_value = dec_value / 10;  
    }
    return valueToBCD;
}


unsigned int digitToBinary(int digit, unsigned int result, int iteration)
{
    unsigned int binary = 0;
    result |= digit << iteration;
    printf("result: %d  iteration %d \n", result, iteration);
    return result;
}
