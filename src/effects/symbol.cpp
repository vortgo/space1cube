
#include <string>
#include "logger.h"
#include "cube.h"

void EffectSymbol::render(Cube &cube, unsigned long deltaTime){
    if (printed){
        return;
    }

    cube.clear();
    drawCharacter(cube, symbol, color);
}

void EffectSymbol::drawCharacter(Cube &cube, const std::string &character, uint32_t color)
{
    int colOffset = 1;
    int rowOffset = 1;

    auto it = charPatterns.find(character);
    if (it == charPatterns.end())
    {
        std::string message = "Символ отсутствует в алфавите: " + character;
        logger.println(message.c_str());
        return;
    }

    const Pattern &charPattern = it->second;
    for (int r = 0; r < 7; r++)
    {
        uint8_t rowPattern = charPattern[r];
        for (int c = 0; c < 5; c++)
        {
            if (rowPattern & (1 << (4 - c)))
            {
                int x = c + colOffset;
                int y = r + rowOffset;
                
                cube.back.setPixel(x,y, color);
                cube.front.setPixel(x,y, color);
                cube.top.setPixel(x,y, color);
                cube.bottom.setPixel(x,y, color);
                cube.left.setPixel(x,y, color);
                cube.right.setPixel(x,y, color);
            }
        }
    }
    cube.render();

    printed = true;
}