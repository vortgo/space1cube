#include "cube.h"
#include <cstdlib>

EffectGameOfLife::EffectGameOfLife() {
    // Инициализируем случайным образом
    randomize();
    cellColor = getRandomColor();
}

void EffectGameOfLife::render(Cube& cube, unsigned long deltaTime) {
    // Накапливаем время
    timeSinceUpdate += deltaTime;
    timeSinceChange += deltaTime;

    // Проверяем, пора ли обновить поколение
    if (timeSinceUpdate >= updateInterval) {
        timeSinceUpdate = 0;

        // Считаем текущую популяцию
        int currentPopulation = 0;
        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                if (grid[x][y]) currentPopulation++;
            }
        }

        // Проверяем стагнацию (популяция не меняется)
        if (currentPopulation == lastPopulation) {
            stagnationCount++;
        } else {
            stagnationCount = 0;
            timeSinceChange = 0;
        }
        lastPopulation = currentPopulation;

        // Если стагнация слишком долгая или популяция вымерла - рестарт
        if (timeSinceChange >= randomizeThreshold ||
            currentPopulation == 0 ||
            stagnationCount > 20) {

            randomize();
            cellColor = getRandomColor();
            timeSinceChange = 0;
            stagnationCount = 0;
        }

        // Вычисляем следующее поколение
        nextGeneration();

        // Копируем результат
        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                grid[x][y] = nextGrid[x][y];
            }
        }
    }

    // Отображаем текущее состояние на всех гранях
    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    for (auto& faceRef : faces) {
        Matrix& face = faceRef.get();
        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                if (grid[x][y]) {
                    face.setPixel(x, y, cellColor);
                } else {
                    face.setPixel(x, y, 0);  // Мёртвые клетки - чёрные
                }
            }
        }
    }

    cube.render();
}

void EffectGameOfLife::randomize() {
    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            // Вероятность живой клетки зависит от initialDensity
            grid[x][y] = (random(100) < initialDensity);
        }
    }
    lastPopulation = -1;  // Сброс для корректного определения стагнации
}

void EffectGameOfLife::nextGeneration() {
    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            int neighbors = countNeighbors(x, y);

            // Правила Конвея:
            // 1. Живая клетка с 2 или 3 соседями выживает
            // 2. Мёртвая клетка с ровно 3 соседями оживает
            // 3. Все остальные клетки умирают или остаются мёртвыми

            if (grid[x][y]) {
                // Живая клетка
                nextGrid[x][y] = (neighbors == 2 || neighbors == 3);
            } else {
                // Мёртвая клетка
                nextGrid[x][y] = (neighbors == 3);
            }
        }
    }
}

int EffectGameOfLife::countNeighbors(int x, int y) {
    int count = 0;

    // Проверяем все 8 соседей с заворачиванием на границах (тороидальная топология)
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;  // Пропускаем саму клетку

            // Заворачиваем координаты
            int nx = (x + dx + GRID_W) % GRID_W;
            int ny = (y + dy + GRID_H) % GRID_H;

            if (grid[nx][ny]) {
                count++;
            }
        }
    }

    return count;
}

uint32_t EffectGameOfLife::getRandomColor() {
    // Генерируем яркий насыщенный цвет
    // Используем HSV с фиксированной насыщенностью и яркостью
    float hue = (float)(random(360));

    // HSV to RGB (S = 1.0, V = 1.0)
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
