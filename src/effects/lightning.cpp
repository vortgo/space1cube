#include "cube.h"
#include <cstdlib>

EffectLightning::EffectLightning() {
    striking = false;
    strikeProgress = 0;
    strikeX = 0;
    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            brightness[x][y] = 0;
        }
    }
}

void EffectLightning::render(Cube& cube, unsigned long deltaTime) {
    // Затухание
    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            brightness[x][y] *= (1.0f - fadeSpeed);
            if (brightness[x][y] < 0.01f) brightness[x][y] = 0;
        }
    }

    // Новая молния?
    if (!striking && (float)random(1000) / 1000.0f < strikeChance) {
        strikeX = random(GRID_W);
        striking = true;
        strikeProgress = 0;
        generateBolt(strikeX);
    }

    if (striking) {
        strikeProgress++;
        if (strikeProgress > GRID_H + 5) {
            striking = false;
        }
    }

    // Рендер
    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    for (auto& faceRef : faces) {
        Matrix& face = faceRef.get();
        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                if (brightness[x][y] > 0) {
                    uint8_t b = (uint8_t)(255 * brightness[x][y]);
                    uint8_t purple = (uint8_t)(200 * brightness[x][y]);
                    face.setPixel(x, y, (purple << 16) | (purple << 8) | b);
                } else {
                    face.setPixel(x, y, 0);
                }
            }
        }
    }
    cube.render();
}

void EffectLightning::generateBolt(int startX) {
    int x = startX;
    for (int y = 0; y < GRID_H; y++) {
        // Основной болт
        if (x >= 0 && x < GRID_W) {
            brightness[x][y] = 1.0f;
        }

        // Случайное ветвление
        if (random(100) < 30 && y < GRID_H - 2) {
            int branchX = x + (random(2) == 0 ? -1 : 1);
            if (branchX >= 0 && branchX < GRID_W) {
                brightness[branchX][y] = 0.7f;
                if (y + 1 < GRID_H) {
                    brightness[branchX][y + 1] = 0.5f;
                }
            }
        }

        // Случайное смещение основного болта
        if (random(100) < 40) {
            x += (random(2) == 0 ? -1 : 1);
            if (x < 0) x = 0;
            if (x >= GRID_W) x = GRID_W - 1;
        }
    }
}
