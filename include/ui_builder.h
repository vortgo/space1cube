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

struct LavaLampData
{
    float period = 7000;
};
LavaLampData lavaLampData;

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
};
SnakeData snakeData;

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

struct RaindropData
{
    float dropInterval = 3.0f;
};
RaindropData raindropData;

struct RotateData
{
    float period = 15000;
};
RotateData rotateData;

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
                sets::Menu m(b, "Lava Lamp");
                b.Slider("lavaLampData.period"_h, "Period", 2000, 15000, 100, "", &lavaLampData.period);
                if (b.Button("Activate"))
                {
                    Serial.println("set active LAVA_LAMP");
                    cube->effectLavaLamp.period = lavaLampData.period;
                    cube->setActiveEffect(CubeEffects::LAVA_LAMP);
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
                if (b.Button("Activate"))
                {
                    Serial.println("set active SNAKE");
                    cube->effectSnake.moveInterval = snakeData.moveInterval;
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
                sets::Menu m(b, "Raindrop");
                b.Slider("raindrop.interval"_h, "Drop Interval", 0.5f, 5.0f, 0.5f, "sec", &raindropData.dropInterval);
                if (b.Button("Activate"))
                {
                    Serial.println("set active RAINDROP");
                    cube->effectRaindropRipples.dropInterval = raindropData.dropInterval;
                    cube->setActiveEffect(CubeEffects::RAINDROP);
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
