#include "cube.h"
#include <cstdlib>

EffectPixelSort::EffectPixelSort() {
    timeSinceShuffle = 0;
    sorting = false;
    sortStep = 0;
    shuffle();
}

void EffectPixelSort::render(Cube& cube, unsigned long deltaTime) {
    timeSinceShuffle += deltaTime;

    if (!sorting) {
        if (timeSinceShuffle >= shuffleTime) {
            shuffle();
            timeSinceShuffle = 0;
        }
    } else {
        // Делаем шаг сортировки
        sortOneStep();
    }

    // Рендер
    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    for (auto& faceRef : faces) {
        Matrix& face = faceRef.get();
        for (int y = 0; y < GRID_H; y++) {
            for (int x = 0; x < GRID_W; x++) {
                int idx = y * GRID_W + x;
                face.setPixel(x, y, pixels[idx]);
            }
        }
    }

    cube.render();
}

void EffectPixelSort::shuffle() {
    // Создаём градиент
    for (int i = 0; i < GRID_W * GRID_H; i++) {
        int hue = (i * 360) / (GRID_W * GRID_H);
        pixels[i] = hueToColor(hue);
    }

    // Перемешиваем
    for (int i = GRID_W * GRID_H - 1; i > 0; i--) {
        int j = random(i + 1);
        uint32_t tmp = pixels[i];
        pixels[i] = pixels[j];
        pixels[j] = tmp;
    }

    sorting = true;
    sortStep = 0;
}

void EffectPixelSort::sortOneStep() {
    bool swapped = false;
    int totalPixels = GRID_W * GRID_H;

    // Bubble sort один проход
    for (int i = 0; i < totalPixels - 1 - sortStep; i++) {
        // Сравниваем по яркости/hue
        uint32_t c1 = pixels[i];
        uint32_t c2 = pixels[i + 1];

        // Извлекаем яркость
        int b1 = ((c1 >> 16) & 0xFF) + ((c1 >> 8) & 0xFF) + (c1 & 0xFF);
        int b2 = ((c2 >> 16) & 0xFF) + ((c2 >> 8) & 0xFF) + (c2 & 0xFF);

        if (b1 > b2) {
            pixels[i] = c2;
            pixels[i + 1] = c1;
            swapped = true;
        }
    }

    sortStep++;
    if (!swapped || sortStep >= totalPixels) {
        sorting = false;
        timeSinceShuffle = 0;
    }
}

uint32_t EffectPixelSort::hueToColor(int hue) {
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
