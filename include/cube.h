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
    RAINDROP,
    PLASMA,
    MATRIX_RAIN,
    GAME_OF_LIFE,
    FIREWORKS,
    STARFIELD,
    LIGHTNING,
    BOUNCING_BALLS,
    RIPPLE_POND,
    FIREFLIES,
    HEARTBEAT_PULSE,
    COMET_TRAIL,
    SPARKLE,
    CORNER_PULSE,
    SCAN_LINE,
    PIXEL_SORT,
    GROWING_SQUARES,
    RANDOM_WALK,
    TETRIS_FALL,
    FACE_NUMBERS,
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

    float dropInterval = 3.0f;  // Интервал между каплями (сек)

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
    uint32_t getRandomColor();
    uint32_t interpolateColor(uint32_t baseColor, float distanceFactor);
    void spawnRaindrop();
};



class EffectVortex : public Effect {
public:
    void render(Cube& cube, unsigned long deltaTime) override;

    float spawnRate = 0.08f;      // Частота появления частиц
    float fadeRate = 0.98f;       // Скорость затухания
    float expansionRate = 0.07f;  // Скорость расширения

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
    float timeSinceLastSpawn = 0;
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

    float spawnRate = 0.05f;  // Частота появления частиц
    float fadeRate = 0.97f;   // Скорость затухания
    float waveSpeed = 0.2f;   // Скорость волн

private:
    struct Particle {
        float x, y;
        float vx, vy;
        uint32_t color;
        float life;
        float initialDistance;
    };

    std::vector<Particle> particles;
    float timeSinceLastSpawn = 0;

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

    unsigned long moveInterval = 200;  // Скорость змейки (мс)
    bool autoMode = true;              // Автоматический режим

    void turnLeft();                   // Поворот влево
    void turnRight();                  // Поворот вправо

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

    int maxParticles = 10;           // Количество частиц
    float gravityStrength = 0.01f;   // Сила притяжения
    float explosionForce = 0.1f;     // Сила взрыва

private:
    struct Particle {
        float x, y;
        float vx, vy;
    };

    std::vector<Particle> particles;
    float friction = 0.95f;
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

// ============ PLASMA EFFECT ============
// Плазменный эффект с синусоидальными волнами и плавными цветовыми переходами
class EffectPlasma : public Effect {
public:
    EffectPlasma();
    void render(Cube& cube, unsigned long deltaTime) override;

    // Публичные параметры для настройки
    float speed = 0.05f;          // Скорость анимации
    float scale = 4.0f;           // Масштаб паттерна
    int colorShift = 0;           // Сдвиг цвета (0-255)

private:
    float time = 0;
    uint32_t plasmaColor(float value);
};

// ============ MATRIX RAIN EFFECT ============
// Цифровой дождь как в фильме "Матрица"
class EffectMatrixRain : public Effect {
public:
    EffectMatrixRain();
    void render(Cube& cube, unsigned long deltaTime) override;

    // Публичные параметры для настройки
    float dropSpeed = 0.15f;      // Скорость падения капель
    int trailLength = 4;          // Длина следа
    float spawnChance = 0.3f;     // Вероятность появления новой капли

private:
    static const int GRID_W = 8;
    static const int GRID_H = 8;

    struct Drop {
        float y;
        bool active;
        float speed;
    };

    Drop drops[GRID_W];
    float brightness[GRID_W][GRID_H];  // Яркость каждого пикселя

    void spawnDrop(int x);
    uint32_t getGreenShade(float intensity);
};

// ============ GAME OF LIFE EFFECT ============
// Клеточный автомат Конвея "Игра жизни"
class EffectGameOfLife : public Effect {
public:
    EffectGameOfLife();
    void render(Cube& cube, unsigned long deltaTime) override;

