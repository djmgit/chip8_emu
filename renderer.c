#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include "renderer.h"

void initRenderer(renderer_t *renderer) {
    renderer->screenWidth = renderer->screenWidth * renderer->scale;
    renderer->screenHeight = renderer->screenHeight * renderer->scale;
    renderer->display = (uint8_t *)malloc(sizeof(uint8_t) * renderer->cols * renderer->rows);
    memset(renderer->display, 0, renderer->rows * renderer->cols);
}

uint8_t setPixel(renderer_t *renderer, uint8_t x, uint8_t y) {
    if (x >= renderer->cols) {
        x -= renderer->cols;
    } else if (x < 0) {
        x += renderer->cols;
    }

    if (y >= renderer->rows) {
        y -= renderer->rows;
    } else if (y < 0) {
        y += renderer->rows;
    }

    int pixelLocation = x + (y * renderer->cols);
    renderer->display[pixelLocation] ^= 1;

    return !renderer->display[pixelLocation];
}

void clear(renderer_t *renderer) {
    memset(renderer->display, 0, renderer->cols * renderer->rows);
}

void render(renderer_t *renderer) {
    for(size_t i = 0; i < (renderer->cols * renderer->rows); i++) {
        if (!renderer->display[i]) {
            continue;
        }
        uint8_t x = (i % renderer->cols) * renderer->scale;
        uint8_t y = floor(i / renderer->cols) * renderer->scale;
        DrawRectangle(x, y, renderer->scale, renderer->scale, (Color){0,0,0,255});
    }
}
