#ifndef CPU_H
#define CPU_H

#include <stdint.h>

#define NUM_FONT_SPRITES 80
#define SPRITE_WIDTH 8

typedef struct {
    uint8_t memory[4096];
    uint8_t registers[16];
    uint16_t i;
    uint16_t pc;
    uint16_t stack[24];
    uint8_t sp;
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint8_t paused;
    uint8_t speed;
} cpu_t;

const uint8_t SPRITES[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void initCPU(cpu_t *cpu);
void pushStack(cpu_t *cpu, uint16_t);
uint16_t popStack(cpu_t *cpu);
void loadSpritesIntoMemory(cpu_t *cpu);
void loadProgramIntoMemory(cpu_t *cpu, char *filename);
void executeInstruction(cpu_t *cpu, renderer_t *renderer, uint16_t instruction);
void cycle(cpu_t *cpu, renderer_t *renderer);
void updateTimers(cpu_t *cpu);

#endif