    // Публичные параметры для настройки
    float updateInterval = 200.0f;    // Интервал обновления поколения (мс)
    float randomizeThreshold = 5000.0f; // Время до рандомизации при стагнации (мс)
    int initialDensity = 35;           // Начальная плотность (%)

private:
    static const int GRID_W = 8;
    static const int GRID_H = 8;

    bool grid[GRID_W][GRID_H];
    bool nextGrid[GRID_W][GRID_H];
    uint32_t cellColor;

    float timeSinceUpdate = 0;
    float timeSinceChange = 0;
    int lastPopulation = 0;
    int stagnationCount = 0;

    void randomize();
    void nextGeneration();
    int countNeighbors(int x, int y);
    uint32_t getRandomColor();
};

// ============ FIREWORKS EFFECT ============
class EffectFireworks : public Effect {
public:
    EffectFireworks();
    void render(Cube& cube, unsigned long deltaTime) override;

    float launchRate = 0.02f;      // Частота запуска ракет
    float gravity = 0.05f;          // Гравитация
    int sparkCount = 12;            // Количество искр при взрыве

private:
    static const int GRID_W = 8;
    static const int GRID_H = 8;
    static const int MAX_SPARKS = 50;

    struct Spark {
        float x, y;
        float vx, vy;
        uint32_t color;
        float life;
        bool active;
    };

    struct Rocket {
        float x, y;
        float vy;
        uint32_t color;
        bool active;
        bool exploded;
    };

    Spark sparks[MAX_SPARKS];
    Rocket rocket;
    float brightness[GRID_W][GRID_H];
    uint32_t colors[GRID_W][GRID_H];

    void launchRocket();
    void explode();
    uint32_t getRandomColor();
};

// ============ STARFIELD EFFECT ============
class EffectStarfield : public Effect {
public:
    EffectStarfield();
    void render(Cube& cube, unsigned long deltaTime) override;

    float speed = 0.1f;             // Скорость звёзд
    int starCount = 15;             // Количество звёзд

private:
    static const int GRID_W = 8;
    static const int GRID_H = 8;
    static const int MAX_STARS = 20;

    struct Star {
        float x, y, z;
        bool active;
    };

    Star stars[MAX_STARS];
    void spawnStar(int idx);
};

// ============ LIGHTNING EFFECT ============
class EffectLightning : public Effect {
public:
    EffectLightning();
    void render(Cube& cube, unsigned long deltaTime) override;

    float strikeChance = 0.01f;     // Вероятность молнии
    float fadeSpeed = 0.15f;        // Скорость затухания

private:
    static const int GRID_W = 8;
    static const int GRID_H = 8;

    float brightness[GRID_W][GRID_H];
    bool striking;
    int strikeX;
    int strikeProgress;

    void generateBolt(int startX);
};

// ============ BOUNCING BALLS EFFECT ============
class EffectBouncingBalls : public Effect {
public:
    EffectBouncingBalls();
    void render(Cube& cube, unsigned long deltaTime) override;

    int ballCount = 3;              // Количество шариков
    float speed = 0.15f;            // Скорость

private:
    static const int GRID_W = 8;
    static const int GRID_H = 8;
    static const int MAX_BALLS = 5;

    struct Ball {
        float x, y;
        float vx, vy;
        uint32_t color;
    };

    Ball balls[MAX_BALLS];
    void initBalls();
};

// ============ RIPPLE POND EFFECT ============
class EffectRipplePond : public Effect {
public:
    EffectRipplePond();
    void render(Cube& cube, unsigned long deltaTime) override;

    float dropRate = 0.02f;         // Частота капель
    float waveSpeed = 0.3f;         // Скорость волн

private:
    static const int GRID_W = 8;
    static const int GRID_H = 8;
    static const int MAX_RIPPLES = 3;

    struct Ripple {
        float x, y;
        float radius;
        float life;
        bool active;
    };

    Ripple ripples[MAX_RIPPLES];
    void spawnRipple();
};

// ============ FIREFLIES EFFECT ============
class EffectFireflies : public Effect {
public:
    EffectFireflies();
    void render(Cube& cube, unsigned long deltaTime) override;

