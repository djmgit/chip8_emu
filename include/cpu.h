#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "keyboard.h"
#include "renderer.h"

#define NUM_FONT_SPRITES 80
#define SPRITE_WIDTH 8
#define MEMORY_SIZE 4096
#define NUM_REGISTERS 16
#define STACK_SIZE 24

typedef struct keyboard_t keyboard_t;
typedef struct cpu_t {
    uint8_t memory[MEMORY_SIZE];
    uint8_t registers[NUM_REGISTERS];
    uint16_t i;
    uint16_t pc;
    uint16_t stack[STACK_SIZE];
    uint8_t sp;
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint8_t paused;
    uint8_t speed;
    uint8_t key_register;
} cpu_t;

void initCPU(cpu_t *cpu);
void pushStack(cpu_t *cpu, uint16_t);
uint16_t popStack(cpu_t *cpu);
void loadSpritesIntoMemory(cpu_t *cpu);
void loadProgramIntoMemory(cpu_t *cpu, char *filename);
void executeInstruction(cpu_t *cpu, renderer_t *renderer, keyboard_t *keyboard, uint16_t instruction);
void cycle(cpu_t *cpu, keyboard_t *keyboard, renderer_t *renderer);
void updateTimers(cpu_t *cpu);

#endif
