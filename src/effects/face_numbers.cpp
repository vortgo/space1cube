#include "cube.h"
#include "matrix.h"

EffectFaceNumbers::EffectFaceNumbers() {
    hueOffset = 0.0f;
}

uint32_t EffectFaceNumbers::hueToColor(float hue) {
    // Преобразование HSV в RGB (S=1, V=1)
    while (hue >= 360.0f) hue -= 360.0f;
    while (hue < 0.0f) hue += 360.0f;

    float c = 1.0f;
    float x = c * (1.0f - fabs(fmod(hue / 60.0f, 2.0f) - 1.0f));
    float m = 0.0f;

    float r, g, b;
    if (hue < 60) {
        r = c; g = x; b = 0;
    } else if (hue < 120) {
        r = x; g = c; b = 0;
    } else if (hue < 180) {
        r = 0; g = c; b = x;
    } else if (hue < 240) {
        r = 0; g = x; b = c;
    } else if (hue < 300) {
        r = x; g = 0; b = c;
    } else {
        r = c; g = 0; b = x;
    }

    uint8_t rr = (uint8_t)((r + m) * 255);
    uint8_t gg = (uint8_t)((g + m) * 255);
    uint8_t bb = (uint8_t)((b + m) * 255);

    return (rr << 16) | (gg << 8) | bb;
}

void EffectFaceNumbers::drawCharOnFace(Matrix& face, const std::string& character, uint32_t color) {
    int colOffset = 1;
    int rowOffset = 1;

    auto it = charPatterns.find(character);
    if (it == charPatterns.end()) {
        return;
    }

    const Pattern& charPattern = it->second;
    for (int r = 0; r < 7; r++) {
        uint8_t rowPattern = charPattern[r];
        for (int c = 0; c < 5; c++) {
            if (rowPattern & (1 << (4 - c))) {
                int x = c + colOffset;
                int y = r + rowOffset;
                face.setPixel(x, y, color);
            }
        }
    }
}

void EffectFaceNumbers::render(Cube& cube, unsigned long deltaTime) {
    cube.clear();

    if (colorCycle) {
        // Циклическая смена цветов - каждая грань со сдвигом 60 градусов
        float hue1 = hueOffset;
        float hue2 = hueOffset + 60.0f;
        float hue3 = hueOffset + 120.0f;
        float hue4 = hueOffset + 180.0f;
        float hue5 = hueOffset + 240.0f;
        float hue6 = hueOffset + 300.0f;

        drawCharOnFace(cube.front,  "1", hueToColor(hue1));
        drawCharOnFace(cube.back,   "2", hueToColor(hue2));
        drawCharOnFace(cube.left,   "3", hueToColor(hue3));
        drawCharOnFace(cube.right,  "4", hueToColor(hue4));
        drawCharOnFace(cube.top,    "5", hueToColor(hue5));
        drawCharOnFace(cube.bottom, "6", hueToColor(hue6));

        hueOffset += cycleSpeed * deltaTime;
        if (hueOffset >= 360.0f) {
            hueOffset -= 360.0f;
        }
    } else {
        // Статические цвета как при запуске
        drawCharOnFace(cube.front,  "1", 0xFF0000);  // Красный
        drawCharOnFace(cube.back,   "2", 0x00FF00);  // Зелёный
        drawCharOnFace(cube.left,   "3", 0x0000FF);  // Синий
        drawCharOnFace(cube.right,  "4", 0xFFFF00);  // Жёлтый
        drawCharOnFace(cube.top,    "5", 0xFF00FF);  // Пурпурный
        drawCharOnFace(cube.bottom, "6", 0x00FFFF);  // Голубой
    }

    cube.render();
}
