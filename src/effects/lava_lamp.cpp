#include "cube.h"

void getLavaColor(float intensity, uint8_t &r, uint8_t &g, uint8_t &b) {
    float hue = 20.0f + intensity * 40.0f; // Цвета от красного к желтому
    float saturation = 1.0f;
    float value = intensity; // Интенсивность регулирует яркость
    myHsvToRgb(hue, saturation, value, r, g, b);
}

void EffectLavaLamp::render(Cube& cube, unsigned long deltaTime) {
    effectTime += deltaTime;
    if (accumulatedTime < 30) {
        accumulatedTime += deltaTime;
        return;
    }
    accumulatedTime = 0;

    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    float timeProgress = sin((float)effectTime / period * M_PI * 2) * 0.5f + 0.5f; // Плавный циклический переход

    int size = 8;
    float waveSpeed = 0.3f;
    float waveFrequency = 2.5f;

    for (int f = 0; f < faces.size(); f++) {
        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                float blob1 = exp(-pow((x - size * 0.3f) * 0.6f, 2) - pow((y - size * timeProgress) * 0.5f, 2));
                float blob2 = exp(-pow((x - size * 0.7f) * 0.6f, 2) - pow((y - size * (1.0f - timeProgress)) * 0.5f, 2));
                float intensity = std::min(1.0f, blob1 + blob2);

                uint8_t r, g, b;
                getLavaColor(intensity, r, g, b);
                faces[f].get().setPixel(x, y, (r << 16) | (g << 8) | b);
            }
        }
    }

    cube.render();
}