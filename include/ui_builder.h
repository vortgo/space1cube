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

struct Data
{
    String symbol = "";
    uint32_t color = 0xff0000;
};

Data data;
bool cfm_f, notice_f, alert_f;
bool onTop = true, onBottom = true, onFront = true, onBack = true, onLeft = true, onRight = true;
void build(sets::Builder &b)
{
    {
        {
            sets::Menu pixelControl(b, "Print Symbol");
            b.Input("Symbol", &data.symbol);
            b.Color("", &data.color);
            if (b.Button("Submit"))
            {
                cube->effectSymbol.print(data.symbol.c_str(), data.color);
                cube->setActiveEffect(CubeEffects::SYMBOL);
            }
        }

        sets::Menu facesMenu(b, "Faces");

        {
            sets::Menu front(b, "Front");
            if (b.Switch(H("ON/OFF-Front"), "ON/OFF", &onFront))
            {
                logger.println("on/off front");
                logger.println(onFront);
                onFront ? cube->front.turnOn() : cube->front.turnOff();
            }
        }
        {
            sets::Menu back(b, "Back");
            if (b.Switch(H("ON/OFF-Back"), "ON/OFF", &onBack))
            {
                onBack ? cube->back.turnOn() : cube->front.turnOff();
            }
        }
        {
            sets::Menu left(b, "Left");
            if (b.Switch(H("ON/OFF-Left"), "ON/OFF", &onLeft))
            {
                onLeft ? cube->left.turnOn() : cube->front.turnOff();
            }
        }
        {
            sets::Menu right(b, "Right");
            if (b.Switch(H("ON/OFF-Right"), "ON/OFF", &onRight))
            {
                onRight ? cube->right.turnOn() : cube->front.turnOff();
            }
        }
        {
            sets::Menu top(b, "Top");
            if (b.Switch(H("ON/OFF-Top"), "ON/OFF", &onTop))
            {
                onTop ? cube->top.turnOn() : cube->front.turnOff();
            }
        }
        {
            sets::Menu bottom(b, "Bottom");
            if (b.Switch(H("ON/OFF-Bottom"), "ON/OFF", &onBottom))
            {
                onBottom ? cube->bottom.turnOn() : cube->front.turnOff();
            }
        }
    }

    {
        sets::Menu g(b, "Logs");

        b.Log(H(log), logger);
    }

    {
        sets::Group g(b, "database^^^^666");

        b.Label(kk::label);
        b.LED(kk::led);
        b.Paragraph(kk::paragr);
        b.Input(kk::input);
        b.Number(kk::number);
        b.Pass(kk::pass);
        b.Color(kk::color);
        b.Switch(kk::sw);
        b.Date(kk::datew);
        b.Time(kk::timew);
        b.DateTime(kk::datetime);
        b.Slider(kk::slider);
        b.Select(kk::sel, "", "foo;bar;test");
        if (b.Button(kk::btn))
            Serial.println("btn 0");
    }

    if (b.beginButtons())
    {
        if (b.Button("Notice"))
            notice_f = true;
        if (b.Button("Error"))
            alert_f = true;
        if (b.Button("Confirm"))
            cfm_f = true;
        b.endButtons();
    }

    bool res;
    if (b.Confirm(kk::conf, "Confirm", &res))
    {
        Serial.println(res);
    }
}

void update(sets::Updater &u)
{
    u.update(H(log), logger);

    if (cfm_f)
    {
        cfm_f = false;
        u.update(kk::conf);
    }
    if (notice_f)
    {
        notice_f = false;
        u.notice("Уведомление");
    }
    if (alert_f)
    {
        alert_f = false;
        u.alert("Ошибка");
    }
}

#endif // UI_BUILDER_H
