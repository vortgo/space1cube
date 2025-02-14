#ifndef EFFECT_H
#define EFFECT_H

#include "cube.h"

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
       unsigned long renderTime;
};


#endif // EFFECT_H