#ifndef MATRIX_SETUP_H
#define MATRIX_SETUP_H

#include <FastLED.h>

// Размеры матрицы
#define WIDTH 8
#define HEIGHT 8
#define NUM_LEDS (WIDTH * HEIGHT)
#define NUM_MATRICES 6

// Пины подключения для каждой матрицы
#define PIN_1 15
#define PIN_2 18
#define PIN_3 19
#define PIN_4 25
#define PIN_5 33
#define PIN_6 13

// Массивы LED для каждой матрицы
extern CRGB leds1[NUM_LEDS];
extern CRGB leds2[NUM_LEDS];
extern CRGB leds3[NUM_LEDS];
extern CRGB leds4[NUM_LEDS];
extern CRGB leds5[NUM_LEDS];
extern CRGB leds6[NUM_LEDS];

// Указатели на массивы для удобного доступа
extern CRGB* ledsArrays[NUM_MATRICES];

// Флаги яркости для каждой матрицы (для turnOn/turnOff)
extern uint8_t matrixBrightness[NUM_MATRICES];

#endif