    int fireflyCount = 8;           // Количество светлячков
    float blinkSpeed = 0.03f;       // Скорость мигания

private:
    static const int GRID_W = 8;
    static const int GRID_H = 8;
    static const int MAX_FIREFLIES = 12;

    struct Firefly {
        int x, y;
        float phase;
        float speed;
        float brightness;
    };

    Firefly fireflies[MAX_FIREFLIES];
};

// ============ HEARTBEAT PULSE EFFECT ============
class EffectHeartbeatPulse : public Effect {
public:
    EffectHeartbeatPulse();
    void render(Cube& cube, unsigned long deltaTime) override;

    float bpm = 60.0f;              // Удары в минуту
    uint32_t pulseColor = 0xFF0000; // Цвет пульса

private:
    static const int GRID_W = 8;
    static const int GRID_H = 8;

    float time;
    float currentRadius;
};

// ============ COMET TRAIL EFFECT ============
class EffectCometTrail : public Effect {
public:
    EffectCometTrail();
    void render(Cube& cube, unsigned long deltaTime) override;

    float speed = 0.2f;             // Скорость кометы
    int trailLength = 5;            // Длина хвоста

private:
    static const int GRID_W = 8;
    static const int GRID_H = 8;

    float x, y;
    float vx, vy;
    uint32_t color;
    float brightness[GRID_W][GRID_H];

    void changeDirection();
    uint32_t getRandomColor();
};

// ============ SPARKLE EFFECT ============
class EffectSparkle : public Effect {
public:
    EffectSparkle();
    void render(Cube& cube, unsigned long deltaTime) override;

    float sparkleRate = 0.15f;      // Частота искр
    float fadeSpeed = 0.2f;         // Скорость затухания

private:
    static const int GRID_W = 8;
    static const int GRID_H = 8;

    float brightness[GRID_W][GRID_H];
    uint32_t colors[GRID_W][GRID_H];
};

// ============ CORNER PULSE EFFECT ============
class EffectCornerPulse : public Effect {
public:
    EffectCornerPulse();
    void render(Cube& cube, unsigned long deltaTime) override;

    float pulseSpeed = 0.1f;        // Скорость пульса
    float interval = 2000.0f;       // Интервал между пульсами (мс)

private:
    static const int GRID_W = 8;
    static const int GRID_H = 8;

    int currentCorner;
    float pulseRadius;
    float timeSincePulse;
    uint32_t pulseColor;

    uint32_t getRandomColor();
};

// ============ SCAN LINE EFFECT ============
class EffectScanLine : public Effect {
public:
    EffectScanLine();
    void render(Cube& cube, unsigned long deltaTime) override;

    float speed = 0.15f;            // Скорость сканирования
    bool vertical = false;          // Вертикальное направление

private:
    static const int GRID_W = 8;
    static const int GRID_H = 8;

    float position;
    int direction;
    uint32_t lineColor;
    float brightness[GRID_W][GRID_H];

    uint32_t getRandomColor();
};

// ============ PIXEL SORT EFFECT ============
class EffectPixelSort : public Effect {
public:
    EffectPixelSort();
    void render(Cube& cube, unsigned long deltaTime) override;

    float sortSpeed = 0.05f;        // Скорость сортировки
    float shuffleTime = 3000.0f;    // Время до перемешивания

private:
    static const int GRID_W = 8;
    static const int GRID_H = 8;

    uint32_t pixels[GRID_W * GRID_H];
    float timeSinceShuffle;
    bool sorting;
    int sortStep;

    void shuffle();
    void sortOneStep();
    uint32_t hueToColor(int hue);
};

// ============ GROWING SQUARES EFFECT ============
class EffectGrowingSquares : public Effect {
public:
    EffectGrowingSquares();
    void render(Cube& cube, unsigned long deltaTime) override;

