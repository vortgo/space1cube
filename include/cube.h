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
    FADE_PIXEL,
    DICE,
    ROTATE,
    ROMB,
    AURORA,
    LAVA_LAMP,
    GRAVITY,
    PARTICLES,
    SNAKE,
    DYNAMIC_GROUPS,
    CYBER_GHOST,
    SPIRIT_WIND,
    VORTEX,
    PACMAN,
    RAINDROP,
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
    virtual ~Effect() = default;

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

    virtual void render(Cube& cube, unsigned long deltaTime) = 0;

private:
    unsigned long lastTime = 0;
    unsigned long renderTime = 10;
};

class EffectRaindropRipples : public Effect {
public:
    void render(Cube& cube, unsigned long deltaTime) override;

private:
    struct Ripple {
        int x, y;
        float radii[3];
        float time;
        uint32_t color;
    };

    Ripple ripple;
    bool rippleActive = false;
    float timeSinceLastDrop = 0.0f;
    float dropInterval = 3.0f;
    uint32_t getRandomColor();
    uint32_t interpolateColor(uint32_t baseColor, float distanceFactor);
    void spawnRaindrop();
};



class EffectPacman : public Effect {
public:
    void render(Cube& cube, unsigned long deltaTime) override;

private:
    struct Pellet {
        int x, y;
        uint32_t color;
    };

    float mouthTimer = 0.0f;
    bool mouthOpen = true;
    float pelletSpawnRate = 1.5f; // Замедлена скорость появления еды
    float timeSinceLastPellet = 0.0f;
    std::vector<Pellet> pellets;
    uint32_t yellow = 0xFFFF00;
    uint32_t black = 0x000000;

    void drawPacman(Cube& cube);
    void spawnPellet();
};

class EffectVortex : public Effect {
public:
    void render(Cube& cube, unsigned long deltaTime) override;

private:
    struct Particle {
        float x, y;
        float angle;
        float radius;
        float angularSpeed;
        uint32_t color;
        float life;
        int cycleCount;
    };

    std::vector<Particle> particles;
    float spawnRate = 0.08f;
    float timeSinceLastSpawn = 0;
    float fadeRate = 0.98f;
    float expansionRate = 0.07f;
    float colorChangeThreshold = 2.5f * M_PI;
    uint32_t baseColor;
    std::vector<uint32_t> colorShades;
    int currentCycle = 0;
    float colorTransitionProgress = 0.0f;

    void generateNewColorScheme();
    uint32_t getShade(int index);
    uint32_t getRandomVortexColor();
    uint32_t interpolateColors(uint32_t color1, uint32_t color2, float t);
};

class EffectSpiritWind : public Effect {
public:
    void render(Cube& cube, unsigned long deltaTime) override;

private:
    struct Particle {
        float x, y;
        float vx, vy;
        uint32_t color;
        float life;
        float initialDistance;
    };

    std::vector<Particle> particles;
    float spawnRate = 0.05f; // Уменьшенная частота появления новых частиц
    float timeSinceLastSpawn = 0;
    float fadeRate = 0.97f;  // Коэффициент затухания
    float waveSpeed = 0.2f;  // Скорость распространения волн

    uint32_t getRandomBlueShade();
};

class EffectCyberGhost : public Effect {
public:
    EffectCyberGhost();
    void render(Cube& cube, unsigned long deltaTime) override;

private:
    struct Flash {
        std::vector<std::pair<int, int>> particles;
        uint32_t color;
        unsigned long startTime;
        unsigned long duration;
        float brightness;
    };

    std::vector<Flash> flashes;
    unsigned long lastUpdateTime;
    void addNewFlash();
    uint32_t getRandomColor();
    std::vector<std::pair<int, int>> getAvailablePixels();
    void cleanupExpiredFlashes();
};

class EffectDynamicGroups : public Effect {
public:
    EffectDynamicGroups();
    void render(Cube& cube, unsigned long deltaTime) override;

private:
    struct Group {
        std::vector<std::pair<int, int>> pixels;
        uint32_t color;
        unsigned long startTime;
        unsigned long duration;
        float brightness;
    };

    std::vector<Group> groups;
    unsigned long lastUpdateTime;
    void addNewGroup();
    uint32_t getRandomColor();
    std::vector<std::pair<int, int>> getAvailablePixels();
    void cleanupExpiredGroups();
};



class EffectSnake : public Effect {
public:
    EffectSnake();
    void render(Cube& cube, unsigned long deltaTime) override;

private:
    struct Segment {
        int x, y;
        uint32_t color;
    };

    std::vector<Segment> snake;
    int foodX, foodY;
    uint32_t foodColor;
    int directionX, directionY;
    unsigned long lastUpdateTime;
    unsigned long moveInterval;

    void resetGame();
    void move();
    void generateFood();
    bool checkCollision(int x, int y);
    void updateDirection();
};


