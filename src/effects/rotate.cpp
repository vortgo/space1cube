#include "cube.h"

EffectRotate::EffectRotate(){

}

void EffectRotate::render(Cube& c, unsigned long deltaTime){
    accumulatedTime+=deltaTime;
        if(accumulatedTime > period){
            accumulatedTime = 0;
            effect++;
            c.clear();

        }

    int currentEffectIndex = effect % c.getEffectsForRotate().size();
    Effect & e = c.getEffectsForRotate()[currentEffectIndex].get();
    e.render(c, deltaTime);
}