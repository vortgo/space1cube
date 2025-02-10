#include "cube.h"
#include <new>

// Буфер для объекта Cube (выравнивание по требованиям Cube)
alignas(Cube) uint8_t cubeBuffer[sizeof(Cube)];
// Глобальный указатель на Cube
Cube *cube = nullptr;
std::vector<Matrix> matrices;
Adafruit_NeoPixel displays[NUM_MATRICES] = {
    Adafruit_NeoPixel(NUM_LEDS, PIN_1, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(NUM_LEDS, PIN_2, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(NUM_LEDS, PIN_3, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(NUM_LEDS, PIN_4, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(NUM_LEDS, PIN_5, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(NUM_LEDS, PIN_6, NEO_GRB + NEO_KHZ800)};

void initCube()
{
    for (int i = 0; i < NUM_MATRICES; ++i)
    {
        matrices.push_back(Matrix(displays[i], WIDTH, HEIGHT, true));
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
        unsigned long currentTime = millis();
        unsigned long delta = currentTime - lastTime;
        lastTime = currentTime;
        activeEffect->tick(*this, delta);
    }
}
