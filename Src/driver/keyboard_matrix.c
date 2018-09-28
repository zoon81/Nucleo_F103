#include "keyboard_matrix.h"
#include "stm32f1xx_hal.h"

void kbm_init(){
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef gpioconf;
    gpioconf.Pin = KBM_ROW1 | KBM_ROW2 | KBM_ROW3 | KBM_ROW4;
    gpioconf.Mode = GPIO_MODE_OUTPUT_PP;
    gpioconf.Pull = GPIO_NOPULL;
    gpioconf.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(KBM_GPIO, &gpioconf);

    gpioconf.Pin = KBM_COLLUM1 | KBM_COLLUM2 | KBM_COLLUM3;
    gpioconf.Mode = GPIO_MODE_INPUT;
    gpioconf.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(KBM_GPIO, &gpioconf);
}

void kbm_scan(uint8_t *scancodes){
    // Clear pins
    KBM_GPIO -> BRR = KBM_ROW1 | KBM_ROW2 | KBM_ROW3 | KBM_ROW4;
    // Set col1 high
    KBM_GPIO -> BSRR = KBM_ROW1;
    // Read GPIOA value
    scancodes[0] = KBM_GPIO -> IDR & 0x7F;

   // Clear pins
    KBM_GPIO -> BRR = KBM_ROW1 | KBM_ROW2 | KBM_ROW3 | KBM_ROW4;
    // Set col1 high
    KBM_GPIO -> BSRR = KBM_ROW2;
    // Read GPIOA value
    scancodes[1] = KBM_GPIO -> IDR & 0x7F;

    // Clear pins
    KBM_GPIO -> BRR = KBM_ROW1 | KBM_ROW2 | KBM_ROW3 | KBM_ROW4;
    // Set col1 high
    KBM_GPIO -> BSRR = KBM_ROW3;
    // Read GPIOA value
    scancodes[2] = KBM_GPIO -> IDR & 0x7F;

    // Clear pins
    KBM_GPIO -> BRR = KBM_ROW1 | KBM_ROW2 | KBM_ROW3 | KBM_ROW4;
    // Set col1 high
    KBM_GPIO -> BSRR = KBM_ROW4;
    // Read GPIOA value
    scancodes[3] = KBM_GPIO -> IDR & 0x7F;

    // Clear pins
    KBM_GPIO -> BRR = KBM_ROW1 | KBM_ROW2 | KBM_ROW3 | KBM_ROW4;
}
// Return the pressed key 
uint8_t kbm_getValue(){
    uint8_t scan_lookup[] = {0x45, 0x0E, 0x0D, 0x0B, 0x16, 0x15, 0x13, 0x26, 0x25, 0x23, 0x46, 0x43}; // 0 1 2 3 4 5 6 7 8 9 * #
    uint8_t scancodes[4];
    uint8_t key;
    kbm_scan(&scancodes[0]);
    uint8_t validscancode = 0xFF;
    for(uint8_t i = 0; i < 4; i++){
        if((scancodes[i] & 0x07) != 0x07){
            validscancode = scancodes[i];
            i = 4;                          //stop looping
        }
    }
    if(validscancode == 0xFF){
        return validscancode;
    }

    //get key by scancode
    for(uint8_t i = 0; i < 12; i++){
        if(scan_lookup[i] == validscancode){
            key = i;
            i = 12;                          //stop looping
        }
    }
    return key;
}

// Return the pressed key in ASCII code
char kbm_getAsciiValue(){
    uint8_t key = kbm_getValue();
    switch (key){
        case 10:    return KBM_KEY_ASCII_ASTERISK;
        case 11:    return KBM_KEY_ASCII_HASHMARK;
        case 0xFF:  return 0;
        default:    return key + 48;
    }
}