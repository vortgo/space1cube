#ifndef CUBE_H
#define CUBE_H

#include "matrix.h"

// Перечисление для обозначения граней куба
enum class CubeFace {
    FRONT,
    BACK,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

enum class CubeEffects {
    SYMBOL,
    HEART,
    FALING_STAR,
    BLINK_CUBE
};

class Cube;
class Effect {
    public:
    void tick(Cube &cube, unsigned long deltaTime) {
        if (deltaTime < renderTime) {
            return;
        }
        render(cube, deltaTime);
    }

    virtual void render(Cube &cube, unsigned long deltaTime);

    private: 
       unsigned long renderTime = 10;
};

class EffectFallingStar : public Effect {
    public:
        // Конструктор
        EffectFallingStar();
    
        // Настройки эффекта
        uint32_t colorFront = 0xFF5733;  // Красный (R: 255, G: 87, B: 51)
        uint32_t colorBack = 0x33FF57;   // Зелёный (R: 51, G: 255, B: 87)
        uint32_t colorTop = 0x3357FF;    // Синий (R: 51, G: 87, B: 255)
        uint32_t colorBottom = 0xFF33A1; // Розовый (R: 255, G: 51, B: 161)
        uint32_t colorLeft = 0xF5A623;   // Оранжевый (R: 245, G: 166, B: 35)
        uint32_t colorRight = 0x8B33FF;  // Фиолетовый (R: 139, G: 51, B: 255)

        std::vector<uint32_t> colors =  {
           colorFront, colorBack, colorLeft, colorRight, colorTop, colorBottom
        };

        float fallSpeed = 0.03f;              // Скорость падения
        uint8_t tailLength = 3;              // Длина хвоста
        unsigned long minDelay = 50;        // Минимальная задержка между звездами
        unsigned long maxDelay = 600;        // Максимальная задержка между звездами
    
        void render(Cube &cube, unsigned long deltaTime) override;
    
    private:
    struct Star {
        int y;                      // Текущая вертикальная позиция звезды (0..8+tailLength)
        float speed;                // Время между шагами в секундах (например, 0.5 для 500 мс)
        unsigned long lastUpdate;   // Счетчик времени, накопленный для движения звезды
        bool active;                // Флаг: активна ли звезда (есть на экране)
    };
    
        // Массив для хранения звезд
        std::vector<Star> stars;
    
        // Таймеры для каждого столбца (время задержки для следующей звезды)
        long columnTimers[8] = {0};
    
        void addStar(int column, bool active);            // Добавить новую звезду в конкретный столбец
        void initializeColumnDelay(int column);
    };
    

class EffectBeatingHeart: public Effect {
    public:  
        // Метод обновления эффекта, deltaTime – прошедшее время в секундах с прошлого обновления.
        void render(Cube &cube, unsigned long deltaTime) override ;
        float beatPeriod = 1.0f;            // Период полного цикла биения
        uint32_t color = 0x00FF00;
    
    private:
        void setOnFace(Matrix &f);
        float accumulatedTime;       // Накопленное время в секундах
        float contractionFraction = 0.3f;   // Доля цикла для фазы сокращения (систола)
        float expansionFraction  = 0.4f;     // Доля цикла для фазы расширения (диастола)
    // Определяем шаблон для большого сердца – заполненный рисунок 8×8
    int bigHeart[8][8] = {
        {0, 1, 1, 0, 0, 1, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {0, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };
    
    float blend;
};    

class EffectSymbol: public Effect {
    public:
    void render(Cube &cube, unsigned long deltaTime) override ;

    void print(std::string s, uint32_t c) {
        symbol = s;
        color = c;
        printed = false;
    }

    private:

    uint8_t width = 8, height = 8;
    void drawCharacter(Cube &cube, const std::string &character, uint32_t color);
    std::string symbol = "A";
    uint32_t color = 0x00FF00;
    bool printed = false;
};

/**
 * @brief Класс Cube объединяет 6 матриц – по одной для каждой грани куба.
 *
 * Грани куба хранятся как публичные ссылки на объекты Matrix. 
 * Метод render() обновляет все матрицы, а метод setPixel() позволяет
 * установить для конкретного пикселя (с указанной яркостью) значение, указав грань через CubeFace.
 */
class Cube {
public:

    EffectSymbol effectSymbol = EffectSymbol();
    EffectBeatingHeart breathingHeart = EffectBeatingHeart();
    EffectFallingStar fallingStar = EffectFallingStar();

    void setActiveEffect(CubeEffects e){
        switch (e)
        {
        case CubeEffects::SYMBOL:
            activeEffect = &effectSymbol;
            break;
        case CubeEffects::HEART:
            activeEffect = &breathingHeart;
            break;
        case CubeEffects::FALING_STAR:
            activeEffect = &fallingStar;
        break;
        default:
            break;
        }
    }
    

    // Грани куба (ссылки на матрицы)
    Matrix &front;
    Matrix &back;
    Matrix &left;
    Matrix &right;
    Matrix &top;
    Matrix &bottom;

    /**
     * @brief Конструктор куба.
     * @param front Ссылка на матрицу передней грани.
     * @param back Ссылка на матрицу задней грани.
     * @param left Ссылка на матрицу левой грани.
     * @param right Ссылка на матрицу правой грани.
     * @param top Ссылка на матрицу верхней грани.
     * @param bottom Ссылка на матрицу нижней грани.
     */
    Cube(Matrix &front,
         Matrix &back,
         Matrix &left,
         Matrix &right,
         Matrix &top,
         Matrix &bottom);

    void tick();

    /**
     * @brief Рендерит (обновляет) все грани куба.
     */
    void render();

    void clear();

    /**
     * @brief Устанавливает для пикселя на указанной грани куба заданный цвет и яркость.
     * @param face Грань куба (CubeFace).
     * @param x Координата X в матрице.
     * @param y Координата Y в матрице.
     * @param color Цвет пикселя в формате 0xRRGGBB.
     * @param brightness Яркость пикселя (0–255).
     */
    void setPixel(CubeFace face, uint8_t x, uint8_t y, uint32_t color, uint8_t brightness = 255);

    // отдать матрицы для итерации 
    // std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    // // Итерируем по вектору и вызываем метод display() для каждой грани
    // for (auto &face : faces) {
    //    
    // }
     std::vector<std::reference_wrapper<Matrix>> getFaces() {
        return {front, back, left, right, top, bottom};
    }

private:
    /**
     * @brief Вспомогательный метод для получения ссылки на матрицу по заданной грани.
     * @param face Грань куба.
     * @return Ссылка на соответствующую матрицу.
     */
    Matrix &getMatrix(CubeFace face);

    Effect* activeEffect = nullptr;
};

extern Cube* cube;
void initCube();

#endif // CUBE_H
