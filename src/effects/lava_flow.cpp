#include "cube.h"
#include <cmath>

EffectLavaFlow::EffectLavaFlow() {
    time = 0;
    for (int f = 0; f < 6; f++) {
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                lavaLevel[f][x][y] = 0;
            }
        }
    }
}

uint32_t EffectLavaFlow::getLavaColor(float heat) {
    if (heat < 0.3f) {
        // Dark red/black
        uint8_t r = (uint8_t)(heat / 0.3f * 100);
        return (r << 16);
    } else if (heat < 0.6f) {
        // Red to orange
        float t = (heat - 0.3f) / 0.3f;
        uint8_t r = 100 + (uint8_t)(t * 155);
        uint8_t g = (uint8_t)(t * 100);
        return (r << 16) | (g << 8);
    } else {
        // Orange to yellow
        float t = (heat - 0.6f) / 0.4f;
        uint8_t r = 255;
        uint8_t g = 100 + (uint8_t)(t * 155);
        uint8_t b = (uint8_t)(t * 50);
        return (r << 16) | (g << 8) | b;
    }
}

void EffectLavaFlow::render(Cube& cube, unsigned long deltaTime) {
    time += flowSpeed * deltaTime;

    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Top face (4) - source of lava
    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            float noise = sin(time * 0.01f + x * 0.5f) * cos(time * 0.008f + y * 0.5f);
            lavaLevel[4][x][y] = 0.7f + noise * 0.3f;
        }
    }

    // Side faces (0,1,2,3) - lava flows down
    for (int f = 0; f < 4; f++) {
        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                float flowY = (float)y / GRID_H;
                float wave = sin(time * 0.015f + x * 0.3f + f) * 0.2f;
                float heat = (1.0f - flowY) * 0.8f + wave;
                heat = fmax(0.0f, fmin(1.0f, heat));
                lavaLevel[f][x][y] = heat;
            }
        }
    }

    // Bottom face (5) - lava pool
    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            float dist = sqrt((x - 3.5f) * (x - 3.5f) + (y - 3.5f) * (y - 3.5f));
            float heat = 1.0f - dist / 5.0f;
            float bubble = sin(time * 0.02f + x + y * 2) * 0.15f;
            heat = fmax(0.2f, fmin(1.0f, heat + bubble));
            lavaLevel[5][x][y] = heat;
        }
    }

    // Render
    cube.clear();
    for (int f = 0; f < 6; f++) {
        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                if (lavaLevel[f][x][y] > 0.1f) {
                    faces[f]->setPixel(x, y, getLavaColor(lavaLevel[f][x][y]));
                }
            }
        }
    }

    cube.render();
}
