#include "cube.h"
#include <cmath>

EffectPlasma::EffectPlasma() {
    time = 0;
}

void EffectPlasma::render(Cube& cube, unsigned long deltaTime) {
    // Увеличиваем время для анимации
    time += speed;

    // Отображаем плазму на всех гранях
    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();

    for (auto& faceRef : faces) {
        Matrix& face = faceRef.get();

        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                // Нормализуем координаты
                float nx = (float)x / 8.0f * scale;
                float ny = (float)y / 8.0f * scale;

                // Комбинируем несколько синусоидальных волн для создания плазменного эффекта
                // Каждая волна имеет свою частоту и фазу

                // Волна 1: Горизонтальные полосы с вращением
                float v1 = sinf(nx + time);

                // Волна 2: Вертикальные полосы с вращением
                float v2 = sinf(ny + time * 0.7f);

                // Волна 3: Диагональные волны
                float v3 = sinf(nx + ny + time * 1.3f);

                // Волна 4: Круговые волны от центра
                float cx = nx - scale / 2.0f;
                float cy = ny - scale / 2.0f;
                float dist = sqrtf(cx * cx + cy * cy);
                float v4 = sinf(dist * 2.0f - time * 1.5f);

                // Волна 5: Спиральные волны
                float angle = atan2f(cy, cx);
                float v5 = sinf(angle * 3.0f + dist * 2.0f + time);

                // Комбинируем все волны
                float value = (v1 + v2 + v3 + v4 + v5) / 5.0f;

                // Нормализуем к диапазону 0..1
                value = (value + 1.0f) / 2.0f;

                // Получаем цвет
                uint32_t color = plasmaColor(value);
                face.setPixel(x, y, color);
            }
        }
    }

    cube.render();
}

uint32_t EffectPlasma::plasmaColor(float value) {
    // Преобразуем значение 0..1 в яркий цвет радуги
    // Используем HSV с полным спектром оттенков

    // Применяем сдвиг цвета
    float hue = value * 360.0f + (float)colorShift * 1.41f;  // 360/255
    if (hue >= 360.0f) hue -= 360.0f;
    if (hue < 0.0f) hue += 360.0f;

    // HSV to RGB (S = 1.0, V = 1.0 для ярких цветов)
    float s = 1.0f;
    float v = 1.0f;

    float c = v * s;
    float x = c * (1.0f - fabsf(fmodf(hue / 60.0f, 2.0f) - 1.0f));
    float m = v - c;

    float r1, g1, b1;

    if (hue >= 0 && hue < 60) {
        r1 = c; g1 = x; b1 = 0;
    } else if (hue >= 60 && hue < 120) {
        r1 = x; g1 = c; b1 = 0;
    } else if (hue >= 120 && hue < 180) {
        r1 = 0; g1 = c; b1 = x;
    } else if (hue >= 180 && hue < 240) {
        r1 = 0; g1 = x; b1 = c;
    } else if (hue >= 240 && hue < 300) {
        r1 = x; g1 = 0; b1 = c;
    } else {
        r1 = c; g1 = 0; b1 = x;
    }

    uint8_t r = (uint8_t)((r1 + m) * 255);
    uint8_t g = (uint8_t)((g1 + m) * 255);
    uint8_t b = (uint8_t)((b1 + m) * 255);

    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}
