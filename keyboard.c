#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"
#include "cpu.h"
#include "keyboard.h"

void initKeyBoard(keyboard_t *keyboard) {
    keyboard->keymap[49] = 0x1; // 1
    keyboard->keymap[50] = 0x2; // 2
    keyboard->keymap[51] = 0x3; // 3
    keyboard->keymap[52] = 0xc; // 4
    keyboard->keymap[81] = 0x4; // Q
    keyboard->keymap[87] = 0x5; // W
    keyboard->keymap[69] = 0x6; // E
    keyboard->keymap[82] = 0xD; // R
    keyboard->keymap[65] = 0x7; // A
    keyboard->keymap[83] = 0x8; // S
    keyboard->keymap[68] = 0x9; // D
    keyboard->keymap[70] = 0xE; // F
    keyboard->keymap[90] = 0xA; // Z
    keyboard->keymap[88] = 0x0; // X
    keyboard->keymap[67] = 0xB; // C
    keyboard->keymap[86] = 0xF; // V

    keyboard->onNextKeyPress = NULL;
    uint8_t keyCount = 0;
    for (size_t i = 0 ; i < KEY_ARRAY_MAX; i++) {
        if (keyboard->keymap[i] != 0) {
            keyboard->validKeys[keyCount++] = i;
        }
    }
}

int isKeyValid(keyboard_t *keyboard, uint8_t keyCode) {
    if (keyCode >= KEY_ARRAY_MAX || keyboard->keymap[keyCode] == 0) {
        return 0;
    }
    return 1;
}

void onKeyDown(keyboard_t *keyboard, cpu_t* cpu, uint8_t keyCode) {
    if (!isKeyValid(keyboard, keyCode)) {
        return;
    }
    uint8_t key = keyboard->keymap[keyCode];
    keyboard->keyPressed[key] = 1;

    if (keyboard->onNextKeyPress != NULL) {
        keyboard->onNextKeyPress(cpu, key);
        keyboard->onNextKeyPress = NULL;
    }
}

void onKeyUp(keyboard_t *keyboard, uint8_t keyCode) {
    if (!isKeyValid(keyboard, keyCode)) {
        return;
    }
    uint8_t key = keyboard->keymap[keyCode];
    keyboard->keyPressed[key] = 0;
}

int isKeyPressed(keyboard_t *keyboard, uint8_t key) {
    return keyboard->keyPressed[key];
}

void processKey(keyboard_t *keyboard, cpu_t *cpu) {
    for (size_t i = 0; i < NUM_VALID_KEYS; i++) {
        if (IsKeyDown(keyboard->validKeys[i])) {
            onKeyDown(keyboard, cpu, keyboard->validKeys[i]);
        } else {
            onKeyUp(keyboard, keyboard->validKeys[i]);
        }
    }
}

void onNextKeyPress(cpu_t *cpu, uint8_t key) {
    cpu->registers[cpu->key_register] = key;
    cpu->paused = 0;
}
