#ifndef CALCULATOR_BCD_H
#define CALCULATOR_BCD_H
#include <stdio.h>
#include <stdint.h>

uint64_t convert_to_BCD(int dec_value);
uint64_t sum_BCD(uint64_t first_num, uint64_t second_num);
uint64_t subtract_BCD(uint64_t first_num, uint64_t second_num);
uint64_t digitToBinary(int digit, uint64_t result, int iteration);
int correct_for_subtraction(int number);
int findIntLength(int num);
int getMSB(uint64_t num);

#endif /* CONVERTER_H */