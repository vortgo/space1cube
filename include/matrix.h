#ifndef MATRIX_H
#define MATRIX_H

#include "matrix_setup.h"
#include <Adafruit_NeoPixel.h>
#include <string>
#include <unordered_map>
#include <vector>

using Pattern = std::array<uint8_t, 7>;
extern const std::unordered_map<std::string, Pattern> charPatterns;

class Matrix {
public:
    Matrix(Adafruit_NeoPixel &display, uint8_t width, uint8_t height);

    void setPixel(uint8_t x, uint8_t y, uint32_t color, uint8_t brightness = 255);

    void clear() {
        display.clear();
        display.show();
    }

    void show() {
        display.show();
    }

    void drawCharacter(const std::string &character, uint32_t color);
    Adafruit_NeoPixel &display;

private:
    uint8_t width, height;

    uint16_t getIndex(uint8_t x, uint8_t y);
};

void initMatrices();
extern std::vector<Matrix> matrices;

#endif
