#include "cube.h"
#include <cstdlib>

EffectSparkle::EffectSparkle() {
    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            brightness[x][y] = 0;
            colors[x][y] = 0xFFFFFF;
        }
    }
}

void EffectSparkle::render(Cube& cube, unsigned long deltaTime) {
    // Затухание
    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            brightness[x][y] *= (1.0f - fadeSpeed);
            if (brightness[x][y] < 0.01f) brightness[x][y] = 0;
        }
    }

    // Новые искры
    int sparks = (int)(sparkleRate * 10);
    for (int i = 0; i < sparks; i++) {
        if ((float)random(100) / 100.0f < sparkleRate) {
            int x = random(GRID_W);
            int y = random(GRID_H);
            brightness[x][y] = 1.0f;

            // Случайный цвет (белый с оттенком)
            int r = 200 + random(55);
            int g = 200 + random(55);
            int b = 200 + random(55);
            colors[x][y] = (r << 16) | (g << 8) | b;
        }
    }

    // Рендер
    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    for (auto& faceRef : faces) {
        Matrix& face = faceRef.get();
        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                if (brightness[x][y] > 0) {
                    uint32_t c = colors[x][y];
                    uint8_t r = (uint8_t)(((c >> 16) & 0xFF) * brightness[x][y]);
                    uint8_t g = (uint8_t)(((c >> 8) & 0xFF) * brightness[x][y]);
                    uint8_t b = (uint8_t)((c & 0xFF) * brightness[x][y]);
                    face.setPixel(x, y, (r << 16) | (g << 8) | b);
                } else {
                    face.setPixel(x, y, 0);
                }
            }
        }
    }

    cube.render();
}
