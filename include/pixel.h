#ifndef PIXEL_H
#define PIXEL_H

#include <cstdint> 

class Pixel {
public:
    /**
     * @brief Конструктор по умолчанию.
     * @param x Координата X.
     * @param y Координата Y.
     * @param color Цвет пикселя в формате 0xRRGGBB (по умолчанию черный).
     * @param brightness Яркость пикселя (0-255, по умолчанию 255).
     */
    Pixel(int x = 0, int y = 0, uint32_t color = 0x000000, uint8_t brightness = 255);

    // Геттеры
    int getX() const;
    int getY() const;
    uint32_t getColor() const;
    uint8_t getBrightness() const;

    // Сеттеры
    void setX(int x);
    void setY(int y);
    void setColor(uint32_t color);
    void setBrightness(uint8_t brightness);

private:
    int x;             // Координата X
    int y;             // Координата Y
    uint32_t color;    // Цвет пикселя в формате 0xRRGGBB
    uint8_t brightness; // Яркость пикселя (0-255)
};

#endif // PIXEL_H
