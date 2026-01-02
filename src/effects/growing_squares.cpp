#include "cube.h"
#include <cstdlib>

EffectGrowingSquares::EffectGrowingSquares() {
    currentSize = 0;
    currentColor = getRandomColor();
    previousColor = 0;
}

void EffectGrowingSquares::render(Cube& cube, unsigned long deltaTime) {
    currentSize += growSpeed;

    // Достигли максимума - новый цвет
    if (currentSize >= GRID_W / 2 + 1) {
        currentSize = 0;
        previousColor = currentColor;
        currentColor = getRandomColor();
    }

    int cx = GRID_W / 2;
    int cy = GRID_H / 2;

    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    for (auto& faceRef : faces) {
        Matrix& face = faceRef.get();

        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                // Расстояние от центра (Чебышёва для квадрата)
                int dx = abs(x - cx);
                int dy = abs(y - cy);
                int dist = dx > dy ? dx : dy;

                uint32_t color;
                if (dist <= (int)currentSize) {
                    color = currentColor;
                } else {
                    color = previousColor;
                }

                face.setPixel(x, y, color);
            }
        }
    }

    cube.render();
}

uint32_t EffectGrowingSquares::getRandomColor() {
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
