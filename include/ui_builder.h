#ifndef UI_BUILDER_H
#define UI_BUILDER_H

#include <Arduino.h>
#include <SettingsGyver.h>
#include <StringUtils.h>
#include "logger.h"
#include "cube.h"

DB_KEYS(
    kk,
    label,
    led,
    paragr,
    input,
    inputc,
    number,
    pass,
    color,
    sw,
    datew,
    timew,
    datetime,
    slider,
    sel,
    conf,
    btn,

    matrix1Leds,
    matrix2Leds,
    matrix3Leds,
    matrix4Leds,
    matrix5Leds,
    matrix6Leds

);

struct PrintSymbolData
{
    String symbol = "";
    uint32_t color = 0xff0000;
};
PrintSymbolData printSymbolData;

struct HeartData
{
    float beatPeriod = 1.0f;
};
HeartData heartData;


struct FallingStarData
{
    float fallSpeed = 0.03f; // Скорость падения
    uint8_t tailLength = 3; // Длина хвоста
};
FallingStarData fallingStarData;

struct SoundLevelData
{
    int basePeriod = 500;
    int pauseTime = 500;
};
SoundLevelData soundLevelData;

struct SpiralData
{
    unsigned long pauseForPixel = 70;
};
SpiralData spiralData;

struct FadePixelsData
{
    float basePeriod = 500.0f;
    float baseCooldown = 200.0f;
};
FadePixelsData fadePixelsData;

struct DiceData
{
    float maxSteps = 50;
};
DiceData diceData;

struct RombData
{
    float period = 1500;
};
RombData rombData;

struct AuroraData
{
    float period = 5000;
};
AuroraData auroraData;

struct GravityData
{
    int maxParticles = 10;
    float gravityStrength = 0.01f;
    float explosionForce = 0.1f;
};
GravityData gravityData;

struct SnakeData
{
    unsigned long moveInterval = 200;
    bool autoMode = true;
};
SnakeData snakeData;

struct StarfieldData { float speed = 0.1f; int starCount = 15; };
StarfieldData starfieldData;

struct LightningData { float strikeChance = 0.01f; float fadeSpeed = 0.15f; };
LightningData lightningData;

struct BouncingBallsData { int ballCount = 3; float speed = 0.15f; };
BouncingBallsData bouncingBallsData;

struct RipplePondData { float dropRate = 0.02f; float waveSpeed = 0.3f; };
RipplePondData ripplePondData;

struct FirefliesData { int fireflyCount = 8; float blinkSpeed = 0.03f; };
FirefliesData firefliesData;

struct HeartbeatPulseData { float bpm = 60.0f; };
HeartbeatPulseData heartbeatPulseData;

struct CometTrailData { float speed = 0.2f; int trailLength = 5; };
CometTrailData cometTrailData;

struct SparkleData { float sparkleRate = 0.15f; float fadeSpeed = 0.2f; };
SparkleData sparkleData;

struct CornerPulseData { float pulseSpeed = 0.1f; float interval = 2000.0f; };
CornerPulseData cornerPulseData;

struct GrowingSquaresData { float growSpeed = 0.08f; };
GrowingSquaresData growingSquaresData;

struct RandomWalkData { int walkerCount = 3; float speed = 0.1f; float fadeSpeed = 0.05f; };
RandomWalkData randomWalkData;

struct TetrisFallData { float fallSpeed = 0.1f; };
TetrisFallData tetrisFallData;

struct FaceNumbersData { bool colorCycle = true; float cycleSpeed = 0.01f; };
FaceNumbersData faceNumbersData;

// 3D Effects Data (with correct cube geometry)
struct RollingBall3DData { float speed = 0.15f; int ballSize = 2; int trailLength = 8; };
RollingBall3DData rollingBall3DData;

struct Wave3DData { float waveSpeed = 0.08f; float frequency = 0.5f; };
Wave3DData wave3DData;

struct Spiral3DData { float speed = 1.5f; float spiralTightness = 25.0f; int arms = 3; };
Spiral3DData spiral3DData;

struct Pulse3DData { float speed = 0.15f; int rings = 3; };
Pulse3DData pulse3DData;

struct Scan3DData { float speed = 0.2f; float thickness = 2.0f; };
Scan3DData scan3DData;

struct DNA3DData { float speed = 2.0f; float helixTwist = 15.0f; float radius = 2.5f; };
DNA3DData dna3DData;

struct Cross3DData { float speed = 0.05f; };
Cross3DData cross3DData;

