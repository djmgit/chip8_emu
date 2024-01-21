#include "raylib.h"
#include <stdio.h>
#include "renderer.h"
#include "keyboard.h"
#include "cpu.h"

#define CHIP8_SCREEN_WIDTH 64
#define CHIP8_SCREEN_HEIGHT 32
#define SCALE 10
#define TARGET_FPS 60

renderer_t renderer = {
    .cols = CHIP8_SCREEN_WIDTH,
    .rows = CHIP8_SCREEN_HEIGHT,
    .scale = SCALE,
    .display = NULL
};

keyboard_t keyboard = (keyboard_t){};
cpu_t cpu = (cpu_t){};

int main(int argc, char *argv[]) {
    InitWindow(CHIP8_SCREEN_WIDTH * SCALE, CHIP8_SCREEN_HEIGHT * SCALE, "CHIP8 Emulator");
    SetTargetFPS(TARGET_FPS);
    initRenderer(&renderer);
    initKeyBoard(&keyboard);
    initCPU(&cpu);
    if (argc == 2) {
        loadProgramIntoMemory(&cpu, argv[1]);
    }

    while (!WindowShouldClose()) {
        processKey(&keyboard, &cpu);
        ClearBackground(RAYWHITE);
        cycle(&cpu, &keyboard, &renderer);
        BeginDrawing();
            render(&renderer);
        EndDrawing();
    }
}

