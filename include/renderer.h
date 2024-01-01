#include <stdlib.h>
#include <stdint.h>

#ifndef RENDERER_H
#define RENDERER_H

typedef struct {
    uint8_t cols;
    uint8_t rows;
    uint8_t scale;
    uint8_t *display;
    int screenWidth;
    int screenHeight;
} renderer_t;

void initRenderer(renderer_t *renderer);
uint8_t setPixel(renderer_t *renderer, uint8_t, uint8_t);
void clear(renderer_t *renderer);
void render(renderer_t *renderer);

#endif