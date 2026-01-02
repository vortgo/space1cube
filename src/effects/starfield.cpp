#include "cube.h"
#include <cstdlib>
#include <cmath>

EffectStarfield::EffectStarfield() {
    for (int i = 0; i < MAX_STARS; i++) {
        spawnStar(i);
        stars[i].z = (float)random(100) / 100.0f;
    }
}

void EffectStarfield::render(Cube& cube, unsigned long deltaTime) {
    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();

    for (auto& faceRef : faces) {
        Matrix& face = faceRef.get();
        // Очищаем грань
        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                face.setPixel(x, y, 0);
            }
        }

        // Обновляем и рисуем звёзды
        for (int i = 0; i < starCount && i < MAX_STARS; i++) {
            stars[i].z -= speed * 0.1f;

            if (stars[i].z <= 0) {
                spawnStar(i);
            }

            // Проецируем 3D на 2D
            float px = (stars[i].x / stars[i].z) + GRID_W / 2.0f;
            float py = (stars[i].y / stars[i].z) + GRID_H / 2.0f;

            int ix = (int)px;
            int iy = (int)py;

            if (ix >= 0 && ix < GRID_W && iy >= 0 && iy < GRID_H) {
                // Яркость зависит от глубины
                float brightness = 1.0f - stars[i].z;
                if (brightness < 0) brightness = 0;
                if (brightness > 1) brightness = 1;

                uint8_t b = (uint8_t)(255 * brightness);
                face.setPixel(ix, iy, (b << 16) | (b << 8) | b);
            } else if (stars[i].z > 0) {
                // Звезда вышла за экран - респавн
                spawnStar(i);
            }
        }
    }

    cube.render();
}

void EffectStarfield::spawnStar(int idx) {
    stars[idx].x = (float)(random(160) - 80) / 10.0f;
    stars[idx].y = (float)(random(160) - 80) / 10.0f;
    stars[idx].z = 1.0f;
    stars[idx].active = true;
}
