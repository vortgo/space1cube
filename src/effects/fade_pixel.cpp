#include "cube.h"


uint8_t getBrightness(unsigned long accumulatedTime, int period, bool up) {
    if (period <= 0) {
        return up ? 255 : 0;
    }

    // Определяем, где мы находимся в текущем периоде
    unsigned long timeInPeriod = accumulatedTime % period;

    // Интерполяция от 0 до 255 (или наоборот)
    float fraction = static_cast<float>(timeInPeriod) / period;
    uint8_t brightness = up ? static_cast<uint8_t>(fraction * 255) : static_cast<uint8_t>((1.0f - fraction) * 255);

    return brightness;
}

uint32_t getRandomColor() {
    // Генерируем случайные значения для RGB, стремясь к насыщенным и контрастным цветам
    uint8_t r = rand() % 128 + 128;  // От 128 до 255
    uint8_t g = rand() % 128 + 128;  // От 128 до 255
    uint8_t b = 255;  // От 128 до 255

    // Генерируем цвет в формате RGB
    uint32_t color = (r << 16) | (g << 8) | b;
    return color;
}

EffectFadePixels::FadePixel EffectFadePixels::initPixel(){
     EffectFadePixels::FadePixel p = EffectFadePixels::FadePixel();
     p.period = getRandonIntInRange(minPeriod, maxPeriod);
     p.cooldown = getRandonIntInRange(minCooldown, maxCooldown);
     p.setColor(getRandomColor());
     p.up = true;

    return p;
}

EffectFadePixels::EffectFadePixels(){
    pixels.resize(8*8);
    for(int x = 0; x < 8; x++) {
        for(int y = 0; y < 8; y++) {
              pixels[x * 8 + y] = initPixel();
              pixels[x * 8 + y].setX(x);
              pixels[x * 8 + y].setY(y);
          }
    }
}



void EffectFadePixels::render(Cube& cube, unsigned long deltaTime){
    cube.clear();
    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();

    for(int i = 0; i < pixels.size(); i++){
        if (pixels[i].paused && pixels[i].cooldownAccumulate < pixels[i].cooldown) {
            pixels[i].cooldownAccumulate += deltaTime;

            continue;
        }

        if (pixels[i].paused && pixels[i].cooldownAccumulate > pixels[i].cooldown) {
            pixels[i].paused = false;
            pixels[i].cooldownAccumulate = 0;
            pixels[i].accumulatedTime = 0;
            pixels[i].setColor(getRandomColor());
            pixels[i].period = getRandonIntInRange(minPeriod, maxPeriod);
            pixels[i].cooldown = getRandonIntInRange(minCooldown, maxCooldown);
        }

        uint8_t br = getBrightness(pixels[i].accumulatedTime, pixels[i].period, pixels[i].up);
        pixels[i].setBrightness(br);
        pixels[i].accumulatedTime += deltaTime;




        if (pixels[i].getBrightness() > 245 && pixels[i].up){
            pixels[i].paused = true;
            pixels[i].up = !pixels[i].up;

        }

        if (pixels[i].getBrightness() < 10 && !pixels[i].up ){
            pixels[i].paused = true;
            pixels[i].setBrightness(0);
            pixels[i].up = !pixels[i].up;

        }

        for (int f = 0; f < faces.size(); f++) {
            faces[f].get().setPixel(pixels[i]);
        }
    }

    cube.render();
}