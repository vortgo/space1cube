#include "cube.h"
#include <new>
#include <cstdlib>
#include <ctime>

// Буфер для объекта Cube (выравнивание по требованиям Cube)
alignas(Cube) uint8_t cubeBuffer[sizeof(Cube)];
// Глобальный указатель на Cube
Cube *cube = nullptr;
std::vector<Matrix> matrices;

// Определение массивов LED для FastLED
CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];
CRGB leds3[NUM_LEDS];
CRGB leds4[NUM_LEDS];
CRGB leds5[NUM_LEDS];
CRGB leds6[NUM_LEDS];

// Указатели на массивы для удобного доступа
CRGB* ledsArrays[NUM_MATRICES] = {leds1, leds2, leds3, leds4, leds5, leds6};

// Флаги яркости для каждой матрицы
uint8_t matrixBrightness[NUM_MATRICES] = {255, 255, 255, 255, 255, 255};

void initCube()
{
    std::srand(std::time(0));

    // Инициализация FastLED для каждой матрицы
    // FastLED использует RMT на ESP32, что решает проблему с WiFi прерываниями
    FastLED.addLeds<WS2812B, PIN_1, GRB>(leds1, NUM_LEDS);
    FastLED.addLeds<WS2812B, PIN_2, GRB>(leds2, NUM_LEDS);
    FastLED.addLeds<WS2812B, PIN_3, GRB>(leds3, NUM_LEDS);
    FastLED.addLeds<WS2812B, PIN_4, GRB>(leds4, NUM_LEDS);
    FastLED.addLeds<WS2812B, PIN_5, GRB>(leds5, NUM_LEDS);
    FastLED.addLeds<WS2812B, PIN_6, GRB>(leds6, NUM_LEDS);

    // Глобальная яркость FastLED
    FastLED.setBrightness(255);

    // Очищаем все LED
    FastLED.clear();
    FastLED.show();

    // Создаём матрицы с указателями на LED массивы
    for (int i = 0; i < NUM_MATRICES; ++i)
    {
        matrices.push_back(Matrix(ledsArrays[i], i, WIDTH, HEIGHT, true));
    }

    // Конструируем Cube в заранее выделенном буфере с помощью placement new
    cube = new (cubeBuffer) Cube(
        matrices[3], matrices[2], matrices[4],
        matrices[0], matrices[5], matrices[1]);
}

Cube::Cube(Matrix &front,
           Matrix &back,
           Matrix &left,
           Matrix &right,
           Matrix &top,
           Matrix &bottom)
    : front(front), back(back), left(left), right(right), top(top), bottom(bottom)
{
    // Дополнительная инициализация, если требуется
}

void Cube::clear(){
    front.clear();
    back.clear();
    left.clear();
    right.clear();
    top.clear();
    bottom.clear();
}

void Cube::render()
{
    // Вызываем render() для каждой матрицы (грани куба)
    front.render();
    back.render();
    left.render();
    right.render();
    top.render();
    bottom.render();

    // Один вызов FastLED.show() для всех матриц сразу
    // FastLED использует RMT, что обеспечивает стабильную работу с WiFi
    FastLED.show();
}

Matrix &Cube::getMatrix(CubeFace face)
{
    switch (face)
    {
    case CubeFace::FRONT:
        return front;
    case CubeFace::BACK:
        return back;
    case CubeFace::LEFT:
        return left;
    case CubeFace::RIGHT:
        return right;
    case CubeFace::TOP:
        return top;
    case CubeFace::BOTTOM:
        return bottom;
    default:
        return front; // Резервный вариант (такой ситуации возникнуть не должно)
    }
}

void Cube::setPixel(CubeFace face, uint8_t x, uint8_t y, uint32_t color, uint8_t brightness)
{
    // Получаем нужную матрицу по грани куба и устанавливаем пиксель
    Matrix &m = getMatrix(face);
    m.setPixel(x, y, color, brightness);
}

unsigned long lastTime;

void Cube::tick()
{
    if (activeEffect != nullptr)
    {
        activeEffect->tick(*this);
    }
}
