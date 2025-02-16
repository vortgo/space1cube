#include "cube.h"



uint32_t getColorForHeight(int height, uint32_t highestColor, uint32_t lowestColor) {
    // Извлекаем компоненты RGB для обоих цветов
    uint8_t r1 = (lowestColor >> 16) & 0xFF;  // Красный компонент для lowestColor
    uint8_t g1 = (lowestColor >> 8) & 0xFF;   // Зеленый компонент для lowestColor
    uint8_t b1 = lowestColor & 0xFF;          // Синий компонент для lowestColor

    uint8_t r2 = (highestColor >> 16) & 0xFF;  // Красный компонент для highestColor
    uint8_t g2 = (highestColor >> 8) & 0xFF;   // Зеленый компонент для highestColor
    uint8_t b2 = highestColor & 0xFF;          // Синий компонент для highestColor

    // Вычисляем фактор интерполяции от 0 до 1
    float factor = height / 7.0f;  // Чем выше пик, тем ближе к highestColor

    // Интерполируем каждый компонент цвета
    uint8_t r = static_cast<uint8_t>(r1 + (r2 - r1) * factor);
    uint8_t g = static_cast<uint8_t>(g1 + (g2 - g1) * factor);
    uint8_t b = static_cast<uint8_t>(b1 + (b2 - b1) * factor);

    // Собираем новый цвет из интерполированных компонентов
    uint32_t newColor = (r << 16) | (g << 8) | b;

    return newColor;
}

int calculateLEDHeight(int N, int M, bool increase, unsigned long accumulatedTime, int periodDuration)
{
    // Вычисляем прогресс от 0 до 1 в зависимости от accumulatedTime и periodDuration
    float progress = static_cast<float>(accumulatedTime % periodDuration) / periodDuration;

    // Если мы увеличиваем, интерполируем от N до M
    if (increase)
    {
        // Убедимся, что прогресс от 0 до 1 дает правильные значения от N до M включительно
        return static_cast<int>(N + (M - N) * progress + 0.5f); // +0.5 для округления
    }
    // Если мы уменьшаем, интерполируем от M до N
    else
    {
        // Убедимся, что прогресс от 0 до 1 дает правильные значения от M до N включительно
        return static_cast<int>(M - (M - N) * progress + 0.5f); // +0.5 для округления
    }
}

EffectSoundLevel::EffectSoundLevel()
{
    peaks.resize(8);

    for (int i = 0; i < 8; ++i)
    {
        Peak peak{};
        peak.currentHeight = 0;
        peak.increase = true;
        peak.low = 1;
        peak.hight = getRandonIntInRange(1, 7);
        peak.period = basePeriod + getRandonIntInRange(0, 400);
        peak.pauseTime = pauseTime;
        peak.accumulatedPause = 0;

        peaks[i] = peak;
    }
}

void EffectSoundLevel::renderOneFace(Matrix &face,int i, uint32_t highestColor, uint32_t lowestColor ){
    face.clearColumn(i);
    for (int y = 7; y >= peaks[i].currentHeight; --y)
    {
        uint32_t color = getColorForHeight(y, highestColor,lowestColor);
        face.setPixel(i, y, color, 1.0f);
    }
}

void EffectSoundLevel::render(Cube &cube, unsigned long deltaTime)
{
    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    accumulatedTime += deltaTime;
    bool changed = false;

    int i = 3;
    for (int i = 0; i < peaks.size(); ++i)
    {
        if (peaks[i].accumulatedPause < peaks[i].pauseTime)
        {
            peaks[i].accumulatedPause += deltaTime;
            continue;
        }

        int newHeight = calculateLEDHeight(peaks[i].low, peaks[i].hight, peaks[i].increase, accumulatedTime, peaks[i].period);
        if (newHeight == peaks[i].currentHeight)
        {
            continue;
        }
        changed = true;

        peaks[i].currentHeight = newHeight;

        for (int f = 0; f < faces.size(); f++) {
            uint32_t hColor = colorFaces[f][0];
            uint32_t lColor= colorFaces[f][1];
            renderOneFace(faces[f], i,hColor, lColor );
        }

        if ((peaks[i].currentHeight == peaks[i].hight && peaks[i].increase) || (peaks[i].currentHeight == peaks[i].low && !peaks[i].increase))
        {
            if (peaks[i].currentHeight < 3)
            {
                peaks[i].increase = true;
                peaks[i].low = peaks[i].currentHeight;
                peaks[i].hight = getRandonIntInRange(peaks[i].currentHeight + 1, 7);
            }
            else
            {
                peaks[i].increase = false;
                peaks[i].hight = peaks[i].currentHeight;
                peaks[i].low = getRandonIntInRange(1, peaks[i].currentHeight - 1);
            }

            peaks[i].accumulatedPause = 0;
            peaks[i].period = basePeriod + getRandonIntInRange(0, 200);
            peaks[i].pauseTime = pauseTime;
        }
    }

    if (changed)
    {
        cube.render();
    }
}