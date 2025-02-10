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
    
    cube.clear();
    
    setOnFace(cube.top);
    // setOnFace(cube.back);
    setOnFace(cube.front);
    // setOnFace(cube.bottom);
    // setOnFace(cube.left);
    // setOnFace(cube.right);
    
    cube.render();
}


  // Для каждого пикселя матрицы вычисляем итоговую яркость
    // Если пиксель в шаблоне большого сердца равен 1, тогда valueBig = 1, иначе 0.
    // Аналогично для маленького сердца – valueSmall.
    // Интерполяция осуществляется по формуле:
    //   finalBrightness = blend * valueBig + (1.0f - blend) * valueSmall
void EffectBeatingHeart::setOnFace(Matrix &f){
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            float valueBig = static_cast<float>(bigHeart[y][x]);
            float finalBrightness = blend * valueBig;  // Только для большого сердца
            f.setPixel(x,y, 0xFF0000, finalBrightness);
        }
    }
}