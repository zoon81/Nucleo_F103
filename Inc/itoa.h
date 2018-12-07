#ifndef INC_ITOA_H
#define INC_ITOA_H
#include "stdint.h"

void uint8_to_str(char *buffer, uint8_t number);
void uint16_to_str(char *buffer, uint32_t number);
void int16_to_str(char *buffer, int16_t number);
uint16_t atoi_(char *str);
uint8_t strlen_(char *str);
uint16_t pwr(uint8_t base, uint8_t exponent);

#endif