struct GameOfLife3DData { float stepInterval = 200.0f; float density = 0.3f; };
GameOfLife3DData gameOfLife3DData;

struct Maze3DData { float solveSpeed = 0.1f; };
Maze3DData maze3DData;

struct Orbits3DData { float speed = 1.0f; int orbitCount = 3; };
Orbits3DData orbits3DData;

struct Fire3DData { int cooling = 20; int sparking = 50; };
Fire3DData fire3DData;

struct VortexData
{
    float spawnRate = 0.08f;
    float fadeRate = 0.98f;
    float expansionRate = 0.07f;
};
VortexData vortexData;

struct SpiritWindData
{
    float spawnRate = 0.05f;
    float fadeRate = 0.97f;
    float waveSpeed = 0.2f;
};
SpiritWindData spiritWindData;

struct RotateData
{
    float period = 15000;
};
RotateData rotateData;

struct PlasmaData
{
    float speed = 0.05f;
    float scale = 4.0f;
    int colorShift = 0;
};
PlasmaData plasmaData;

struct MatrixRainData
{
    float dropSpeed = 0.15f;
    int trailLength = 4;
    float spawnChance = 0.3f;
};
MatrixRainData matrixRainData;

struct GameOfLifeData
{
    float updateInterval = 200.0f;
    float randomizeThreshold = 5000.0f;
    int initialDensity = 35;
};
GameOfLifeData gameOfLifeData;

bool cfm_f, notice_f, alert_f;
bool onTop = true, onBottom = true, onFront = true, onBack = true, onLeft = true, onRight = true;

float maxBrightness = 0.5f;


