#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

typedef struct {
    uint8_t keymap[87];
    uint8_t keypressed[17];
    void (* onNextKeyPress) (keyboard_t* keyboard, uint8_t);
} keyboard_t;

void initKeyBoard(keyboard_t *keyboard);
void onNextKeyPress(keyboard_t *keyboard, uint8_t);

#endif
