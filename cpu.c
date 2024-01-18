#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include "cpu.h"
#include "renderer.h"
#include "keyboard.h"

void initCPU(cpu_t *cpu) {
    cpu->i = 0;
    cpu->pc = 0x200;
    cpu->sp = -1;
    cpu->paused = 0;
    cpu->speed = 10;
    srand(time(0));

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

void updateTimers(cpu_t *cpu) {
    cpu->delayTimer = cpu->delayTimer > 0 ? cpu->delayTimer - 1 : cpu->delayTimer;
    cpu->soundTimer = cpu->soundTimer > 0 ? cpu->soundTimer - 1 : cpu->soundTimer;
}

void cycle(cpu_t *cpu, keyboard_t *keyboard, renderer_t *renderer) {
    for (size_t i = 0; i < cpu->speed; i++) {
        if (!cpu->paused) {
            uint16_t instruction = (cpu->memory[cpu->pc] << 8 | cpu->memory[cpu->pc + 1]);
            executeInstruction(cpu, renderer, keyboard, instruction);
        }
    }

    if (!cpu->paused) {
        updateTimers(cpu);
    }

    render(renderer);
}

void executeInstruction(cpu_t *cpu, renderer_t *renderer, keyboard_t *keyboard, uint16_t instruction) {

    cpu->pc += 2;

    uint8_t x = (instruction & 0x0f00) >> 8;
    uint8_t y = (instruction & 0x00f0) >> 4;

    switch(instruction & 0xf000) {
        case 0x0000:
            switch(instruction) {
                case 0x00e0:
                    clear(renderer);
                    break;
                case 0x00ee:
                    cpu->pc = popStack(cpu);
                    break;
            }
            break;
        
        case 0x1000:
            cpu->pc = (instruction & 0xfff);
            break;
        
        case 0x2000:
            pushStack(cpu, cpu->pc);
            cpu->pc = (instruction & 0xfff);
            break;
        
        case 0x3000:
            if (cpu->registers[x] == (instruction & 0xff)) {
                cpu->pc += 2;
            }
            break;
        
        case 0x4000:
            if (cpu->registers[x] != (instruction & 0xff)) {
                cpu->pc += 2;
            }
            break;
        
        case 0x5000:
            if (cpu->registers[x] == cpu->registers[y]) {
                cpu->pc += 2;
            }
            break;
        
        case 0x6000:
            cpu->registers[x] = (instruction & 0xff);
            break;
        
        case 0x7000:
            cpu->registers[x] += (instruction & 0xff);
            break;
        
        case 0x8000:
            switch(instruction & 0xf){
                case 0x0:
                    cpu->registers[x] = cpu->registers[y];
                    break;
                
                case 0x1:
                    cpu->registers[x] |= cpu->registers[y];
                    break;
                
                case 0x2:
                    cpu->registers[x] &= cpu->registers[y];
                    break;
                
                case 0x3:
                    cpu->registers[x] ^= cpu->registers[y];
                    break;
                
                case 0x4:
                    uint16_t sum = cpu->registers[x] + cpu->registers[y];
                    cpu->registers[0xf] = 0;
                    if (sum > 0xff) {
                        cpu->registers[0xf] = 1;
                    }
                    cpu->registers[x] = sum;
                    break;
                
                case 0x5:
                    cpu->registers[0xf] = 1;
                    
                    if (cpu->registers[x] <= cpu->registers[y]) {
                        cpu->registers[0xf] = 0;
                    }

                    cpu->registers[x] -= cpu->registers[y];
                    break;
                
                case 0x6:
                    cpu->registers[0xf] = (cpu->registers[x] & 0x1);
                    cpu->registers[x] >>= 1;
                    break;
                
                case 0x7:
                    cpu->registers[0xf] = 0;
                    
                    if (cpu->registers[y] > cpu->registers[x]) {
                        cpu->registers[0xf] = 1;
                    }

                    cpu->registers[x] = cpu->registers[y] - cpu->registers[x];
                    break;
                
                case 0xe:
                    cpu->registers[0xf] = (cpu->registers[x] & 0x80);
                    cpu->registers[x] <<= 1;
                    break;
            break;
        }

        case 0x9000:
            if (cpu->registers[x] != cpu->registers[y]) {
                cpu->pc += 2;
            }
            break;
        
        case 0xa000:
            cpu->i = (instruction & 0xfff);
            break;
        
        case 0xb000:
            cpu->pc = (cpu->registers[0x0] + (instruction & 0xfff));
            break;
        
        case 0xc000:
            uint8_t random_num = rand() % 256;
            cpu->registers[x] = (random_num & (instruction & 0xff));
            break;
        
        case 0xd000:
            uint8_t height = (instruction & 0xf);
            cpu->registers[0xf] = 0;

            for (size_t row = 0; row < height; row++) {
                uint8_t sprite_row = cpu->memory[cpu->i + row];
                for (size_t col = 0; col < SPRITE_WIDTH; col++) {
                    if (sprite_row & 0x80 > 0) {
                        if (setPixel(renderer, cpu->registers[x] + col, cpu->registers[y] + row)) {
                            cpu->registers[0xf] = 1;
                        }
                    }
                    sprite_row <<= 1;
                }
            }
            break;
        
        case 0xe000:
            switch(instruction & 0xff) {
                case 0x9e:
                    if (isKeyPressed(keyboard, cpu->registers[x])) {
                        cpu->pc += 2;
                    }
                    break;
                
                case 0xa1:
                    if (!isKeyPressed(keyboard, cpu->registers[x])) {
                        cpu->pc += 2;
                    }
                    break;
            }
            break;
        
        case 0xf000:
            switch(instruction & 0xff) {
                case 0x07:
                    cpu->registers[x] = cpu->delayTimer;
                    break;
                
                case 0x0a:
                    cpu->paused = 1;
                    cpu->key_register = x;
                    keyboard->onNextKeyPress = onNextKeyPress;
                    break;
                
                case 0x15:
                    cpu->delayTimer = cpu->registers[x];
                    break;
                
                case 0x18:
                    cpu->soundTimer = cpu->registers[x];
                    break;
                
                case 0x1E:
                    cpu->i += cpu->registers[x];
                    break;
                
                case 0x29:
                    cpu->i = cpu->registers[x] * 5;
                    break;
                
                case 0x33:
                    cpu->memory[cpu->i] = (int)(cpu->registers[x] / 100);
                    cpu->memory[cpu->i+1] = (int)((cpu->registers[x] % 100) / 10);
                    cpu->memory[cpu->i+2] = (int)(cpu->registers[x] % 10); 
                    break;
                
                case 0x55:
                    break;
                
                case 0x65:
                    break;
            }
            break;

        break;
    }

}
