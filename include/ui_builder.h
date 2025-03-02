#ifndef UI_BUILDER_H
#define UI_BUILDER_H

#include <Arduino.h>
#include <SettingsGyver.h>
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

bool cfm_f, notice_f, alert_f;
bool onTop = true, onBottom = true, onFront = true, onBack = true, onLeft = true, onRight = true;

float maxBrightness = 0.8f;


void build(sets::Builder &b)
{
    {
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
                if (b.Button("Activate"))
                {
                    Serial.println("set active Rotate");
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
}

void update(sets::Updater &u)
{
    u.update(H(log), logger);
}

#endif // UI_BUILDER_H
