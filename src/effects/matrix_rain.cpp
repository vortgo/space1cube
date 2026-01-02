#include "cube.h"
#include <cstdlib>
#include <cmath>

EffectMatrixRain::EffectMatrixRain() {
    // Инициализируем все капли как неактивные
    for (int x = 0; x < GRID_W; x++) {
        drops[x].active = false;
        drops[x].y = 0;
        drops[x].speed = dropSpeed;
    }

    // Инициализируем яркость пикселей
    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            brightness[x][y] = 0;
        }
    }
}

void EffectMatrixRain::render(Cube& cube, unsigned long deltaTime) {
    // Затухание всех пикселей
    float fadeAmount = 0.15f;  // Скорость затухания

    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            brightness[x][y] *= (1.0f - fadeAmount);
            if (brightness[x][y] < 0.01f) {
                brightness[x][y] = 0;
            }
        }
    }

    // Обновляем каждую каплю
    for (int x = 0; x < GRID_W; x++) {
        if (drops[x].active) {
            // Перемещаем каплю вниз
            drops[x].y += drops[x].speed;

            int iy = (int)drops[x].y;

            // Рисуем яркую головку капли
            if (iy >= 0 && iy < GRID_H) {
                brightness[x][iy] = 1.0f;  // Максимальная яркость для головки
            }

            // Рисуем хвост с постепенным затуханием
            for (int t = 1; t <= trailLength; t++) {
                int tailY = iy - t;
                if (tailY >= 0 && tailY < GRID_H) {
                    float tailBrightness = 1.0f - ((float)t / (trailLength + 1));
                    tailBrightness *= 0.7f;  // Хвост менее яркий чем голова
                    if (brightness[x][tailY] < tailBrightness) {
                        brightness[x][tailY] = tailBrightness;
                    }
                }
            }

            // Проверяем, вышла ли капля за пределы + хвост
            if (iy > GRID_H + trailLength) {
                drops[x].active = false;
            }
        } else {
            // Пытаемся создать новую каплю
            if ((float)random(1000) / 1000.0f < spawnChance * 0.1f) {
                spawnDrop(x);
            }
        }
    }

    // Отображаем результат на всех гранях
    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    for (auto& faceRef : faces) {
        Matrix& face = faceRef.get();
        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                uint32_t color = getGreenShade(brightness[x][y]);
                face.setPixel(x, y, color);
            }
        }
    }

    cube.render();
}

void EffectMatrixRain::spawnDrop(int x) {
    drops[x].y = -1;  // Начинаем сверху за экраном
    drops[x].active = true;

    // Немного случайная скорость для разнообразия
    drops[x].speed = dropSpeed * (0.7f + (float)random(60) / 100.0f);
}

uint32_t EffectMatrixRain::getGreenShade(float intensity) {
    if (intensity <= 0) {
        return 0;  // Чёрный
    }

    // Ограничиваем интенсивность
    if (intensity > 1.0f) intensity = 1.0f;

    // Головка капли - яркая белая с зелёным оттенком
    // Хвост - тёмно-зелёный
    uint8_t r, g, b;

    if (intensity > 0.85f) {
        // Яркая головка - белая с зелёным оттенком
        float headIntensity = (intensity - 0.85f) / 0.15f;
        r = (uint8_t)(180 * headIntensity);
        g = 255;
        b = (uint8_t)(180 * headIntensity);
    } else {
        // Хвост - чисто зелёный с вариацией яркости
        r = 0;
        g = (uint8_t)(255 * intensity);
        b = (uint8_t)(20 * intensity);  // Небольшой синий оттенок для глубины
    }

    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}
