#include "cube.h"
#include "matrix.h"
#include <map>
#include <array>

std::map<int, std::array<std::array<int, 8>, 8>> dicePatterns{
    {1, {
        {
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 1, 0, 0, 0},
            {0, 0, 0, 1, 1, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
        }
    }},
    {2, {
        {
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 1, 0},
            {0, 0, 0, 0, 0, 1, 1, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 1, 0, 0, 0, 0, 0},
            {0, 1, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
        }
    }},
    {3, {
        {
            {0, 0, 0, 0, 0, 0, 1, 1},
            {0, 0, 0, 0, 0, 0, 1, 1},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 1, 0, 0, 0},
            {0, 0, 0, 1, 1, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 0, 0, 0, 0, 0, 0},
            {1, 1, 0, 0, 0, 0, 0, 0},
        }
    }},
    {4, {
        {
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 1, 0, 0, 1, 1, 0},
            {0, 1, 1, 0, 0, 1, 1, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 1, 0, 0, 1, 1, 0},
            {0, 1, 1, 0, 0, 1, 1, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
        }
    }},
    {5, {
        {
            {1, 1, 0, 0, 0, 0, 1, 1},
            {1, 1, 0, 0, 0, 0, 1, 1},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 1, 0, 0, 0},
            {0, 0, 0, 1, 1, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 0, 0, 0, 0, 1, 1},
            {1, 1, 0, 0, 0, 0, 1, 1},
        }
    }},
    {6, {
        {
            {0, 1, 1, 0, 0, 1, 1, 0},
            {0, 1, 1, 0, 0, 1, 1, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 1, 0, 0, 1, 1, 0},
            {0, 1, 1, 0, 0, 1, 1, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 1, 0, 0, 1, 1, 0},
            {0, 1, 1, 0, 0, 1, 1, 0},
        }
    }},
};

EffectDice::EffectDice(){
  faces.resize(6);

  for(int i = 0; i< faces.size(); i++){
      EffectDice::Face f = EffectDice::Face();
      f.value = i +1;
      f.steps = 0;
      f.startColorIndex = i % colors.size();
      f.endColorIndex = (i + 1) % colors.size();

      faces[i] = f;
  }
}

uint32_t getColor(uint32_t startColor, uint32_t endColor, int step, float maxSteps) {
    // Нормализуем шаг
    float t = step / maxSteps;
    if (t > 1.0f) t = 1.0f; // На случай выхода за пределы

    // Извлекаем компоненты цвета из uint32_t
    uint8_t startR = (startColor >> 16) & 0xFF;
    uint8_t startG = (startColor >> 8) & 0xFF;
    uint8_t startB = startColor & 0xFF;

    uint8_t endR = (endColor >> 16) & 0xFF;
    uint8_t endG = (endColor >> 8) & 0xFF;
    uint8_t endB = endColor & 0xFF;

    // Интерполяция компонентов цвета
    uint8_t r = static_cast<uint8_t>(startR + t * (endR - startR));
    uint8_t g = static_cast<uint8_t>(startG + t * (endG - startG));
    uint8_t b = static_cast<uint8_t>(startB + t * (endB - startB));

    // Собираем обратно в uint32_t
    return (r << 16) | (g << 8) | b;
}

void EffectDice::changeColors(EffectDice::Face &f){
    f.startColorIndex = f.endColorIndex;
    int nextColorIndex = f.endColorIndex + 1;
    if (nextColorIndex > 5) {
      nextColorIndex = 0;
    }
    f.endColorIndex = nextColorIndex;
}

void EffectDice::renderValue(Matrix &m, int diceValue, uint32_t color){
    const auto& pattern = dicePatterns[diceValue];

    Serial.println("renderValue");
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
           if (pattern[i][j] == 1) {
               m.setPixel(i,j, color, 1.0f);
           }
        }
    }
}

void EffectDice::render(Cube& c, unsigned long deltaTime) {
    std::vector<std::reference_wrapper<Matrix>> matrices = c.getFaces();
    if (accumulatedTime < period){
        Serial.println("continue");

        Serial.print("accumulatedTime: ");
        Serial.println(accumulatedTime);

        accumulatedTime += deltaTime;
        return;
    }

    accumulatedTime = 0;
    c.clear();

    for(int i = 0; i< faces.size(); i++){
        Serial.print("I: ");
        Serial.println(i);

        faces[i].steps ++;

        uint32_t sColor = static_cast<uint32_t>(colors[faces[i].startColorIndex]);
        uint32_t eColor = static_cast<uint32_t>(colors[faces[i].endColorIndex]);
        uint32_t currentCollor = getColor(sColor, eColor, faces[i].steps, maxSteps);

        renderValue(matrices[i],faces[i].value, currentCollor);

        if (faces[i].steps >= maxSteps) {
            changeColors(faces[i]);
            faces[i].steps = 0;
        }
    }

    c.render();
}