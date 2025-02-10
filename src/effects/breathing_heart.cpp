#include "cube.h"

void EffectBeatingHeart::render(Cube &cube, unsigned long deltaTime) {
    // Преобразуем миллисекунды в секунды
    float deltaSec = deltaTime / 1000.0f;
    accumulatedTime += deltaSec;
    
    // Вычисляем позицию в цикле биения [0, beatPeriod)
    float phaseTime = fmod(accumulatedTime, beatPeriod);
    
    // Расчёт blend – интерполяция между большим и маленьким сердцем:
    // blend = 1.0 => полностью большое сердце, blend = 0.0 => полностью маленькое
    if (phaseTime < contractionFraction * beatPeriod) {
        float t = phaseTime / (contractionFraction * beatPeriod);
        blend = 1.0f - t;  // при t=0: blend=1, при t=1: blend=0
    } else if (phaseTime < (contractionFraction + expansionFraction) * beatPeriod) {
        float t = (phaseTime - contractionFraction * beatPeriod) / (expansionFraction * beatPeriod);
        blend = t;         // при t=0: blend=0, при t=1: blend=1
    } else {
        blend = 1.0f;
    }
    
    // Расчёт дополнительного коэффициента для маленького сердца:
    // В фазе сжатия маленькое сердце должно быть на 30% темнее (то есть иметь яркость 70% от нормальной)
    if (phaseTime < contractionFraction * beatPeriod) {
        smallHeartFactor = 0.7f; // фиксированное уменьшение на 30%
    } else if (phaseTime < (contractionFraction + expansionFraction) * beatPeriod) {
        // Во время расширения яркость маленького сердца плавно растёт с 70% до 100%
        float t = (phaseTime - contractionFraction * beatPeriod) / (expansionFraction * beatPeriod);
        smallHeartFactor = 0.7f + 0.3f * t;
    } else {
        smallHeartFactor = 1.0f;
    }
    
    cube.clear();
    
    setOnFace(cube.top);
    setOnFace(cube.back);
    setOnFace(cube.front);
    setOnFace(cube.bottom);
    setOnFace(cube.left);
    setOnFace(cube.right);
    
    cube.render();
}


  // Для каждого пикселя матрицы вычисляем итоговую яркость
    // Если пиксель в шаблоне большого сердца равен 1, тогда valueBig = 1, иначе 0.
    // Аналогично для маленького сердца – valueSmall.
    // Интерполяция осуществляется по формуле:
    //   finalBrightness = blend * valueBig + (1.0f - blend) * valueSmall
    void EffectBeatingHeart::setOnFace(Matrix &f) {
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                float valueBig = static_cast<float>(bigHeart[y][x]);
                float valueSmall = static_cast<float>(smallHeart[y][x]);
                // Интерполяция: blend берёт вклад большого сердца, а (1 - blend) – вклад маленького, 
                // при этом маленькое сердце умножается на smallHeartFactor для уменьшения яркости.
                float finalBrightness = blend * valueBig + (1.0f - blend) * (valueSmall * smallHeartFactor);
                
                Serial.println("setOnFace");
                Serial.println(x);
                Serial.println(y);
                Serial.println(finalBrightness);
                f.setPixel(x, y, 0xFF0000, finalBrightness);
            }
        }
    }
    