#include "raylib.h"
#include <stdio.h>
#include "cpu.h"
#include "renderer.h"

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

int main() {
    InitWindow(CHIP8_SCREEN_WIDTH * SCALE, CHIP8_SCREEN_HEIGHT * SCALE, "CHIP8 Emulator");
    SetTargetFPS(TARGET_FPS);
    show();
    initRenderer(&renderer);

    while (!WindowShouldClose()) {
        ClearBackground(RAYWHITE);

        BeginDrawing();
            render(&renderer);
        EndDrawing();
    }
}

