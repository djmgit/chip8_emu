#include <stdio.h>
#include "cpu.h"
#include "renderer.h"
#include "keyboard.h"

void initCPU(cpu_t *cpu) {
    cpu->i = 0;
    cpu->pc = 0x200;
    cpu->sp = -1;
    cpu->paused = 0;
    cpu->speed = 10;

    loadSpritesIntoMemory(cpu);
}

void pushStack(cpu_t *cpu, uint16_t val) {
    cpu->stack[++cpu->sp] = val;
}

uint16_t popStack(cpu_t *cpu) {
    return cpu->stack[cpu->sp--];
}

void loadSpritesIntoMemory(cpu_t *cpu) {
    for (size_t i = 0; i < NUM_FONT_SPRITES; i++) {
        cpu->memory[i] = SPRITES[i];
    }
}
