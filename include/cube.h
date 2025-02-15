#ifndef CUBE_H
#define CUBE_H

#include "matrix.h"

// Перечисление для обозначения граней куба
enum class CubeFace
{
    FRONT,
    BACK,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

enum class CubeEffects
{
    SYMBOL,
    HEART,
    FAILING_STAR,
    SOUND_PEAKS,
    SPIRAL,
    FADE_PIXEL
};

enum class Color : uint32_t
{
    BLUE = 0x0000FF, // Синий
    CYAN = 0x00FFFF, // Циан
    GREEN = 0x00FF00, // Зеленый
    YELLOW = 0xFFFF00, // Желтый
    CHARTREUSE = 0x7FFF00, // Хартрез
    LIME = 0x00FF80, // Лайм
    SPRING_GREEN = 0x00FF7F, // Весенний зеленый
    ORANGE = 0xFFA500, // Оранжевый
    RED = 0xFF0000, // Красный
    VIOLET = 0x8A2BE2 // Фиолетовый
};

inline float getRandomFloatInRange(const float min, const float max)
{
    return min + (static_cast<float>(rand()) / RAND_MAX) * (max - min);
}

inline int getRandonIntInRange(const int x, const int y)
{
    return std::rand() % (y - x + 1) + x;
}

class Cube;

class Effect
{
public:
    void tick(Cube& cube)
    {
        unsigned long currentTime = millis();
        unsigned long delta = currentTime - lastTime;
        if (delta < renderTime)
        {
            return;
        }
        lastTime = currentTime;
        render(cube, delta);
    }

    virtual void render(Cube& cube, unsigned long deltaTime);

private:
    unsigned long lastTime = 0;
    unsigned long renderTime = 10;
};

class EffectFadePixels : public Effect
{
public:
    EffectFadePixels();

    void render(Cube& cube, unsigned long deltaTime) override;

    int minPeriod = 100;
    int minCooldown = 100;
    int maxPeriod = 500;
    int maxCooldown = 500;

    struct FadePixel: Pixel
    {
        int period;
        bool up = true;
        bool paused = false;
        int cooldownAccumulate = 0;
        int cooldown = 0;
        int accumulatedTime = 0 ;
    };

    FadePixel initPixel();

private:
    std::vector<FadePixel> pixels;
};

class EffectSpiral : public Effect
{
public:
    EffectSpiral();

    unsigned long pauseForPixel = 70;

    void render(Cube& cube, unsigned long deltaTime) override;

private:
    std::vector<Pixel> pixels;
    unsigned long lastTime = 0;
    bool up = true;
    int index = 0;
    int step = 0;
};

class EffectSoundLevel : public Effect
{
public:
    EffectSoundLevel();

    int basePeriod = 500;
    int pauseTime = 500;


    //публичные параметры для конфигурации эффекта  и методы
    void render(Cube& cube, unsigned long deltaTime) override;

private:
    //внутрення реализация
    void renderOneFace(Matrix& face, int i, uint32_t highestColor, uint32_t lowestColor);

    struct Peak
    {
        int hight;
        int low;
        bool increase;
        int currentHeight;
        int period;
        int pauseTime;
        unsigned long accumulatedPause;
    };

    unsigned long accumulatedTime = 0;

    std::vector<Peak> peaks;

    uint32_t colorFaces[6][2] = {
        {0xFF0000, 0x00FF00}, // Красный и зеленый
        {0xffb700, 0x00ffd0}, // оранжевый и бирюзовый
        {0x15ff00, 0x002aff}, // зеленый и синий
        {0x05ecfc, 0xfc05f4}, // бирюзовый  и фиолетоывй
        {0x2605fc, 0xfc0505}, // синий и красный
        {0x2605fc, 0xfc05d3} // синий и фиолетовый
    };
};

class EffectFallingStar : public Effect
{
public:
    // Конструктор
    EffectFallingStar();

    // Настройки эффекта
    uint32_t colorFront = 0xFF5733;
    uint32_t colorBack = 0x33FF57;
    uint32_t colorTop = 0x3357FF;
    uint32_t colorBottom = 0xFF33A1;
    uint32_t colorLeft = 0xF5A623;
    uint32_t colorRight = 0x8B33FF;

    std::vector<uint32_t> colors = {
        colorFront, colorBack, colorLeft, colorRight, colorTop, colorBottom
    };

