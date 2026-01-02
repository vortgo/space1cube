#include "cube.h"
#include <cstdlib>
#include <cmath>

EffectBouncingBalls::EffectBouncingBalls() {
    initBalls();
}

void EffectBouncingBalls::render(Cube& cube, unsigned long deltaTime) {
    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();

    for (auto& faceRef : faces) {
        Matrix& face = faceRef.get();
        // Очищаем
        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                face.setPixel(x, y, 0);
            }
        }

        // Обновляем и рисуем шарики
        for (int i = 0; i < ballCount && i < MAX_BALLS; i++) {
            balls[i].x += balls[i].vx * speed;
            balls[i].y += balls[i].vy * speed;

            // Отскок от стен
            if (balls[i].x <= 0 || balls[i].x >= GRID_W - 1) {
                balls[i].vx = -balls[i].vx;
                balls[i].x = balls[i].x <= 0 ? 0 : GRID_W - 1;
            }
            if (balls[i].y <= 0 || balls[i].y >= GRID_H - 1) {
                balls[i].vy = -balls[i].vy;
                balls[i].y = balls[i].y <= 0 ? 0 : GRID_H - 1;
            }

            int ix = (int)balls[i].x;
            int iy = (int)balls[i].y;
            if (ix >= 0 && ix < GRID_W && iy >= 0 && iy < GRID_H) {
                face.setPixel(ix, iy, balls[i].color);
            }
        }
    }

    cube.render();
}

void EffectBouncingBalls::initBalls() {
    uint32_t colors[] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF};
    for (int i = 0; i < MAX_BALLS; i++) {
        balls[i].x = random(GRID_W);
        balls[i].y = random(GRID_H);
        float angle = (float)random(360) * M_PI / 180.0f;
        balls[i].vx = cosf(angle);
        balls[i].vy = sinf(angle);
        balls[i].color = colors[i % 5];
    }
}