    float growSpeed = 0.08f;        // Скорость роста
    float interval = 1500.0f;       // Интервал между квадратами

private:
    static const int GRID_W = 8;
    static const int GRID_H = 8;

    float currentSize;
    uint32_t currentColor;
    uint32_t previousColor;

    uint32_t getRandomColor();
};

// ============ RANDOM WALK EFFECT ============
class EffectRandomWalk : public Effect {
public:
    EffectRandomWalk();
    void render(Cube& cube, unsigned long deltaTime) override;

    int walkerCount = 3;            // Количество точек
    float speed = 0.1f;             // Скорость движения
    float fadeSpeed = 0.05f;        // Скорость затухания

private:
    static const int GRID_W = 8;
    static const int GRID_H = 8;
    static const int MAX_WALKERS = 5;

    struct Walker {
        int x, y;
        uint32_t color;
        float moveTimer;
    };

    Walker walkers[MAX_WALKERS];
    float brightness[GRID_W][GRID_H];
    uint32_t colors[GRID_W][GRID_H];
};

// ============ TETRIS FALL EFFECT ============
class EffectTetrisFall : public Effect {
public:
    EffectTetrisFall();
    void render(Cube& cube, unsigned long deltaTime) override;

    float fallSpeed = 0.1f;         // Скорость падения
    float clearDelay = 500.0f;      // Задержка перед очисткой линии

private:
    static const int GRID_W = 8;
    static const int GRID_H = 8;

    struct Piece {
        int x, y;
        int type;
        uint32_t color;
        bool active;
    };

    uint32_t grid[GRID_W][GRID_H];
    Piece currentPiece;
    float fallTimer;
    float clearTimer;
    int lineToClear;

    void spawnPiece();
    void lockPiece();
    bool checkCollision(int nx, int ny);
    void checkLines();
    uint32_t getRandomColor();
};

// ============ FACE NUMBERS EFFECT ============
// Отображение номеров граней разными цветами
class EffectFaceNumbers : public Effect {
public:
    EffectFaceNumbers();
    void render(Cube& cube, unsigned long deltaTime) override;

    bool colorCycle = true;          // Циклическая смена цветов
    float cycleSpeed = 0.01f;        // Скорость смены цветов

private:
    static const int GRID_W = 8;
    static const int GRID_H = 8;