class EffectPhysicsParticles : public Effect {
public:
    EffectPhysicsParticles(int numParticles = 5, float baseSpeed = 1.2f);
    void render(Cube& cube, unsigned long deltaTime) override;

private:
    struct Particle {
        float x, y;
        float vx, vy;
        uint8_t r, g, b;
        bool active;
    };

    std::vector<Particle> particles;
    int maxParticles;
    float baseSpeed;

    void initParticles();
    void updateParticles(unsigned long deltaTime);
    void handleCollisions();
    uint32_t randomColor();
};

class EffectGravityParticles : public Effect {
public:
    void render(Cube& cube, unsigned long deltaTime) override;

private:
    struct Particle {
        float x, y;
        float vx, vy;
    };

    std::vector<Particle> particles;
    int maxParticles = 10;
    float gravityStrength = 0.01f; // Замедленная скорость притяжения
    float friction = 0.95f; // Больше трения для плавности
    float explosionForce = 0.1f; // Уменьшена сила взрыва
    bool exploding = false;
    bool gathered = false;
    unsigned long effectTime = 0;
    unsigned long explosionDuration = 2500; // 2.5 сек разлет
    unsigned long gatherDuration = 4000; // 4 сек притяжение
    uint8_t baseR, baseG, baseB;

    void initParticles();
    void updateParticles();
    void pickRandomBaseColor();
    bool allParticlesInCenter();
};

class EffectAurora : public Effect {
public:
    float period = 5000;
    void render(Cube& cube, unsigned long deltaTime) override;

private:
    float accumulatedTime = 0.0f;
    unsigned long effectTime = 0;
};

class EffectRomb : public Effect
{
public:
    float period = 1500;
    void render(Cube& cube, unsigned long deltaTime) override;

private:
    uint32_t interpolateColor(uint32_t colorStart, uint32_t colorEnd, float t);
    float accumulatedTime = 0.0f;
    unsigned long effectTime = 0;
};

class EffectRotate : public Effect
{
public:
    EffectRotate();
    void render(Cube& cube, unsigned long deltaTime) override;
    float period = 15000;
    int effect = 0;
private:
    std::vector<std::reference_wrapper<Effect>>effects;

    unsigned long accumulatedTime = 0;
};

class EffectLavaLamp : public Effect {
public:
    float period = 7000; // Плавный цикл эффекта
    void render(Cube& cube, unsigned long deltaTime) override;

private:
    float accumulatedTime = 0.0f;
    unsigned long effectTime = 0;
};

class EffectDice final : public Effect
{
public:
    EffectDice();
    void render(Cube& cube, unsigned long deltaTime) override;

    std::vector<Color> colors = {
        Color::BLUE, Color::CYAN, Color::GREEN, Color::ORANGE, Color::RED, Color::VIOLET
    };
    float period = 20;
    float maxSteps = 50;
    unsigned long accumulatedTime = 0;

private:

    struct Face
    {
        int value = 0 ;
        int startColorIndex;
        int endColorIndex;
        int steps = 0;
    };
    void changeColors(Face &f);

    std::vector<Face> faces;

    void renderValue(Matrix &m, int value, uint32_t color);
};

class EffectFadePixels : public Effect
{
public:
    EffectFadePixels();

    void render(Cube& cube, unsigned long deltaTime) override;

    int basePeriod = 500;
    int baseCooldown = 200;

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
    EffectDice effectDice = EffectDice();
    EffectRotate effectRotate = EffectRotate();
    EffectRomb effectRomb = EffectRomb();
    EffectAurora effectAurora = EffectAurora();
    EffectLavaLamp effectLavaLamp = EffectLavaLamp();
    EffectGravityParticles effectGravity = EffectGravityParticles();
    EffectPhysicsParticles effectParticles = EffectPhysicsParticles();
    EffectSnake effectSnake = EffectSnake();
    EffectDynamicGroups effectDynamicGroups = EffectDynamicGroups();
    EffectCyberGhost effectCyberGhost = EffectCyberGhost();
    EffectSpiritWind effectSpiritWind = EffectSpiritWind();
    EffectVortex effectVortex = EffectVortex();
    EffectPacman effectPacman = EffectPacman();
    EffectRaindropRipples effectRaindropRipples = EffectRaindropRipples();

    float voltage = 0.0f;

    void setVoltage(float v)
    {
        voltage = v;
    }

    int getBatteryValue()
    {
        // Границы напряжения
        const float minVoltage = 1.17f;
        const float maxVoltage = 1.82f;

        // Ограничение напряжения в допустимых пределах
        float clampedVoltage = voltage;
        if (clampedVoltage < minVoltage) clampedVoltage = minVoltage;
        if (clampedVoltage > maxVoltage) clampedVoltage = maxVoltage;

        // Вычисление процента заряда
        float percent = ((clampedVoltage - minVoltage) / (maxVoltage - minVoltage)) * 100.0f;

        return static_cast<int>(percent);
    }

