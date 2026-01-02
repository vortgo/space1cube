#include "cube.h"
#include <cstdlib>
#include <cmath>

EffectRipplePond::EffectRipplePond() {
    for (int i = 0; i < MAX_RIPPLES; i++) {
        ripples[i].active = false;
    }
}

void EffectRipplePond::render(Cube& cube, unsigned long deltaTime) {
    // Новая капля?
    if ((float)random(1000) / 1000.0f < dropRate) {
        spawnRipple();
    }

    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    for (auto& faceRef : faces) {
        Matrix& face = faceRef.get();

        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                float totalBrightness = 0;

                for (int i = 0; i < MAX_RIPPLES; i++) {
                    if (ripples[i].active) {
                        float dx = x - ripples[i].x;
                        float dy = y - ripples[i].y;
                        float dist = sqrtf(dx * dx + dy * dy);

                        // Кольцо волны
                        float ringDist = fabsf(dist - ripples[i].radius);
                        if (ringDist < 1.5f) {
                            float b = (1.5f - ringDist) / 1.5f * ripples[i].life;
                            totalBrightness += b;
                        }
                    }
                }

                if (totalBrightness > 1.0f) totalBrightness = 1.0f;

                if (totalBrightness > 0) {
                    uint8_t b = (uint8_t)(100 * totalBrightness);
                    uint8_t g = (uint8_t)(150 * totalBrightness);
                    uint8_t blue = (uint8_t)(255 * totalBrightness);
                    face.setPixel(x, y, (b << 16) | (g << 8) | blue);
                } else {
                    face.setPixel(x, y, 0x000010); // Тёмно-синий фон
                }
            }
        }
    }

    // Обновляем волны
    for (int i = 0; i < MAX_RIPPLES; i++) {
        if (ripples[i].active) {
            ripples[i].radius += waveSpeed;
            ripples[i].life -= 0.02f;

            if (ripples[i].life <= 0 || ripples[i].radius > GRID_W * 2) {
                ripples[i].active = false;
            }
        }
    }

    cube.render();
}

void EffectRipplePond::spawnRipple() {
    for (int i = 0; i < MAX_RIPPLES; i++) {
        if (!ripples[i].active) {
            ripples[i].x = random(GRID_W);
            ripples[i].y = random(GRID_H);
            ripples[i].radius = 0;
            ripples[i].life = 1.0f;
            ripples[i].active = true;
            break;
        }
    }
}
