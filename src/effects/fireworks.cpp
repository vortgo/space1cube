#include "cube.h"
#include <cstdlib>
#include <cmath>

EffectFireworks::EffectFireworks() {
    rocket.active = false;
    rocket.exploded = false;
    for (int i = 0; i < MAX_SPARKS; i++) {
        sparks[i].active = false;
    }
    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            brightness[x][y] = 0;
            colors[x][y] = 0;
        }
    }
}

void EffectFireworks::render(Cube& cube, unsigned long deltaTime) {
    // Затухание
    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            brightness[x][y] *= 0.85f;
            if (brightness[x][y] < 0.01f) brightness[x][y] = 0;
        }
    }

    // Запуск ракеты
    if (!rocket.active && !rocket.exploded) {
        if ((float)random(1000) / 1000.0f < launchRate) {
            launchRocket();
        }
    }

    // Обновление ракеты
    if (rocket.active && !rocket.exploded) {
        rocket.y += rocket.vy;
        rocket.vy += gravity * 0.5f;

        int ix = (int)rocket.x;
        int iy = (int)rocket.y;
        if (ix >= 0 && ix < GRID_W && iy >= 0 && iy < GRID_H) {
            brightness[ix][iy] = 1.0f;
            colors[ix][iy] = 0xFFFFFF;
        }

        if (rocket.vy >= -0.1f || rocket.y <= 2) {
            explode();
        }
    }

    // Обновление искр
    for (int i = 0; i < MAX_SPARKS; i++) {
        if (sparks[i].active) {
            sparks[i].x += sparks[i].vx;
            sparks[i].y += sparks[i].vy;
            sparks[i].vy += gravity;
            sparks[i].life -= 0.02f;

            int ix = (int)sparks[i].x;
            int iy = (int)sparks[i].y;
            if (ix >= 0 && ix < GRID_W && iy >= 0 && iy < GRID_H) {
                float b = sparks[i].life;
                if (b > brightness[ix][iy]) {
                    brightness[ix][iy] = b;
                    colors[ix][iy] = sparks[i].color;
                }
            }

            if (sparks[i].life <= 0 || sparks[i].y >= GRID_H) {
                sparks[i].active = false;
            }
        }
    }

    // Проверяем завершение взрыва
    bool anyActive = false;
    for (int i = 0; i < MAX_SPARKS; i++) {
        if (sparks[i].active) anyActive = true;
    }
    if (rocket.exploded && !anyActive) {
        rocket.exploded = false;
    }

    // Рендер
    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    for (auto& faceRef : faces) {
        Matrix& face = faceRef.get();
        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                if (brightness[x][y] > 0) {
                    uint32_t c = colors[x][y];
                    uint8_t r = ((c >> 16) & 0xFF) * brightness[x][y];
                    uint8_t g = ((c >> 8) & 0xFF) * brightness[x][y];
                    uint8_t b = (c & 0xFF) * brightness[x][y];
                    face.setPixel(x, y, (r << 16) | (g << 8) | b);
                } else {
                    face.setPixel(x, y, 0);
                }
            }
        }
    }
    cube.render();
}

void EffectFireworks::launchRocket() {
    rocket.x = random(2, GRID_W - 2);
    rocket.y = GRID_H - 1;
    rocket.vy = -0.4f - (float)random(20) / 100.0f;
    rocket.color = getRandomColor();
    rocket.active = true;
    rocket.exploded = false;
}

void EffectFireworks::explode() {
    rocket.active = false;
    rocket.exploded = true;

    int count = 0;
    for (int i = 0; i < MAX_SPARKS && count < sparkCount; i++) {
        if (!sparks[i].active) {
            float angle = (float)random(360) * M_PI / 180.0f;
            float speed = 0.1f + (float)random(15) / 100.0f;
            sparks[i].x = rocket.x;
            sparks[i].y = rocket.y;
            sparks[i].vx = cosf(angle) * speed;
            sparks[i].vy = sinf(angle) * speed;
            sparks[i].color = rocket.color;
            sparks[i].life = 0.8f + (float)random(40) / 100.0f;
            sparks[i].active = true;
            count++;
        }
    }
}

uint32_t EffectFireworks::getRandomColor() {
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
