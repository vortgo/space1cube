#include "cube.h"

EffectSpiral::EffectSpiral(){
      pixels.resize(64);
    int top = 0, bottom = 7;
    int left = 0, right = 7;

    int idx = 0;

    while (top <= bottom && left <= right) {
        // Перебор верхней строки
        for (int i = left; i <= right; ++i) {
            pixels[idx++] = Pixel(top, i);
        }
        top++;

        // Перебор правого столбца
        for (int i = top; i <= bottom; ++i) {
            pixels[idx++] = Pixel(i, right);
        }
        right--;

        if (top <= bottom) {
            // Перебор нижней строки
            for (int i = right; i >= left; --i) {
                pixels[idx++] = Pixel(bottom, i);
            }
            bottom--;
        }

        if (left <= right) {
            // Перебор левого столбца
            for (int i = bottom; i >= top; --i) {
                pixels[idx++] = Pixel(i, left);
            }
            left++;
        }
    }
}


// Функция для преобразования HSV в RGB
void hsvToRgb(float h, float s, float v, uint8_t &r, uint8_t &g, uint8_t &b) {
    float c = v * s;
    float x = c * (1 - fabs(fmod(h / 60.0f, 2) - 1));
    float m = v - c;

    float r1, g1, b1;
    if (h >= 0 && h < 60) {
        r1 = c; g1 = x; b1 = 0;
    } else if (h >= 60 && h < 120) {
        r1 = x; g1 = c; b1 = 0;
    } else if (h >= 120 && h < 180) {
        r1 = 0; g1 = c; b1 = x;
    } else if (h >= 180 && h < 240) {
        r1 = 0; g1 = x; b1 = c;
    } else if (h >= 240 && h < 300) {
        r1 = x; g1 = 0; b1 = c;
    } else {
        r1 = c; g1 = 0; b1 = x;
    }

    // Добавляем m, чтобы получить итоговые значения RGB
    r = static_cast<uint8_t>((r1 + m) * 255);
    g = static_cast<uint8_t>((g1 + m) * 255);
    b = static_cast<uint8_t>((b1 + m) * 255);
}


uint32_t getColor(int index) {
    // Делаем плавный переход от 0 до 360 (от красного до красного) для оттенка (Hue)
    float factor = index / 63.0f;  // Индекс от 0 до 63 для 64 позиции

    // Преобразуем фактор в диапазон от 0 до 360 (Hue в HSV)
    float hue = factor * 360.0f;

    // Устанавливаем Saturation и Value на максимальные значения
    float saturation = 1.0f;  // Максимальная насыщенность
    float value = 1.0f;  // Максимальная яркость

    // Конвертируем HSV в RGB
    uint8_t r, g, b;
    hsvToRgb(hue, saturation, value, r, g, b);

    // Собираем новый цвет из RGB компонентов
    uint32_t newColor = (r << 16) | (g << 8) | b;

    return newColor;
}

void EffectSpiral::render(Cube& cube, unsigned long deltaTime){
    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();

    unsigned long currentTime = millis();
    unsigned long delta = currentTime - lastTime;
    if (delta < pauseForPixel)
    {
        return;
    }
    lastTime = currentTime;
    if (index == 64){
      step++;
      index = 0;
      up = !up;
    }

    int i;
    if (step % 4 == 2 || step % 4 == 3) {
      i = 63-index;
    } else {
      i = index;
    }

    if (up){
        pixels[i].setColor(getColor(i));
        pixels[i].setBrightness(255);
    } else {
        pixels[i].setBrightness(0);
    }


    for (int f = 0; f < faces.size(); f++) {
       faces[f].get().setPixel(pixels[i]);
    }

    index++;

    cube.render();
}
