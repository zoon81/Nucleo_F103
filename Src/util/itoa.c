#include "itoa.h"

void uint8_to_str(char *buffer, uint8_t number)
{
	uint8_t data;
	if (number > 99)
	{
		data = number / 100;
		number %= 100;
		*buffer++ = data + 48;
		data = number / 10;
		number %= 10;
		*buffer++ = data + 48;
		*buffer++ = number + 48;
		*buffer = 0; //string termination
	}
	else if (number > 9)
	{
		data = number / 10;
		number %= 10;
		*buffer++ = data + 48;
		*buffer++ = number + 48;
		*buffer = 0; //string termination
	}
	else if (number >= 0)
	{
		*buffer++ = number + 48;
		*buffer = 0; //string termination
	}
}
void uint16_to_str(char *buffer, uint32_t number)
{
	//this is a realy big number?
	if (number > 255)
	{
		//how long is the number?
		uint32_t divider = 100;
		uint8_t number_len = 2;
		while (number / divider)
		{
			number_len++;
			divider *= 10;
		}
		//rolling back the divider
		divider /= 10;
		//tearing down
		while(number_len--){
			*buffer++ = (number / divider) + 48;
			number %= divider;
			divider /= 10; 
		}
		*buffer = 0; 		//terminate string
	}
	else
	{
		return uint8_to_str(buffer, number);
	}
}
void int16_to_str(char *buffer, int16_t number){
	if(number & 0x8000){
		buffer[0] = '-';
		number--;
		number = ~number;
		number &= 0x7FFF;
		uint16_to_str(&buffer[1], number);
	} else{
		uint16_to_str(buffer, number);
	}
}

uint16_t atoi_(char *str){
	uint8_t str_len = strlen_(str);
	uint16_t multiplier = pwr(10, str_len - 1);
	uint16_t number = 0;
	do{
		number += multiplier * (*str++ - 48);
		multiplier /= 10;
	}while(multiplier);
	return number;
}

uint8_t strlen_(char *str){
	uint8_t i = 0;
	while(*str++){
		i++;
	}
	return i;
}

uint16_t pwr(uint8_t base, uint8_t exponent){
	uint16_t result = 1;
	while(exponent--){
		result *= base;
	}
	return result;
}