    void setActiveEffect(CubeEffects e)
    {
        switch (e)
        {
        case CubeEffects::SYMBOL:
            activeEffect = &effectSymbol;
            Serial.println("activeEffect SYMBOL");
            break;
        case CubeEffects::HEART:
            activeEffect = &breathingHeart;
            Serial.println("activeEffect HEART");
            break;
        case CubeEffects::FAILING_STAR:
            activeEffect = &fallingStar;
            Serial.println("activeEffect FAILING_STAR");
            break;
        case CubeEffects::SOUND_PEAKS:
            activeEffect = &soundLevel;
            Serial.println("activeEffect SOUND_PEAKS");
            break;
        case CubeEffects::SPIRAL:
            activeEffect = &effectSpiral;
            Serial.println("activeEffect SPIRAL");
            break;
        case CubeEffects::FADE_PIXEL:
            activeEffect = &fadePixels;
            Serial.println("activeEffect FADE_PIXEL");
            break;
        case CubeEffects::DICE:
            activeEffect = &effectDice;
            Serial.println("activeEffect DICE");
            break;
        case CubeEffects::ROTATE:
            activeEffect = &effectRotate;
            Serial.println("activeEffect ROTATE");
            break;
        case CubeEffects::ROMB:
            activeEffect = &effectRomb;
            Serial.println("activeEffect ROMB");
            break;
        case CubeEffects::AURORA:
            activeEffect = &effectAurora;
            Serial.println("activeEffect AURORA");
            break;
        case CubeEffects::LAVA_LAMP:
            activeEffect = &effectLavaLamp;
            Serial.println("activeEffect LAVA_LAMP");
            break;
        case CubeEffects::GRAVITY:
            activeEffect = &effectGravity;
            Serial.println("activeEffect GRAVITY");
            break;
        case CubeEffects::PARTICLES:
            activeEffect = &effectParticles;
            Serial.println("activeEffect PARTICLES");
            break;
        case CubeEffects::SNAKE:
            activeEffect = &effectSnake;
            Serial.println("activeEffect SNAKE");
            break;
        case CubeEffects::DYNAMIC_GROUPS:
            activeEffect = &effectDynamicGroups;
            Serial.println("activeEffect DYNAMIC_GROUPS");
            break;
        case CubeEffects::CYBER_GHOST:
            activeEffect = &effectCyberGhost;
            Serial.println("activeEffect CYBER_GHOST");
            break;
        case CubeEffects::SPIRIT_WIND:
            activeEffect = &effectSpiritWind;
            Serial.println("activeEffect SPIRIT_WIND");
            break;
        case CubeEffects::VORTEX:
            activeEffect = &effectVortex;
            Serial.println("activeEffect VORTEX");
            break;
        case CubeEffects::PACMAN:
            activeEffect = &effectPacman;
            Serial.println("activeEffect PACMAN");
            break;
        case CubeEffects::RAINDROP:
            activeEffect = &effectRaindropRipples;
            Serial.println("activeEffect RAINDROP");
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

    std::vector<std::reference_wrapper<Effect>> getEffectsForRotate()
    {
        return {breathingHeart,fallingStar, soundLevel, effectSpiral, fadePixels, effectDice, effectRomb, effectAurora, effectLavaLamp, effectGravity, effectParticles, effectSnake, effectDynamicGroups, effectCyberGhost, effectSpiritWind, effectVortex, effectPacman, effectRaindropRipples };
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


// Функция для преобразования HSV в RGB
inline void myHsvToRgb(float h, float s, float v, uint8_t &r, uint8_t &g, uint8_t &b) {
    float c = v * s;
    float x = c * (1 - fabs(fmod(h / 60.0f, 2) - 1));
    float m = v - c;

    float r1, g1, b1;
    if (h >= 0 && h < 60) {
        r1 = c; g1 = x; b1 = 0;
    } else if (h >= 60 && h < 120) {
        r1 = x; g1 = c; b1 = 0;
    } else if (h >= 120 && h < 180) {
        r1 = 0; g1 = c; b1 = x;
    } else if (h >= 180 && h < 240) {
        r1 = 0; g1 = x; b1 = c;
    } else if (h >= 240 && h < 300) {
        r1 = x; g1 = 0; b1 = c;
    } else {
        r1 = c; g1 = 0; b1 = x;
    }

    // Добавляем m, чтобы получить итоговые значения RGB
    r = static_cast<uint8_t>((r1 + m) * 255);
    g = static_cast<uint8_t>((g1 + m) * 255);
    b = static_cast<uint8_t>((b1 + m) * 255);
}

#endif // CUBE_H
