#include "cube.h"
#include <cstdlib>

EffectScanLine::EffectScanLine() {
    position = 0;
    direction = 1;
    lineColor = getRandomColor();
    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            brightness[x][y] = 0;
        }
    }
}

void EffectScanLine::render(Cube& cube, unsigned long deltaTime) {
    // Затухание
    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            brightness[x][y] *= 0.9f;
            if (brightness[x][y] < 0.01f) brightness[x][y] = 0;
        }
    }

    // Обновляем позицию
    position += speed * direction;

    int maxPos = vertical ? GRID_W : GRID_H;
    if (position >= maxPos || position <= 0) {
        direction = -direction;
        lineColor = getRandomColor();
    }

    // Рисуем линию
    int pos = (int)position;
    if (vertical) {
        for (int y = 0; y < GRID_H; y++) {
            if (pos >= 0 && pos < GRID_W) {
                brightness[pos][y] = 1.0f;
            }
        }
    } else {
        for (int x = 0; x < GRID_W; x++) {
            if (pos >= 0 && pos < GRID_H) {
                brightness[x][pos] = 1.0f;
            }
        }
    }

    // Рендер
    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    for (auto& faceRef : faces) {
        Matrix& face = faceRef.get();
        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                if (brightness[x][y] > 0) {
                    uint8_t r = (uint8_t)(((lineColor >> 16) & 0xFF) * brightness[x][y]);
                    uint8_t g = (uint8_t)(((lineColor >> 8) & 0xFF) * brightness[x][y]);
                    uint8_t b = (uint8_t)((lineColor & 0xFF) * brightness[x][y]);
                    face.setPixel(x, y, (r << 16) | (g << 8) | b);
                } else {
                    face.setPixel(x, y, 0);
                }
            }
        }
    }

    cube.render();
}

uint32_t EffectScanLine::getRandomColor() {
    int hue = random(360);
    float h = hue / 60.0f;
    int i = (int)h;
    float f = h - i;
    uint8_t q = (uint8_t)(255 * (1.0f - f));
    uint8_t t = (uint8_t)(255 * f);
    switch (i % 6) {
        case 0: return (255 << 16) | (t << 8);
        case 1: return (q << 16) | (255 << 8);
        case 2: return (255 << 8) | t;
        case 3: return (255 << 8) | (q << 16);
        case 4: return (t << 16) | 255;
        default: return (255 << 16) | q;
    }
}
