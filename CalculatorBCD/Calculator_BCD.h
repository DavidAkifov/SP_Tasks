#ifndef CALCULATOR_BCD_H
#define CALCULATOR_BCD_H
#include <stdio.h>

unsigned int convertToBCD(int dec_value);
unsigned int appendIntegers(unsigned int num1, unsigned int num2);
unsigned int digitToBinary(int digit, unsigned int result, int iteration);
unsigned int binaryStringToInt(const char* binaryStr);

#endif /* CONVERTER_H */