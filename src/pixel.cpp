#include "pixel.h"

Pixel::Pixel(int x, int y, uint32_t color, uint8_t brightness)
    : x(x), y(y), color(color), brightness(brightness) {
   
}

int Pixel::getX() const {
    return x;
}

int Pixel::getY() const {
    return y;
}

uint32_t Pixel::getColor() const {
    return color;
}

uint8_t Pixel::getBrightness() const {
    return brightness;
}

void Pixel::setX(int x) {
    this->x = x;
}

void Pixel::setY(int y) {
    this->y = y;
}

void Pixel::setColor(uint32_t color) {
    this->color = color;
}

void Pixel::setBrightness(uint8_t brightness) {
    this->brightness = brightness;
}

