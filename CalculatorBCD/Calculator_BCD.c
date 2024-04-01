#include <stdio.h>
#include "Calculator_BCD.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

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
        uint64_t result = sum_bcd(convert_to_bcd(first_number),convert_to_bcd(second_number));  
        printf("Result in BCD(binary): ");
        print_bcd_in_binary(result, 0); 
        printf("\n");
    } 
    else if (operation_simbol == '-')  
    {
        uint64_t result = subtract(first_number, second_number);  
        printf("Result in BCD(binary): ");
        if(first_number<second_number)  
        {
            print_bcd_in_binary(result,1);
        }
        else
            print_bcd_in_binary(result,0);  
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

uint64_t sum_bcd(uint64_t first_number, uint64_t second_number) {
    uint64_t result = 0;
    uint64_t carry = 0;
    const uint64_t mask = 0x0F;
    

     
    for (int i = 0; i < 63; i += 4) {
        uint64_t digit1 = (first_number >> i) & mask;  
        uint64_t digit2 = (second_number >> i) & mask;  
        
        uint64_t sum = digit1 + digit2 + carry;  
        
         
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


uint64_t convert_to_bcd(int dec_value)
{
    uint64_t valueToBCD = 0;
    int digit;
    int iteration = 0;
    while (dec_value > 0)  
    {
        digit = dec_value % 10;  
        valueToBCD |= (uint64_t)digit << iteration;  
        iteration +=4;  
        dec_value = dec_value / 10;   
    }
    return valueToBCD;
}

int findIntLength(int num) {  
    int length = 0;

    if (num < 0) {  
        return 0;
    }

    length = floor(log10(abs(num))) + 1;  
    return length;
}


int get_msb(uint64_t num) {
     
    for (int msb_position = 63; msb_position >= 0; msb_position--) {
        if ((num >> msb_position) & 1) {
            return msb_position;
        }
    }
    return -1;
}

void print_bcd_in_binary(uint64_t number, uint8_t flag) 
{
    int first_one_found = 0;
    if(!flag)  
    {
        printf("0000 ");
    }
     
    for (int i = 60; i >= 0; i -= 4) {
        uint64_t bit = (number >> i) & 0xF;  
        if (bit > 0 || first_one_found || i == 0) {  
            printf("%s ", decimal_to_binary_4_bits(bit));
            first_one_found = 1;  
            if ((i % 4 == 0) && (i != 0)) {
                printf(" ");  
            }
        }
    }
    printf("\n");
}

const char* decimal_to_binary_4_bits(unsigned int num) {
     
    static const char* binaryLookup[10] = {
        "0000", "0001", "0010", "0011", "0100",
        "0101", "0110", "0111", "1000", "1001"
    };

    return binaryLookup[num];
}

 
int int_append(int to_append, uint16_t append) {
    to_append = to_append * 10 + append;
    return to_append;
}
 
void add_nine_for_sign(uint64_t *number) {
    int number_length = get_msb(*number);
    
    for (int i = 0; i < 64; i += 4) {
        if (i > number_length) {
            *number |= 9ULL << i;  
            break;
        }
    }
}


 
uint64_t subtract(int first_number, int second_number) {
    if (!second_number) {
        return sum_bcd(convert_to_bcd(first_number), convert_to_bcd(second_number));  
    } else if (!first_number) {  
        uint64_t result = convert_to_bcd(second_number);  
        add_nine_for_sign(&result);  
        return result;  
    } else {
        uint8_t result_flag = (first_number > second_number) ? 0 : 1;  
        int length = findIntLength(second_number);  
        second_number = (int)pow(10, length)  - second_number;  
        uint64_t second_number_bcd = convert_to_bcd(second_number); 
        add_nine_for_sign(&second_number_bcd); 
        
        uint64_t result_bcd = sum_bcd(convert_to_bcd(first_number), second_number_bcd);  

         
        int first_one_found = 0;
        int decimal_value = 0;
        for (int i = 60; i >= 0; i -= 4) {
            uint64_t bit = (result_bcd >> i) & 0xF;
            if (bit > 0 || first_one_found || i == 0) {
                if (!first_one_found) {
                    uint64_t mask = ~(0xFULL << i);
                    result_bcd &= mask;
                    first_one_found = 1;
                    continue;
                }            
                decimal_value = int_append(decimal_value, bit);
            }
        }
         
        if (result_flag && decimal_value) {
            int tens_complement = pow(10, findIntLength(decimal_value)) - decimal_value;
            result_bcd = convert_to_bcd(tens_complement);  
            add_nine_for_sign(&result_bcd);  
        }

        return result_bcd; 
    }
}


