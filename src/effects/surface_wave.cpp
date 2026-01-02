#include "cube.h"
#include <cmath>

EffectSurfaceWave::EffectSurfaceWave() {
    time = 0;
    origin.face = 0;
    origin.x = GRID_W / 2;
    origin.y = GRID_H / 2;
}

float EffectSurfaceWave::getDistanceOnCube(int face, int x, int y) {
    // Calculate distance on cube surface from origin
    // This is simplified - treats adjacent faces as extensions
    float dx = 0, dy = 0;

    if (face == origin.face) {
        dx = x - origin.x;
        dy = y - origin.y;
    } else {
        // Distance includes face transition
        float baseDist = GRID_W / 2;  // Approximate distance to edge

        if (face == 0 || face == 1) { // front or back
            dx = baseDist + (face == origin.face ? 0 : GRID_W);
            dy = y - origin.y;
        } else if (face == 2 || face == 3) { // left or right
            dx = baseDist + GRID_W;
            dy = y - origin.y;
        } else { // top or bottom
            dx = x - origin.x;
            dy = baseDist + (face == 4 ? -GRID_H : GRID_H);
        }
    }

    return sqrt(dx * dx + dy * dy);
}

void EffectSurfaceWave::render(Cube& cube, unsigned long deltaTime) {
    time += waveSpeed;

    // Occasionally change origin
    if (random(1000) < 5) {
        origin.face = random(6);
        origin.x = random(GRID_W);
        origin.y = random(GRID_H);
    }

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    for (int f = 0; f < 6; f++) {
        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                float dist = getDistanceOnCube(f, x, y);
                float wave = sin(dist * frequency - time) * 0.5f + 0.5f;

                // Color based on wave height
                uint8_t r = (uint8_t)(wave * 100);
                uint8_t g = (uint8_t)(wave * 150 + 50);
                uint8_t b = (uint8_t)(wave * 255);

                faces[f]->setPixel(x, y, (r << 16) | (g << 8) | b);
            }
        }
    }

    cube.render();
}
