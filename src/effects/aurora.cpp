#include "cube.h"

void getAuroraColor(float noise, uint8_t &r, uint8_t &g, uint8_t &b) {
    float hue = fmod(noise * 360.0f, 360.0f);
    float saturation = 1.0f;
    float value = 0.8f + 0.2f * sin(noise * M_PI); // Мягкое мерцание
    myHsvToRgb(hue, saturation, value, r, g, b);
}

void EffectAurora::render(Cube& cube, unsigned long deltaTime) {
    effectTime += deltaTime;
    if (accumulatedTime < 30) {
        accumulatedTime += deltaTime;
        return;
    }
    accumulatedTime = 0;

    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    float timeProgress = sin((float)effectTime / period * M_PI * 2) * 0.5f + 0.5f; // Плавный циклический переход

    int size = 8;
    float waveSpeed = 0.5f;
    float waveFrequency = 3.0f;

    for (int f = 0; f < faces.size(); f++) {
        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                float noise = sin((x + timeProgress * waveSpeed) * waveFrequency) + cos((y - timeProgress * waveSpeed) * waveFrequency);
                noise = (noise + 2.0f) / 4.0f; // Нормализация в диапазон [0,1]

                uint8_t r, g, b;
                getAuroraColor(noise, r, g, b);
                faces[f].get().setPixel(x, y, (r << 16) | (g << 8) | b);
            }
        }
    }

    cube.render();
}

