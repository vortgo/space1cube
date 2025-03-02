#include "cube.h"

void getColorForDiagonal(int diagonal, int totalDiagonals, float timeProgress, uint8_t &r, uint8_t &g, uint8_t &b) {
    // Рассчитываем оттенок (Hue) на основе времени и диагонали
    float hue = fmod(timeProgress * 360.0f + (360.0f / totalDiagonals) * diagonal, 360.0f);

    // Насыщенность и яркость
    float saturation = 1.0f;  // Максимальная насыщенность
    float value = 1.0f;  // Максимальная яркость

    // Преобразуем в RGB с использованием вашей функции
    myHsvToRgb(hue, saturation, value, r, g, b);
}

void EffectRomb::render(Cube& cube, unsigned long deltaTime) {
    effectTime += deltaTime;
    if (accumulatedTime < 30){
        accumulatedTime += deltaTime;
        return;
    }

    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();

    accumulatedTime = 0;
    float timeInCycle = std::fmod(effectTime, period);

    int size = 8;
    int halfSize = size / 2;
    int currentDiagonal = 0;
    float timeProgress = timeInCycle / period;

    for (int start = halfSize - 1; start >= 0; --start)
    {
        uint8_t r, g, b;
        getColorForDiagonal(currentDiagonal, 7, timeProgress, r, g, b);
        for (int i = start, j = halfSize -1 ; i < halfSize; ++i,--j)
        {
            for (int f = 0; f < faces.size(); f++) {
               faces[f].get().setPixel(i, j, (r << 16) | (g << 8) | b);
               faces[f].get().setPixel(i, size - j -1, (r << 16) | (g << 8) | b);
               faces[f].get().setPixel(size - i -1 , j, (r << 16) | (g << 8) | b);
               faces[f].get().setPixel(size - i -1 , size - j -1 , (r << 16) | (g << 8) | b);
            }
        }
        currentDiagonal++;
    }

    for (int start = 2; start >=0; --start)
    {
        uint8_t r, g, b;
        getColorForDiagonal(currentDiagonal, 7, timeProgress, r, g, b);
        for (int i = 0, j = start; j >= 0; ++i, --j)
        {
            for (int f = 0; f < faces.size(); f++) {
                faces[f].get().setPixel(i, j, (r << 16) | (g << 8) | b);
                faces[f].get().setPixel(i, size - j -1, (r << 16) | (g << 8) | b);
                faces[f].get().setPixel(size - i -1 , j, (r << 16) | (g << 8) | b);
                faces[f].get().setPixel(size - i -1 , size - j -1 , (r << 16) | (g << 8) | b);
            }
        }
        currentDiagonal++;
    }

    cube.render();
}
