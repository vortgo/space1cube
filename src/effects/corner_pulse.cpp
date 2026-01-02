#include "cube.h"
#include <cstdlib>
#include <cmath>

EffectCornerPulse::EffectCornerPulse() {
    currentCorner = 0;
    pulseRadius = 0;
    timeSincePulse = 0;
    pulseColor = getRandomColor();
}

void EffectCornerPulse::render(Cube& cube, unsigned long deltaTime) {
    timeSincePulse += deltaTime;

    // Время для нового пульса?
    if (pulseRadius <= 0 && timeSincePulse >= interval) {
        timeSincePulse = 0;
        pulseRadius = 0.1f;
        currentCorner = random(4);
        pulseColor = getRandomColor();
    }

    // Обновляем радиус
    if (pulseRadius > 0) {
        pulseRadius += pulseSpeed;
    }

    // Координаты угла
    float cx, cy;
    switch (currentCorner) {
        case 0: cx = 0; cy = 0; break;
        case 1: cx = GRID_W - 1; cy = 0; break;
        case 2: cx = 0; cy = GRID_H - 1; break;
        default: cx = GRID_W - 1; cy = GRID_H - 1; break;
    }

    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    for (auto& faceRef : faces) {
        Matrix& face = faceRef.get();

        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                float dx = x - cx;
                float dy = y - cy;
                float dist = sqrtf(dx * dx + dy * dy);

                float brightness = 0;
                float maxDist = sqrtf(GRID_W * GRID_W + GRID_H * GRID_H);

                if (pulseRadius > 0) {
                    // Кольцо волны
                    float ringDist = fabsf(dist - pulseRadius);
                    if (ringDist < 1.5f) {
                        brightness = (1.5f - ringDist) / 1.5f;
                        // Затухание по расстоянию
                        brightness *= 1.0f - (pulseRadius / maxDist);
                    }
                }

                if (brightness > 0) {
                    uint8_t r = (uint8_t)(((pulseColor >> 16) & 0xFF) * brightness);
                    uint8_t g = (uint8_t)(((pulseColor >> 8) & 0xFF) * brightness);
                    uint8_t b = (uint8_t)((pulseColor & 0xFF) * brightness);
                    face.setPixel(x, y, (r << 16) | (g << 8) | b);
                } else {
                    face.setPixel(x, y, 0);
                }
            }
        }
    }

    // Сброс когда волна вышла за пределы
    float maxDist = sqrtf(GRID_W * GRID_W + GRID_H * GRID_H);
    if (pulseRadius > maxDist) {
        pulseRadius = 0;
    }

    cube.render();
}

uint32_t EffectCornerPulse::getRandomColor() {
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
