#ifndef MATRIX_SETUP_H
#define MATRIX_SETUP_H

#include <Adafruit_NeoPixel.h>
#include <unordered_map>

// Размеры матрицы
#define WIDTH 8
#define HEIGHT 8
#define NUM_LEDS (WIDTH * HEIGHT)
#define NUM_MATRICES 6

// Пины подключения для каждой матрицы
// #define PIN_1 5
#define PIN_1 15
#define PIN_2 18
#define PIN_3 19
#define PIN_4 25
#define PIN_5 33
#define PIN_6 32

#endif
