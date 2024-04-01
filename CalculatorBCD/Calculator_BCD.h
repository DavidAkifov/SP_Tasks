#ifndef CALCULATOR_BCD_H
#define CALCULATOR_BCD_H
#include <stdio.h>
#include <stdint.h>

uint64_t convert_to_bcd(int dec_value);
uint64_t sum_bcd(uint64_t first_number, uint64_t second_number);
uint64_t prep_for_subtract(int number);
uint64_t digitToBinary(int digit, uint64_t result, int iteration);
void correct_for_subtraction(int *number);
int findIntLength(int num);
int get_msb(uint64_t num);
void print_bcd_in_binary(uint64_t number);
const char* decimal_to_binary_4_bits(unsigned int num);
int int_append(int to_append, uint16_t append);
int bcd_to_decimal(uint64_t bcd);
void add_nine_for_sign(uint64_t *number);
uint64_t subtract(int first_number, int second_number);

#endif /* CONVERTER_H */