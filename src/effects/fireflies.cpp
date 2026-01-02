#include "cube.h"
#include <cstdlib>
#include <cmath>

EffectFireflies::EffectFireflies() {
    for (int i = 0; i < MAX_FIREFLIES; i++) {
        fireflies[i].x = random(GRID_W);
        fireflies[i].y = random(GRID_H);
        fireflies[i].phase = (float)random(628) / 100.0f; // 0 to 2*PI
        fireflies[i].speed = 0.02f + (float)random(30) / 1000.0f;
        fireflies[i].brightness = 0;
    }
}

void EffectFireflies::render(Cube& cube, unsigned long deltaTime) {
    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();

    for (auto& faceRef : faces) {
        Matrix& face = faceRef.get();

        // Очищаем
        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                face.setPixel(x, y, 0);
            }
        }

        // Обновляем и рисуем светлячков
        for (int i = 0; i < fireflyCount && i < MAX_FIREFLIES; i++) {
            fireflies[i].phase += fireflies[i].speed * blinkSpeed * 10.0f;
            if (fireflies[i].phase > 2 * M_PI) {
                fireflies[i].phase -= 2 * M_PI;

                // Иногда перемещаемся
                if (random(100) < 30) {
                    fireflies[i].x = random(GRID_W);
                    fireflies[i].y = random(GRID_H);
                }
            }

            // Плавное мигание
            fireflies[i].brightness = (sinf(fireflies[i].phase) + 1.0f) / 2.0f;

            if (fireflies[i].brightness > 0.1f) {
                uint8_t g = (uint8_t)(255 * fireflies[i].brightness);
                uint8_t r = (uint8_t)(200 * fireflies[i].brightness);
                face.setPixel(fireflies[i].x, fireflies[i].y, (r << 16) | (g << 8));
            }
        }
    }

    cube.render();
}