    float fallSpeed = 0.03f; // Скорость падения
    uint8_t tailLength = 3; // Длина хвоста
    unsigned long minDelay = 50; // Минимальная задержка между звездами
    unsigned long maxDelay = 600; // Максимальная задержка между звездами

    void render(Cube& cube, unsigned long deltaTime) override;

private:
    struct Star
    {
        int y; // Текущая вертикальная позиция звезды (0..8+tailLength)
        float speed; // Время между шагами в секундах (например, 0.5 для 500 мс)
        unsigned long lastUpdate; // Счетчик времени, накопленный для движения звезды
        bool active; // Флаг: активна ли звезда (есть на экране)
    };

    // Массив для хранения звезд
    std::vector<Star> stars;

    // Таймеры для каждого столбца (время задержки для следующей звезды)
    long columnTimers[8] = {0};

    void addStar(int column, bool active); // Добавить новую звезду в конкретный столбец
    void initializeColumnDelay(int column);
};


class EffectBeatingHeart : public Effect
{
public:
    // Метод обновления эффекта, deltaTime – прошедшее время в секундах с прошлого обновления.
    void render(Cube& cube, unsigned long deltaTime) override;
    float beatPeriod = 1.0f; // Период полного цикла биения
    uint32_t color = 0x00FF00;

private:
    void setOnFace(Matrix& f);
    float accumulatedTime; // Накопленное время в секундах
    float contractionFraction = 0.3f; // Доля цикла для фазы сокращения (систола)
    float expansionFraction = 0.4f; // Доля цикла для фазы расширения (диастола)
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

class EffectSymbol : public Effect
{
public:
    void render(Cube& cube, unsigned long deltaTime) override;

    void print(std::string s, uint32_t c)
    {
        symbol = s;
        color = c;
        printed = false;
    }

private:
    uint8_t width = 8, height = 8;
    void drawCharacter(Cube& cube, const std::string& character, uint32_t color);
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
class Cube
{
public:
    EffectSymbol effectSymbol = EffectSymbol();
    EffectBeatingHeart breathingHeart = EffectBeatingHeart();
    EffectFallingStar fallingStar = EffectFallingStar();
    EffectSoundLevel soundLevel = EffectSoundLevel();
    EffectSpiral effectSpiral = EffectSpiral();
    EffectFadePixels fadePixels = EffectFadePixels();

    void setActiveEffect(CubeEffects e)
    {
        switch (e)
        {
        case CubeEffects::SYMBOL:
            activeEffect = &effectSymbol;
            break;
        case CubeEffects::HEART:
            activeEffect = &breathingHeart;
            break;
        case CubeEffects::FAILING_STAR:
            activeEffect = &fallingStar;
            break;
        case CubeEffects::SOUND_PEAKS:
            activeEffect = &soundLevel;
            break;
        case CubeEffects::SPIRAL:
            activeEffect = &effectSpiral;
            break;
        case CubeEffects::FADE_PIXEL:
            activeEffect = &fadePixels;
            break;
        default:
            break;
        }
    }


    // Грани куба (ссылки на матрицы)
    Matrix& front;
    Matrix& back;
    Matrix& left;
    Matrix& right;
    Matrix& top;
    Matrix& bottom;

    /**
     * @brief Конструктор куба.
     * @param front Ссылка на матрицу передней грани.
     * @param back Ссылка на матрицу задней грани.
     * @param left Ссылка на матрицу левой грани.
     * @param right Ссылка на матрицу правой грани.
     * @param top Ссылка на матрицу верхней грани.
     * @param bottom Ссылка на матрицу нижней грани.
     */
    Cube(Matrix& front,
         Matrix& back,
         Matrix& left,
         Matrix& right,
         Matrix& top,
         Matrix& bottom);

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
    std::vector<std::reference_wrapper<Matrix>> getFaces()
    {
        return {front, back, left, right, top, bottom};
    }

private:
    /**
     * @brief Вспомогательный метод для получения ссылки на матрицу по заданной грани.
     * @param face Грань куба.
     * @return Ссылка на соответствующую матрицу.
     */
    Matrix& getMatrix(CubeFace face);

    Effect* activeEffect = nullptr;
};

extern Cube* cube;
void initCube();

#endif // CUBE_H
