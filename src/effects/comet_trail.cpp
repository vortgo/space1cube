#include "cube.h"
#include <cstdlib>
#include <cmath>

EffectCometTrail::EffectCometTrail() {
    x = random(GRID_W);
    y = random(GRID_H);
    changeDirection();
    color = getRandomColor();
    for (int i = 0; i < GRID_W; i++) {
        for (int j = 0; j < GRID_H; j++) {
            brightness[i][j] = 0;
        }
    }
}

void EffectCometTrail::render(Cube& cube, unsigned long deltaTime) {
    // Затухание следа
    for (int i = 0; i < GRID_W; i++) {
        for (int j = 0; j < GRID_H; j++) {
            brightness[i][j] *= 0.85f;
            if (brightness[i][j] < 0.01f) brightness[i][j] = 0;
        }
    }

    // Обновляем позицию
    x += vx * speed;
    y += vy * speed;

    // Отскок от стен с изменением цвета
    bool bounced = false;
    if (x <= 0 || x >= GRID_W - 1) {
        vx = -vx;
        x = x <= 0 ? 0 : GRID_W - 1;
        bounced = true;
    }
    if (y <= 0 || y >= GRID_H - 1) {
        vy = -vy;
        y = y <= 0 ? 0 : GRID_H - 1;
        bounced = true;
    }

    if (bounced) {
        color = getRandomColor();
    }

    // Рисуем голову кометы
    int ix = (int)x;
    int iy = (int)y;
    if (ix >= 0 && ix < GRID_W && iy >= 0 && iy < GRID_H) {
        brightness[ix][iy] = 1.0f;
    }

    // Рендер
    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    for (auto& faceRef : faces) {
        Matrix& face = faceRef.get();
        for (int i = 0; i < GRID_W; i++) {
            for (int j = 0; j < GRID_H; j++) {
                if (brightness[i][j] > 0) {
                    uint8_t r = (uint8_t)(((color >> 16) & 0xFF) * brightness[i][j]);
                    uint8_t g = (uint8_t)(((color >> 8) & 0xFF) * brightness[i][j]);
                    uint8_t b = (uint8_t)((color & 0xFF) * brightness[i][j]);
                    face.setPixel(i, j, (r << 16) | (g << 8) | b);
                } else {
                    face.setPixel(i, j, 0);
                }
            }
        }
    }

    cube.render();
}

void EffectCometTrail::changeDirection() {
    float angle = (float)random(360) * M_PI / 180.0f;
    vx = cosf(angle);
    vy = sinf(angle);
}

uint32_t EffectCometTrail::getRandomColor() {
    int hue = random(360);
    float h = hue / 60.0f;
    int i = (int)h;
    float f = h - i;
    uint8_t q = (uint8_t)(255 * (1.0f - f));
    uint8_t t = (uint8_t)(255 * f);
    switch (i % 6) {
        case 0: return (255 << 16) | (t << 8);
        case 1: return (q << 16) | (255 << 8);
        case 2: return (255 << 8) | t;
        case 3: return (255 << 8) | (q << 16);
        case 4: return (t << 16) | 255;
        default: return (255 << 16) | q;
    }
}
