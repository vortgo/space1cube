#ifndef SYMBOL_H
#define SYMBOL_H

#include "cube.h"
#include "effect.h"
#include <string>

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

#endif // SYMBOL_H