    float hueOffset = 0.0f;
    void drawCharOnFace(Matrix& face, const std::string& character, uint32_t color);
    uint32_t hueToColor(float hue);
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
    EffectRaindropRipples effectRaindropRipples = EffectRaindropRipples();
    EffectPlasma effectPlasma = EffectPlasma();
    EffectMatrixRain effectMatrixRain = EffectMatrixRain();
    EffectGameOfLife effectGameOfLife = EffectGameOfLife();
    EffectFireworks effectFireworks = EffectFireworks();
    EffectStarfield effectStarfield = EffectStarfield();
    EffectLightning effectLightning = EffectLightning();
    EffectBouncingBalls effectBouncingBalls = EffectBouncingBalls();
    EffectRipplePond effectRipplePond = EffectRipplePond();
    EffectFireflies effectFireflies = EffectFireflies();
    EffectHeartbeatPulse effectHeartbeatPulse = EffectHeartbeatPulse();
    EffectCometTrail effectCometTrail = EffectCometTrail();
    EffectSparkle effectSparkle = EffectSparkle();
    EffectCornerPulse effectCornerPulse = EffectCornerPulse();
    EffectScanLine effectScanLine = EffectScanLine();
    EffectPixelSort effectPixelSort = EffectPixelSort();
    EffectGrowingSquares effectGrowingSquares = EffectGrowingSquares();
    EffectRandomWalk effectRandomWalk = EffectRandomWalk();
    EffectTetrisFall effectTetrisFall = EffectTetrisFall();
    EffectFaceNumbers effectFaceNumbers = EffectFaceNumbers();

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
        case CubeEffects::RAINDROP:
            activeEffect = &effectRaindropRipples;
            Serial.println("activeEffect RAINDROP");
            break;
        case CubeEffects::PLASMA:
            activeEffect = &effectPlasma;
            Serial.println("activeEffect PLASMA");
            break;
        case CubeEffects::MATRIX_RAIN:
            activeEffect = &effectMatrixRain;
            Serial.println("activeEffect MATRIX_RAIN");
            break;
        case CubeEffects::GAME_OF_LIFE:
            activeEffect = &effectGameOfLife;
            Serial.println("activeEffect GAME_OF_LIFE");
            break;
        case CubeEffects::FIREWORKS:
            activeEffect = &effectFireworks;
            Serial.println("activeEffect FIREWORKS");
            break;
        case CubeEffects::STARFIELD:
            activeEffect = &effectStarfield;
            Serial.println("activeEffect STARFIELD");
            break;
        case CubeEffects::LIGHTNING:
            activeEffect = &effectLightning;
            Serial.println("activeEffect LIGHTNING");
            break;
        case CubeEffects::BOUNCING_BALLS:
            activeEffect = &effectBouncingBalls;
            Serial.println("activeEffect BOUNCING_BALLS");
            break;
        case CubeEffects::RIPPLE_POND:
            activeEffect = &effectRipplePond;
            Serial.println("activeEffect RIPPLE_POND");
            break;
        case CubeEffects::FIREFLIES:
            activeEffect = &effectFireflies;
            Serial.println("activeEffect FIREFLIES");
            break;
        case CubeEffects::HEARTBEAT_PULSE:
            activeEffect = &effectHeartbeatPulse;
            Serial.println("activeEffect HEARTBEAT_PULSE");
            break;
        case CubeEffects::COMET_TRAIL:
            activeEffect = &effectCometTrail;
            Serial.println("activeEffect COMET_TRAIL");
            break;
        case CubeEffects::SPARKLE:
            activeEffect = &effectSparkle;
            Serial.println("activeEffect SPARKLE");
            break;
        case CubeEffects::CORNER_PULSE:
            activeEffect = &effectCornerPulse;
            Serial.println("activeEffect CORNER_PULSE");
            break;
        case CubeEffects::SCAN_LINE:
            activeEffect = &effectScanLine;
            Serial.println("activeEffect SCAN_LINE");
            break;
        case CubeEffects::PIXEL_SORT:
            activeEffect = &effectPixelSort;
            Serial.println("activeEffect PIXEL_SORT");
            break;
        case CubeEffects::GROWING_SQUARES:
            activeEffect = &effectGrowingSquares;
            Serial.println("activeEffect GROWING_SQUARES");
            break;
        case CubeEffects::RANDOM_WALK:
            activeEffect = &effectRandomWalk;
            Serial.println("activeEffect RANDOM_WALK");
            break;
        case CubeEffects::TETRIS_FALL:
            activeEffect = &effectTetrisFall;
            Serial.println("activeEffect TETRIS_FALL");
            break;
        case CubeEffects::FACE_NUMBERS:
            activeEffect = &effectFaceNumbers;
            Serial.println("activeEffect FACE_NUMBERS");
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
        return {breathingHeart, fallingStar, soundLevel, effectSpiral, fadePixels, effectDice, effectRomb, effectAurora, effectLavaLamp, effectGravity, effectParticles, effectSnake, effectDynamicGroups, effectCyberGhost, effectSpiritWind, effectVortex, effectRaindropRipples, effectPlasma, effectMatrixRain, effectGameOfLife, effectFireworks, effectStarfield, effectLightning, effectBouncingBalls, effectRipplePond, effectFireflies, effectHeartbeatPulse, effectCometTrail, effectSparkle, effectCornerPulse, effectScanLine, effectPixelSort, effectGrowingSquares, effectRandomWalk, effectTetrisFall};
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
