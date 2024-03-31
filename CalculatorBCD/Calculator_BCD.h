#ifndef CALCULATOR_BCD_H
#define CALCULATOR_BCD_H
#include <stdio.h>
#include <stdint.h>

uint64_t convert_to_BCD(int dec_value);
uint64_t sum_BCD(int first_number, int second_number);
uint64_t prep_for_subtract(int number);
uint64_t digitToBinary(int digit, uint64_t result, int iteration);
void correct_for_subtraction(int *number);
int findIntLength(int num);
int getMSB(uint64_t num);
void print_bcd_in_binary(uint64_t number);

#endif /* CONVERTER_H */