void build(sets::Builder &b)
{
 	{
   		b.Label("batteryValue"_h, "Battery", Text(std::to_string(cube->getBatteryValue()).c_str()));
 	}
        {
            sets::Group g(b, "Effects");

            {
                sets::Menu pixelControl(b, "Print Symbol");
                b.Input("Symbol", &printSymbolData.symbol);
                b.Color("Color", &printSymbolData.color);
                if (b.Button("Submit"))
                {
                    Serial.println("set active SYMBOL");
                    cube->effectSymbol.print(printSymbolData.symbol.c_str(), printSymbolData.color);
                    cube->setActiveEffect(CubeEffects::SYMBOL);
                }
            }

            {
                sets::Menu pixelControl(b, "Fade Pixels");
                b.Slider("fadePixelsData.basePeriod"_h,"Base Period", 100, 1000, 50,"", &fadePixelsData.basePeriod);
                b.Slider("fadePixelsData.baseCooldown"_h,"Base Cooldown", 100, 1000, 50,"", &fadePixelsData.baseCooldown);

                if (b.Button("Activate"))
                {
                    Serial.println("set active FADE_PIXEL");
                    cube->fadePixels.basePeriod = static_cast<int>(fadePixelsData.basePeriod);
                    cube->fadePixels.baseCooldown = static_cast<int>(fadePixelsData.baseCooldown);
                    cube->setActiveEffect(CubeEffects::FADE_PIXEL);
                }
            }

            {
                sets::Menu pixelControl(b, "Spiral");
                b.Slider("spiralData.pauseForPixel"_h,"Pause Time", 1, 250, 1,"TEXT", &spiralData.pauseForPixel);

                if (b.Button("Activate"))
                {
                    Serial.println("set active SPIRAL");
                    cube->effectSpiral.pauseForPixel = spiralData.pauseForPixel;
                    cube->clear();
                    cube->setActiveEffect(CubeEffects::SPIRAL);
                }
            }

            {
                sets::Menu pixelControl(b, "Sound Levels");
                b.Slider("soundLevelData.basePeriod"_h,"Base Period", 200, 1500, 10,"TEXT", &soundLevelData.basePeriod);
                b.Slider("soundLevelData.pauseTime"_h,"Pause Time", 200, 1500, 10,"TEXT", &soundLevelData.pauseTime);

                if (b.Button("Activate"))
                {
                    Serial.println("set active SOUND_PEAKS");
                    cube->soundLevel.basePeriod = soundLevelData.basePeriod;
                    cube->soundLevel.pauseTime = soundLevelData.pauseTime;
                    cube->setActiveEffect(CubeEffects::SOUND_PEAKS);
                }
            }

            {
                sets::Menu pixelControl(b, "Dice");
                b.Slider("diceData.maxSteps"_h,"Period", 20, 200, 10,"", &diceData.maxSteps);

                if (b.Button("Activate"))
                {
                    Serial.println("set active DICE");

                    cube->effectDice.maxSteps = diceData.maxSteps;
                    cube->setActiveEffect(CubeEffects::DICE);
                }
            }

            {
                sets::Menu pixelControl(b, "Rotate");
                b.Slider("rotateData.period"_h, "Period (ms)", 5000, 30000, 1000, "", &rotateData.period);
                if (b.Button("Activate"))
                {
                    Serial.println("set active Rotate");
                    cube->effectRotate.period = rotateData.period;
                    cube->setActiveEffect(CubeEffects::ROTATE);
                }
            }

            {
                sets::Menu pixelControl(b, "Falling Starts");

                b.Slider("fallingStarData.tailLength"_h, "Tail Length", 1, 5, 1,"", &fallingStarData.tailLength);
                b.Slider("fallingStarData.fallSpeed"_h, "Fall Speed", 0.01, 0.1, 0.01,"", &fallingStarData.fallSpeed);

                if (b.Button("Activate"))
                {
                    Serial.println("set active FAILING_STAR");
                    cube->fallingStar.tailLength = fallingStarData.tailLength - 1;
                    cube->fallingStar.fallSpeed = fallingStarData.fallSpeed;
                    cube->setActiveEffect(CubeEffects::FAILING_STAR);
                }
            }

            {
                sets::Menu pixelControl(b, "Heart");
                b.Slider("heartData.beatPeriod"_h,"Beat Period", 0, 5, 0.1,"", &heartData.beatPeriod);
                if (b.Button("Activate"))
                {
                    Serial.println("set active HEART");
                    cube->breathingHeart.beatPeriod = heartData.beatPeriod;
                    cube->setActiveEffect(CubeEffects::HEART);
                }
            }

            {
                sets::Menu pixelControl(b, "Romb");
                b.Slider("rombData.period"_h,"Period", 500, 5000, 10,"", &rombData.period);
                if (b.Button("Activate"))
                {
                    Serial.println("set active Romb");
                    cube->effectRomb.period = rombData.period;
                    cube->setActiveEffect(CubeEffects::ROMB);
                }
            }

            {
                sets::Menu m(b, "Aurora");
                b.Slider("auroraData.period"_h, "Period", 1000, 10000, 100, "", &auroraData.period);
                if (b.Button("Activate"))
                {
                    Serial.println("set active AURORA");
                    cube->effectAurora.period = auroraData.period;
                    cube->setActiveEffect(CubeEffects::AURORA);
                }
            }

            {
                sets::Menu m(b, "Gravity");
                b.Slider("gravityData.particles"_h, "Particles", 5, 20, 1, "", &gravityData.maxParticles);
                b.Slider("gravityData.gravity"_h, "Gravity", 0.005f, 0.05f, 0.005f, "", &gravityData.gravityStrength);
                b.Slider("gravityData.explosion"_h, "Explosion", 0.05f, 0.3f, 0.01f, "", &gravityData.explosionForce);
                if (b.Button("Activate"))
                {
                    Serial.println("set active GRAVITY");
                    cube->effectGravity.maxParticles = gravityData.maxParticles;
                    cube->effectGravity.gravityStrength = gravityData.gravityStrength;
                    cube->effectGravity.explosionForce = gravityData.explosionForce;
                    cube->setActiveEffect(CubeEffects::GRAVITY);
                }
            }

            {
                sets::Menu m(b, "Particles");
                if (b.Button("Activate"))
                {
                    Serial.println("set active PARTICLES");
                    cube->setActiveEffect(CubeEffects::PARTICLES);
                }
            }

            {
                sets::Menu m(b, "Snake");
                b.Slider("snakeData.speed"_h, "Speed (ms)", 50, 500, 10, "", &snakeData.moveInterval);
                if (b.Switch("snakeData.auto"_h, "Auto Mode", &snakeData.autoMode)) {
                    cube->effectSnake.autoMode = snakeData.autoMode;
                }
                b.beginRow();
                if (b.Button("◀ Left")) {
                    cube->effectSnake.turnLeft();
                }
                if (b.Button("Right ▶")) {
                    cube->effectSnake.turnRight();
                }
                b.endRow();
                if (b.Button("Activate"))
                {
                    Serial.println("set active SNAKE");
                    cube->effectSnake.moveInterval = snakeData.moveInterval;
                    cube->effectSnake.autoMode = snakeData.autoMode;
                    cube->setActiveEffect(CubeEffects::SNAKE);
                }
            }

            {
                sets::Menu m(b, "Dynamic Groups");
                if (b.Button("Activate"))
                {
                    Serial.println("set active DYNAMIC_GROUPS");
                    cube->setActiveEffect(CubeEffects::DYNAMIC_GROUPS);
                }
            }

            {
                sets::Menu m(b, "Cyber Ghost");
                if (b.Button("Activate"))
                {
                    Serial.println("set active CYBER_GHOST");
                    cube->setActiveEffect(CubeEffects::CYBER_GHOST);
                }
            }

            {
                sets::Menu m(b, "Spirit Wind");
                b.Slider("spiritWind.spawn"_h, "Spawn Rate", 0.01f, 0.15f, 0.01f, "", &spiritWindData.spawnRate);
                b.Slider("spiritWind.fade"_h, "Fade Rate", 0.9f, 0.99f, 0.01f, "", &spiritWindData.fadeRate);
                b.Slider("spiritWind.wave"_h, "Wave Speed", 0.1f, 0.5f, 0.05f, "", &spiritWindData.waveSpeed);
                if (b.Button("Activate"))
                {
                    Serial.println("set active SPIRIT_WIND");
                    cube->effectSpiritWind.spawnRate = spiritWindData.spawnRate;
                    cube->effectSpiritWind.fadeRate = spiritWindData.fadeRate;
                    cube->effectSpiritWind.waveSpeed = spiritWindData.waveSpeed;
                    cube->setActiveEffect(CubeEffects::SPIRIT_WIND);
                }
            }

            {
                sets::Menu m(b, "Vortex");
                b.Slider("vortex.spawn"_h, "Spawn Rate", 0.02f, 0.2f, 0.01f, "", &vortexData.spawnRate);
                b.Slider("vortex.fade"_h, "Fade Rate", 0.9f, 0.99f, 0.01f, "", &vortexData.fadeRate);
                b.Slider("vortex.expand"_h, "Expansion", 0.02f, 0.15f, 0.01f, "", &vortexData.expansionRate);
                if (b.Button("Activate"))
                {
                    Serial.println("set active VORTEX");
                    cube->effectVortex.spawnRate = vortexData.spawnRate;
                    cube->effectVortex.fadeRate = vortexData.fadeRate;
                    cube->effectVortex.expansionRate = vortexData.expansionRate;
                    cube->setActiveEffect(CubeEffects::VORTEX);
                }
            }

            {
                sets::Menu m(b, "Plasma");
                b.Slider("plasma.speed"_h, "Speed", 0.01f, 0.15f, 0.01f, "", &plasmaData.speed);
                b.Slider("plasma.scale"_h, "Scale", 2.0f, 8.0f, 0.5f, "", &plasmaData.scale);
                b.Slider("plasma.color"_h, "Color Shift", 0, 255, 5, "", &plasmaData.colorShift);
                if (b.Button("Activate"))
                {
                    Serial.println("set active PLASMA");
                    cube->effectPlasma.speed = plasmaData.speed;
                    cube->effectPlasma.scale = plasmaData.scale;
                    cube->effectPlasma.colorShift = plasmaData.colorShift;
                    cube->setActiveEffect(CubeEffects::PLASMA);
                }
            }

            {
                sets::Menu m(b, "Matrix Rain");
                b.Slider("matrix.speed"_h, "Drop Speed", 0.05f, 0.3f, 0.02f, "", &matrixRainData.dropSpeed);
                b.Slider("matrix.trail"_h, "Trail Length", 2, 6, 1, "", &matrixRainData.trailLength);
                b.Slider("matrix.spawn"_h, "Spawn Chance", 0.1f, 0.5f, 0.05f, "", &matrixRainData.spawnChance);
                if (b.Button("Activate"))
                {
                    Serial.println("set active MATRIX_RAIN");
                    cube->effectMatrixRain.dropSpeed = matrixRainData.dropSpeed;
                    cube->effectMatrixRain.trailLength = matrixRainData.trailLength;
                    cube->effectMatrixRain.spawnChance = matrixRainData.spawnChance;
                    cube->setActiveEffect(CubeEffects::MATRIX_RAIN);
                }
            }

            {
                sets::Menu m(b, "Game of Life");
                b.Slider("gol.interval"_h, "Update Interval", 100.0f, 500.0f, 25.0f, "ms", &gameOfLifeData.updateInterval);
                b.Slider("gol.threshold"_h, "Reset Threshold", 3000.0f, 10000.0f, 500.0f, "ms", &gameOfLifeData.randomizeThreshold);
                b.Slider("gol.density"_h, "Initial Density", 20, 50, 5, "%", &gameOfLifeData.initialDensity);
                if (b.Button("Activate"))
                {
                    Serial.println("set active GAME_OF_LIFE");
                    cube->effectGameOfLife.updateInterval = gameOfLifeData.updateInterval;
                    cube->effectGameOfLife.randomizeThreshold = gameOfLifeData.randomizeThreshold;
                    cube->effectGameOfLife.initialDensity = gameOfLifeData.initialDensity;
                    cube->setActiveEffect(CubeEffects::GAME_OF_LIFE);
                }
            }

            {
                sets::Menu m(b, "Starfield");
                b.Slider("sf.speed"_h, "Speed", 0.05f, 0.3f, 0.02f, "", &starfieldData.speed);
                b.Slider("sf.count"_h, "Star Count", 5, 20, 1, "", &starfieldData.starCount);
                if (b.Button("Activate")) {
                    cube->effectStarfield.speed = starfieldData.speed;
                    cube->effectStarfield.starCount = starfieldData.starCount;
                    cube->setActiveEffect(CubeEffects::STARFIELD);
                }
            }

            {
                sets::Menu m(b, "Lightning");
                b.Slider("lg.chance"_h, "Strike Chance", 0.005f, 0.05f, 0.005f, "", &lightningData.strikeChance);
                b.Slider("lg.fade"_h, "Fade Speed", 0.05f, 0.3f, 0.02f, "", &lightningData.fadeSpeed);
                if (b.Button("Activate")) {
                    cube->effectLightning.strikeChance = lightningData.strikeChance;
                    cube->effectLightning.fadeSpeed = lightningData.fadeSpeed;
                    cube->setActiveEffect(CubeEffects::LIGHTNING);
                }
            }

            {
                sets::Menu m(b, "Bouncing Balls");
                b.Slider("bb.count"_h, "Ball Count", 1, 5, 1, "", &bouncingBallsData.ballCount);
                b.Slider("bb.speed"_h, "Speed", 0.05f, 0.3f, 0.02f, "", &bouncingBallsData.speed);
                if (b.Button("Activate")) {
                    cube->effectBouncingBalls.ballCount = bouncingBallsData.ballCount;
                    cube->effectBouncingBalls.speed = bouncingBallsData.speed;
                    cube->setActiveEffect(CubeEffects::BOUNCING_BALLS);
                }
            }

            {
                sets::Menu m(b, "Ripple Pond");
                b.Slider("rp.rate"_h, "Drop Rate", 0.01f, 0.1f, 0.01f, "", &ripplePondData.dropRate);
                b.Slider("rp.wave"_h, "Wave Speed", 0.1f, 0.5f, 0.05f, "", &ripplePondData.waveSpeed);
                if (b.Button("Activate")) {
                    cube->effectRipplePond.dropRate = ripplePondData.dropRate;
                    cube->effectRipplePond.waveSpeed = ripplePondData.waveSpeed;
                    cube->setActiveEffect(CubeEffects::RIPPLE_POND);
                }
            }

            {
                sets::Menu m(b, "Fireflies");
                b.Slider("ff.count"_h, "Firefly Count", 3, 12, 1, "", &firefliesData.fireflyCount);
                b.Slider("ff.blink"_h, "Blink Speed", 0.01f, 0.1f, 0.01f, "", &firefliesData.blinkSpeed);
                if (b.Button("Activate")) {
                    cube->effectFireflies.fireflyCount = firefliesData.fireflyCount;
                    cube->effectFireflies.blinkSpeed = firefliesData.blinkSpeed;
                    cube->setActiveEffect(CubeEffects::FIREFLIES);
                }
            }

            {
                sets::Menu m(b, "Heartbeat Pulse");
                b.Slider("hb.bpm"_h, "BPM", 40.0f, 120.0f, 5.0f, "", &heartbeatPulseData.bpm);
                if (b.Button("Activate")) {
                    cube->effectHeartbeatPulse.bpm = heartbeatPulseData.bpm;
                    cube->setActiveEffect(CubeEffects::HEARTBEAT_PULSE);
                }
            }

            {
                sets::Menu m(b, "Comet Trail");
                b.Slider("ct.speed"_h, "Speed", 0.1f, 0.4f, 0.02f, "", &cometTrailData.speed);
                b.Slider("ct.trail"_h, "Trail Length", 3, 8, 1, "", &cometTrailData.trailLength);
                if (b.Button("Activate")) {
                    cube->effectCometTrail.speed = cometTrailData.speed;
                    cube->effectCometTrail.trailLength = cometTrailData.trailLength;
                    cube->setActiveEffect(CubeEffects::COMET_TRAIL);
                }
            }

            {
                sets::Menu m(b, "Sparkle");
                b.Slider("sp.rate"_h, "Sparkle Rate", 0.05f, 0.3f, 0.02f, "", &sparkleData.sparkleRate);
                b.Slider("sp.fade"_h, "Fade Speed", 0.1f, 0.4f, 0.02f, "", &sparkleData.fadeSpeed);
                if (b.Button("Activate")) {
                    cube->effectSparkle.sparkleRate = sparkleData.sparkleRate;
                    cube->effectSparkle.fadeSpeed = sparkleData.fadeSpeed;
                    cube->setActiveEffect(CubeEffects::SPARKLE);
                }
            }

            {
                sets::Menu m(b, "Corner Pulse");
                b.Slider("cp.speed"_h, "Pulse Speed", 0.05f, 0.2f, 0.02f, "", &cornerPulseData.pulseSpeed);
                b.Slider("cp.interval"_h, "Interval (ms)", 1000.0f, 4000.0f, 200.0f, "", &cornerPulseData.interval);
                if (b.Button("Activate")) {
                    cube->effectCornerPulse.pulseSpeed = cornerPulseData.pulseSpeed;
                    cube->effectCornerPulse.interval = cornerPulseData.interval;
                    cube->setActiveEffect(CubeEffects::CORNER_PULSE);
                }
            }

            {
                sets::Menu m(b, "Growing Squares");
                b.Slider("gs.speed"_h, "Grow Speed", 0.03f, 0.15f, 0.01f, "", &growingSquaresData.growSpeed);
                if (b.Button("Activate")) {
                    cube->effectGrowingSquares.growSpeed = growingSquaresData.growSpeed;
                    cube->setActiveEffect(CubeEffects::GROWING_SQUARES);
                }
            }

            {
                sets::Menu m(b, "Random Walk");
                b.Slider("rw.count"_h, "Walker Count", 1, 5, 1, "", &randomWalkData.walkerCount);
                b.Slider("rw.speed"_h, "Speed", 0.05f, 0.2f, 0.02f, "", &randomWalkData.speed);
                b.Slider("rw.fade"_h, "Fade Speed", 0.02f, 0.15f, 0.01f, "", &randomWalkData.fadeSpeed);
                if (b.Button("Activate")) {
                    cube->effectRandomWalk.walkerCount = randomWalkData.walkerCount;
                    cube->effectRandomWalk.speed = randomWalkData.speed;
                    cube->effectRandomWalk.fadeSpeed = randomWalkData.fadeSpeed;
                    cube->setActiveEffect(CubeEffects::RANDOM_WALK);
                }
            }

            {
                sets::Menu m(b, "Tetris Fall");
                b.Slider("tf.speed"_h, "Fall Speed", 0.05f, 0.3f, 0.02f, "", &tetrisFallData.fallSpeed);
                b.beginRow();
                if (b.Button("◀")) {
                    cube->effectTetrisFall.moveLeft();
                }
                if (b.Button("▼")) {
                    cube->effectTetrisFall.drop();
                }
                if (b.Button("▶")) {
                    cube->effectTetrisFall.moveRight();
                }
                b.endRow();
                if (b.Button("↻ Rotate")) {
                    cube->effectTetrisFall.rotate();
                }
                if (b.Button("Activate")) {
                    cube->effectTetrisFall.fallSpeed = tetrisFallData.fallSpeed;
                    cube->setActiveEffect(CubeEffects::TETRIS_FALL);
                }
            }

            {
                sets::Menu m(b, "Face Numbers");
                if (b.Switch("fn.cycle"_h, "Color Cycle", &faceNumbersData.colorCycle)) {
                    cube->effectFaceNumbers.colorCycle = faceNumbersData.colorCycle;
                }
                if (faceNumbersData.colorCycle) {
                    b.Slider("fn.speed"_h, "Cycle Speed", 0.005f, 0.05f, 0.005f, "", &faceNumbersData.cycleSpeed);
                }
                if (b.Button("Activate")) {
                    cube->effectFaceNumbers.colorCycle = faceNumbersData.colorCycle;
                    cube->effectFaceNumbers.cycleSpeed = faceNumbersData.cycleSpeed;
                    cube->setActiveEffect(CubeEffects::FACE_NUMBERS);
                }
            }

            // ============ 3D EFFECTS (with correct cube geometry) ============
            {
                sets::Menu m(b, "Rolling Ball 3D");
                b.Slider("rb.speed"_h, "Speed", 0.05f, 0.3f, 0.02f, "", &rollingBall3DData.speed);
                b.Slider("rb.size"_h, "Ball Size", 1, 3, 1, "", &rollingBall3DData.ballSize);
                b.Slider("rb.trail"_h, "Trail Length", 3, 15, 1, "", &rollingBall3DData.trailLength);
                if (b.Button("Activate")) {
                    cube->effectRollingBall3D.speed = rollingBall3DData.speed;
                    cube->effectRollingBall3D.ballSize = rollingBall3DData.ballSize;
                    cube->effectRollingBall3D.trailLength = rollingBall3DData.trailLength;
                    cube->setActiveEffect(CubeEffects::ROLLING_BALL_3D);
                }
            }

            {
                sets::Menu m(b, "Wave 3D");
                b.Slider("w3d.speed"_h, "Wave Speed", 0.03f, 0.15f, 0.01f, "", &wave3DData.waveSpeed);
                b.Slider("w3d.freq"_h, "Frequency", 0.2f, 1.0f, 0.1f, "", &wave3DData.frequency);
                if (b.Button("Activate")) {
                    cube->effectWave3D.waveSpeed = wave3DData.waveSpeed;
                    cube->effectWave3D.frequency = wave3DData.frequency;
                    cube->setActiveEffect(CubeEffects::WAVE_3D);
                }
            }

            {
                sets::Menu m(b, "Spiral 3D");
                b.Slider("sp3d.speed"_h, "Speed", 0.5f, 3.0f, 0.2f, "", &spiral3DData.speed);
                b.Slider("sp3d.tight"_h, "Tightness", 10.0f, 40.0f, 5.0f, "", &spiral3DData.spiralTightness);
                b.Slider("sp3d.arms"_h, "Arms", 2, 6, 1, "", &spiral3DData.arms);
                if (b.Button("Activate")) {
                    cube->effectSpiral3D.speed = spiral3DData.speed;
                    cube->effectSpiral3D.spiralTightness = spiral3DData.spiralTightness;
                    cube->effectSpiral3D.arms = spiral3DData.arms;
                    cube->setActiveEffect(CubeEffects::SPIRAL_3D);
                }
            }

            {
                sets::Menu m(b, "Pulse 3D");
                b.Slider("pu3d.speed"_h, "Speed", 0.05f, 0.3f, 0.02f, "", &pulse3DData.speed);
                b.Slider("pu3d.rings"_h, "Rings", 1, 5, 1, "", &pulse3DData.rings);
                if (b.Button("Activate")) {
                    cube->effectPulse3D.speed = pulse3DData.speed;
                    cube->effectPulse3D.rings = pulse3DData.rings;
                    cube->setActiveEffect(CubeEffects::PULSE_3D);
                }
            }

            {
                sets::Menu m(b, "Scan 3D");
                b.Slider("sc3d.speed"_h, "Speed", 0.1f, 0.5f, 0.05f, "", &scan3DData.speed);
                b.Slider("sc3d.thick"_h, "Thickness", 1.0f, 4.0f, 0.5f, "", &scan3DData.thickness);
                if (b.Button("Activate")) {
                    cube->effectScan3D.speed = scan3DData.speed;
                    cube->effectScan3D.thickness = scan3DData.thickness;
                    cube->setActiveEffect(CubeEffects::SCAN_3D);
                }
            }

            {
                sets::Menu m(b, "DNA 3D");
                b.Slider("dn3d.speed"_h, "Speed", 0.5f, 4.0f, 0.3f, "", &dna3DData.speed);
                b.Slider("dn3d.twist"_h, "Helix Twist", 8.0f, 25.0f, 2.0f, "", &dna3DData.helixTwist);
                b.Slider("dn3d.rad"_h, "Radius", 1.5f, 3.5f, 0.3f, "", &dna3DData.radius);
                if (b.Button("Activate")) {
                    cube->effectDNA3D.speed = dna3DData.speed;
                    cube->effectDNA3D.helixTwist = dna3DData.helixTwist;
                    cube->effectDNA3D.radius = dna3DData.radius;
                    cube->setActiveEffect(CubeEffects::DNA_3D);
                }
            }

            {
                sets::Menu m(b, "Cross 3D (Test)");
                b.Slider("cr3d.speed"_h, "Speed", 0.01f, 0.15f, 0.01f, "", &cross3DData.speed);
                if (b.Button("Activate")) {
                    cube->effectCross3D.speed = cross3DData.speed;
                    cube->setActiveEffect(CubeEffects::CROSS_3D);
                }
            }

            {
                sets::Menu m(b, "Game of Life 3D");
                b.Slider("gol3d.interval"_h, "Step Interval", 100.0f, 400.0f, 25.0f, "ms", &gameOfLife3DData.stepInterval);
                b.Slider("gol3d.density"_h, "Density", 0.2f, 0.5f, 0.05f, "", &gameOfLife3DData.density);
                if (b.Button("Reset")) {
                    cube->effectGameOfLife3D.reset();
                }
                if (b.Button("Activate")) {
                    cube->effectGameOfLife3D.stepInterval = gameOfLife3DData.stepInterval;
                    cube->effectGameOfLife3D.density = gameOfLife3DData.density;
                    cube->setActiveEffect(CubeEffects::GAME_OF_LIFE_3D);
                }
            }

            {
                sets::Menu m(b, "Maze 3D");
                b.Slider("mz3d.speed"_h, "Solve Speed", 0.05f, 0.2f, 0.02f, "", &maze3DData.solveSpeed);
                if (b.Button("Reset")) {
                    cube->effectMaze3D.reset();
                }
                if (b.Button("Activate")) {
                    cube->effectMaze3D.solveSpeed = maze3DData.solveSpeed;
                    cube->setActiveEffect(CubeEffects::MAZE_3D);
                }
            }

            {
                sets::Menu m(b, "Orbits 3D");
                b.Slider("orb3d.speed"_h, "Speed", 0.5f, 2.0f, 0.1f, "", &orbits3DData.speed);
                b.Slider("orb3d.count"_h, "Orbit Count", 1, 5, 1, "", &orbits3DData.orbitCount);
                if (b.Button("Activate")) {
                    cube->effectOrbits3D.speed = orbits3DData.speed;
                    cube->effectOrbits3D.orbitCount = orbits3DData.orbitCount;
                    cube->setActiveEffect(CubeEffects::ORBITS_3D);
                }
            }

            {
                sets::Menu m(b, "Fire 3D");
                b.Slider("fire3d.cool"_h, "Cooling", 10, 40, 2, "", &fire3DData.cooling);
                b.Slider("fire3d.spark"_h, "Sparking", 30, 80, 5, "%", &fire3DData.sparking);
                if (b.Button("Activate")) {
                    cube->effectFire3D.cooling = fire3DData.cooling;
                    cube->effectFire3D.sparking = fire3DData.sparking;
                    cube->setActiveEffect(CubeEffects::FIRE_3D);
                }
            }
        }


        {
            sets::Group g(b, "Settings");
            {
                {
                    sets::Menu facesMenu(b, "Faces");

                    if (b.Switch(H("ON/OFF-Front"), "Front: ON/OFF", &onFront))
                    {
                        onFront ? cube->front.turnOn() : cube->front.turnOff();
                    }

                    if (b.Switch(H("ON/OFF-Back"), "Back: ON/OFF", &onBack))
                    {
                        onBack ? cube->back.turnOn() : cube->back.turnOff();
                    }

                    if (b.Switch(H("ON/OFF-Left"), "Left: ON/OFF", &onLeft))
                    {
                        onLeft ? cube->left.turnOn() : cube->left.turnOff();
                    }

                    if (b.Switch(H("ON/OFF-Right"), "Right: ON/OFF", &onRight))
                    {
                        onRight ? cube->right.turnOn() : cube->right.turnOff();
                    }

                    if (b.Switch(H("ON/OFF-Top"), "Top: ON/OFF", &onTop))
                    {
                        onTop ? cube->top.turnOn() : cube->top.turnOff();
                    }

                    if (b.Switch(H("ON/OFF-Bottom"), "Bottom: ON/OFF", &onBottom))
                    {
                        onBottom ? cube->bottom.turnOn() : cube->bottom.turnOff();
                    }
                }
                {
                    sets::Menu g(b, "Logs");
                    b.Log(H(log), logger);
                }
                if (b.Slider("maxBrightness"_h,"Max Brightness", 0, 1, 0.05,"", &maxBrightness))
                {
                    std::vector<std::reference_wrapper<Matrix>> faces = cube->getFaces();
                    for (auto face : faces)
                    {
                        face.get().maxBrightness = maxBrightness;
                    }
                }
            }
        }

}

void update(sets::Updater &u)
{
    u.update("batteryValue"_h,Text(std::to_string(cube->getBatteryValue()).c_str()));
    u.update(H(log), logger);
}

#endif // UI_BUILDER_H
