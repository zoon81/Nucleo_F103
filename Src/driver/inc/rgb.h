#ifndef INC_RGB_H
#define INC_RGB_H

#include "stm32f1xx_hal.h"

struct rgb_t{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t index;
};

void rgb_init_start(TIM_HandleTypeDef *htim4);
void rgb_set(TIM_HandleTypeDef *htim4, struct rgb_t *rgb);


#endif