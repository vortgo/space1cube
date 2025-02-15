#ifndef MATRIX_H
#define MATRIX_H

#include "matrix_setup.h"
#include "pixel.h"
#include <Adafruit_NeoPixel.h>
#include <string>
#include <unordered_map>
#include <vector>

using Pattern = std::array<uint8_t, 7>;
extern const std::unordered_map<std::string, Pattern> charPatterns;

class Matrix {
public:
    Matrix(Adafruit_NeoPixel &display, uint8_t width, uint8_t height, bool on);

    void setPixel(uint8_t x, uint8_t y, uint32_t color, uint8_t brightness = 255);
    void setPixel(uint8_t x, uint8_t y, uint32_t color, float brightness);
    void setPixel(const Pixel& p);
    
    Pixel& getPixel(uint8_t x, uint8_t y);

    void render();

    Adafruit_NeoPixel &display;

    void turnOn();
    void turnOff();
    void clear();

    void clearColumn(uint8_t x){
        for (uint8_t y = 0; y < height; y++){
            setPixel(x, y, 0x000000, 0.0f);
        }
    }

    float maxBrightness = 1.0f;
private:
    uint8_t width, height;

    std::vector<Pixel> pixels; 

    uint16_t getIndex(uint8_t x, uint8_t y);

    bool on = true;
};

#endif
