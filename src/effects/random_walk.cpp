#include "cube.h"
#include <cstdlib>

EffectRandomWalk::EffectRandomWalk() {
    uint32_t colorPalette[] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF};
    for (int i = 0; i < MAX_WALKERS; i++) {
        walkers[i].x = random(GRID_W);
        walkers[i].y = random(GRID_H);
        walkers[i].color = colorPalette[i % 5];
        walkers[i].moveTimer = 0;
    }
    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            brightness[x][y] = 0;
            colors[x][y] = 0;
        }
    }
}

void EffectRandomWalk::render(Cube& cube, unsigned long deltaTime) {
    // Затухание
    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            brightness[x][y] *= (1.0f - fadeSpeed);
            if (brightness[x][y] < 0.01f) brightness[x][y] = 0;
        }
    }

    // Обновляем точки
    for (int i = 0; i < walkerCount && i < MAX_WALKERS; i++) {
        walkers[i].moveTimer += speed;
        if (walkers[i].moveTimer >= 1.0f) {
            walkers[i].moveTimer = 0;

            // Случайное направление
            int dir = random(4);
            switch (dir) {
                case 0: walkers[i].x++; break;
                case 1: walkers[i].x--; break;
                case 2: walkers[i].y++; break;
                case 3: walkers[i].y--; break;
            }

            // Ограничиваем границами
            if (walkers[i].x < 0) walkers[i].x = 0;
            if (walkers[i].x >= GRID_W) walkers[i].x = GRID_W - 1;
            if (walkers[i].y < 0) walkers[i].y = 0;
            if (walkers[i].y >= GRID_H) walkers[i].y = GRID_H - 1;
        }

        // Рисуем след
        int wx = walkers[i].x;
        int wy = walkers[i].y;
        brightness[wx][wy] = 1.0f;
        colors[wx][wy] = walkers[i].color;
    }

    // Рендер
    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    for (auto& faceRef : faces) {
        Matrix& face = faceRef.get();
        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                if (brightness[x][y] > 0) {
                    uint32_t c = colors[x][y];
                    uint8_t r = (uint8_t)(((c >> 16) & 0xFF) * brightness[x][y]);
                    uint8_t g = (uint8_t)(((c >> 8) & 0xFF) * brightness[x][y]);
                    uint8_t b = (uint8_t)((c & 0xFF) * brightness[x][y]);
                    face.setPixel(x, y, (r << 16) | (g << 8) | b);
                } else {
                    face.setPixel(x, y, 0);
                }
            }
        }
    }

    cube.render();
}
