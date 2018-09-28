#ifndef INC_KEYBOARD_MATRIX
#define INC_KEYBOARD_MATRIX

#include <inttypes.h>

// Keyboard connected on PA lower part
// PA0 - PA3 ---- ROWS
// PA4 - PA6 ---- COLLUMS

#define KBM_GPIO GPIOA
#define KBM_ROW1 GPIO_PIN_3
#define KBM_ROW2 GPIO_PIN_4
#define KBM_ROW3 GPIO_PIN_5
#define KBM_ROW4 GPIO_PIN_6
#define KBM_COLLUM1 GPIO_PIN_0
#define KBM_COLLUM2 GPIO_PIN_1
#define KBM_COLLUM3 GPIO_PIN_2

#define KBM_KEY_INDEX_ASTERISK 10
#define KBM_KEY_ASCII_ASTERISK 0x2A
#define KBM_KEY_INDEX_HASHMARK 11
#define KBM_KEY_ASCII_HASHMARK 0x23

void kbm_init();
char kbm_getAsciiValue();
uint8_t kbm_getValue();


#endif