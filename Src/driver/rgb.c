#include "rgb.h"

void rgb_init_start(TIM_HandleTypeDef *htim4){
    __HAL_TIM_SET_COMPARE(htim4, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(htim4, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(htim4, TIM_CHANNEL_3, 0);
    HAL_TIM_PWM_Start(htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(htim4, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(htim4, TIM_CHANNEL_3);
    
}

void rgb_set(TIM_HandleTypeDef *htim4, struct rgb_t *rgb){
    __HAL_TIM_SET_COMPARE(htim4, TIM_CHANNEL_1, rgb->red);
    __HAL_TIM_SET_COMPARE(htim4, TIM_CHANNEL_2, rgb->green);
    __HAL_TIM_SET_COMPARE(htim4, TIM_CHANNEL_3, rgb->blue);
}