#include "cube.h"

void EffectBeatingHeart::render(Cube &cube, unsigned long deltaTime) {
    // Преобразуем миллисекунды в секунды
    float deltaSec = deltaTime / 1000.0f;
    accumulatedTime += deltaSec;
    
    // Вычисляем позицию в цикле биения (значение в диапазоне [0, beatPeriod))
    float phaseTime = fmod(accumulatedTime, beatPeriod);
    
    // Расчёт blend – интерполяция между большим и маленьким сердцем:
    if (phaseTime < contractionFraction * beatPeriod) {
        // Фаза сжатия (систола) – сердце сжимается
        float t = phaseTime / (contractionFraction * beatPeriod);
        blend = 1.0f - t;  // при t=0: blend=1, при t=1: blend=0
    } else if (phaseTime < (contractionFraction + expansionFraction) * beatPeriod) {
        // Фаза расширения (диастола) – сердце расширяется
        float t = (phaseTime - contractionFraction * beatPeriod) / (expansionFraction * beatPeriod);
        blend = t;         // при t=0: blend=0, при t=1: blend=1
    } else {
        blend = 1.0f;
    }
    
    // Очищаем куб
    cube.clear();
    
    // Рендерим на всех сторонах куба
    setOnFace(cube.top);
    setOnFace(cube.front);
    setOnFace(cube.back); // Можно раскомментировать, если нужно отобразить на всех сторонах
    setOnFace(cube.bottom);
    setOnFace(cube.left);
    setOnFace(cube.right);
    
    // Вызываем render на кубе
    cube.render();
}

// Линейная интерполяция между двумя цветами
uint32_t interpolateColor(uint32_t c1, uint32_t c2, float factor) {
    // Извлекаем компоненты цветов (красный, зелёный, синий)
    uint8_t r1 = (c1 >> 16) & 0xFF;
    uint8_t g1 = (c1 >> 8) & 0xFF;
    uint8_t b1 = c1 & 0xFF;
    
    uint8_t r2 = (c2 >> 16) & 0xFF;
    uint8_t g2 = (c2 >> 8) & 0xFF;
    uint8_t b2 = c2 & 0xFF;
    
    // Интерполяция по каждому каналу (красный, зелёный, синий)
    uint8_t r = r1 + (uint8_t)((r2 - r1) * factor);
    uint8_t g = g1 + (uint8_t)((g2 - g1) * factor);
    uint8_t b = b1 + (uint8_t)((b2 - b1) * factor);
    
    // Возвращаем объединённый цвет
    return (r << 16) | (g << 8) | b;
}



  // Для каждого пикселя матрицы вычисляем итоговую яркость
    // Если пиксель в шаблоне большого сердца равен 1, тогда valueBig = 1, иначе 0.
    // Аналогично для маленького сердца – valueSmall.
    // Интерполяция осуществляется по формуле:
    //   finalBrightness = blend * valueBig + (1.0f - blend) * valueSmall
    void EffectBeatingHeart::setOnFace(Matrix &f) {
        // Определяем два цвета для интерполяции
        uint32_t darkRed   = 0x990000; // Тёмно-красный (для маленького сердца)
        uint32_t brightRed = 0xFF0000; // Ярко-красный (для большого сердца)
        
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                // Определяем, входит ли пиксель в шаблон большого сердца
                float valueBig = static_cast<float>(bigHeart[y][x]);
                
                // Интерполяция яркости – от минимальной яркости (0.2) для маленького сердца до полной яркости для большого
                float finalBrightness = blend * valueBig;  // Только для большого сердца
                
                // Интерполяция цвета: от тёмно-красного до ярко-красного
                uint32_t currentColor = interpolateColor(darkRed, brightRed, blend);
                
                // Добавляем случайную вариацию для плавности (делаем анимацию менее механической)
                float noise = ((float)random(-5, 6)) / 100.0f;  // случайное число от -0.05 до +0.05
                finalBrightness += noise;
                if (finalBrightness < 0.0f) finalBrightness = 0.0f;
                if (finalBrightness > 1.0f) finalBrightness = 1.0f;
                
                // Отображаем пиксель с вычисленным цветом и яркостью
                f.setPixel(x, y, currentColor, finalBrightness);
            }
        }
    }
    