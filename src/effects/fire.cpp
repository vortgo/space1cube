#include "cube.h"
#include <cstdlib>

EffectFire::EffectFire() {
    // Инициализируем карту тепла нулями
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            heat[x][y] = 0;
        }
    }
}

void EffectFire::render(Cube& cube, unsigned long deltaTime) {
    // Выполняем один шаг симуляции огня
    fireStep();

    // Отображаем результат на всех гранях
    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    for (auto& faceRef : faces) {
        Matrix& face = faceRef.get();
        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                uint32_t color = heatToColor(heat[x][y]);
                face.setPixel(x, y, color);
            }
        }
    }
    cube.render();
}

void EffectFire::fireStep() {
    // Шаг 1: Охлаждение - каждая ячейка немного теряет тепло
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            int cooldown = random(0, ((cooling * 10) / HEIGHT) + 2);
            if (cooldown > heat[x][y]) {
                heat[x][y] = 0;
            } else {
                heat[x][y] -= cooldown;
            }
        }
    }

    // Шаг 2: Диффузия тепла вверх с размытием
    // Тепло поднимается снизу вверх с усреднением соседних значений
    for (int y = HEIGHT - 1; y >= 2; y--) {
        for (int x = 0; x < WIDTH; x++) {
            // Берём среднее от 3 ячеек снизу (с учётом границ)
            int leftX = (x == 0) ? WIDTH - 1 : x - 1;
            int rightX = (x == WIDTH - 1) ? 0 : x + 1;

            heat[x][y] = (heat[leftX][y - 1] +
                          heat[x][y - 1] +
                          heat[x][y - 1] +
                          heat[rightX][y - 1]) / 4;
        }
    }

    // Шаг 3: Генерация искр внизу (случайное зажигание)
    for (int x = 0; x < WIDTH; x++) {
        // Вероятность появления искры зависит от параметра sparking
        if (random(255) < sparking) {
            // Зажигаем нижние ячейки (y = 0 или y = 1)
            int y = random(2);
            // Добавляем случайное значение тепла
            int newHeat = heat[x][y] + random(160, 255);
            heat[x][y] = (newHeat > 255) ? 255 : newHeat;
        }
    }
}

uint32_t EffectFire::heatToColor(uint8_t h) {
    // Преобразуем значение тепла в цвет огня
    // Используем палитру: черный -> красный -> оранжевый -> жёлтый -> белый

    uint8_t r, g, b;

    // Применяем сдвиг оттенка для разных цветов огня
    // hueShift: 0 = оранжевый, 80 = зелёный, 160 = синий

    // Масштабируем h для работы с палитрой
    uint8_t t192 = (uint8_t)((h * 192UL) / 256);

    // Вычисляем позицию в палитре
    uint8_t heatramp = t192 & 0x3F;  // 0..63
    heatramp <<= 2;  // масштабируем до 0..252

    if (t192 >= 128) {
        // Горячий: жёлтый -> белый
        r = 255;
        g = 255;
        b = heatramp;
    } else if (t192 >= 64) {
        // Средний: красный -> жёлтый
        r = 255;
        g = heatramp;
        b = 0;
    } else {
        // Холодный: чёрный -> красный
        r = heatramp;
        g = 0;
        b = 0;
    }

    // Применяем сдвиг оттенка если нужен нестандартный цвет огня
    if (hueShift != 0) {
        // Конвертируем RGB в HSV, сдвигаем оттенок, конвертируем обратно
        // Упрощённый вариант: ротация каналов
        float shift = hueShift / 255.0f;
        if (shift < 0.33f) {
            // Красный -> Зелёный
            float t = shift * 3.0f;
            uint8_t newR = (uint8_t)(r * (1.0f - t) + g * t);
            uint8_t newG = (uint8_t)(g * (1.0f - t) + b * t);
            uint8_t newB = (uint8_t)(b * (1.0f - t) + r * t);
            r = newR; g = newG; b = newB;
        } else if (shift < 0.66f) {
            // Зелёный -> Синий
            float t = (shift - 0.33f) * 3.0f;
            uint8_t newR = (uint8_t)(g * (1.0f - t) + b * t);
            uint8_t newG = (uint8_t)(b * (1.0f - t) + r * t);
            uint8_t newB = (uint8_t)(r * (1.0f - t) + g * t);
            r = newR; g = newG; b = newB;
        } else {
            // Синий -> Красный
            float t = (shift - 0.66f) * 3.0f;
            uint8_t newR = (uint8_t)(b * (1.0f - t) + r * t);
            uint8_t newG = (uint8_t)(r * (1.0f - t) + g * t);
            uint8_t newB = (uint8_t)(g * (1.0f - t) + b * t);
            r = newR; g = newG; b = newB;
        }
    }

    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}
