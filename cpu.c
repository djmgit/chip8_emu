#include <stdio.h>
#include <errno.h>
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

void loadProgramIntoMemory(cpu_t *cpu, char *filename) {
    FILE *fp;
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Failed to openfile: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    int c_byte;
    uint16_t loc = 0;
    while ((c_byte = getc(fp)) != EOF) {
        cpu->memory[0x200 + loc] = c_byte;
        loc++;
    }
    fclose(filename);
    prinft("ROM successfully loaded into memory");
}

void cycle(cpu_t *cpu) {
    for (size_t i = 0; i < cpu->speed; i++) {
        if (!cpu->paused) {
            uint16_t instruction = (cpu->memory[cpu->pc] << 8 | cpu->memory[cpu->pc + 1]);
            executeInstruction(cpu, instruction);
        }
    }
}

void executeInstruction(cpu_t *cpu, uint16_t instruction) {

}
