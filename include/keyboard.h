#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

typedef struct {
    uint8_t keymap[87];
    uint8_t keypressed[17];
};

#endif
