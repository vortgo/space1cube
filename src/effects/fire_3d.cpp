#include "cube.h"
#include "cube_geometry.h"
#include <cmath>

// Fire effect rising from bottom face

EffectFire3D::EffectFire3D() {
    // Initialize heat map
    for (int f = 0; f < 6; f++) {
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                heat[f][x][y] = 0;
            }
        }
    }
}

void EffectFire3D::render(Cube& cube, unsigned long deltaTime) {
    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Cool down all pixels
    for (int f = 0; f < 6; f++) {
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                int cooling = random((int)(cooldown * 10)) + 1;
                heat[f][x][y] = (heat[f][x][y] > cooling) ? heat[f][x][y] - cooling : 0;
            }
        }
    }

    // Spark new fire at bottom
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (random(100) < (int)(sparking * 100)) {
                heat[5][x][y] = min(255, heat[5][x][y] + random(160, 255));
            }
        }
    }

    // Heat rises - propagate from bottom to sides to top
    // Bottom (5) -> sides (0,1,2,3) -> top (4)

    // From bottom to sides
    for (int f = 0; f < 4; f++) {
        for (int x = 0; x < 8; x++) {
            // Get heat from corresponding bottom edge
            int bottomHeat;
            switch(f) {
                case 0: bottomHeat = heat[5][7-x][7]; break;  // front
                case 1: bottomHeat = heat[5][x][0]; break;    // back
                case 2: bottomHeat = heat[5][0][x]; break;    // left
                case 3: bottomHeat = heat[5][7][7-x]; break;  // right
                default: bottomHeat = 0;
            }
            heat[f][x][0] = max(heat[f][x][0], bottomHeat - 20);
        }
    }

    // Heat rises on side faces
    for (int f = 0; f < 4; f++) {
        for (int y = 7; y >= 1; y--) {
            for (int x = 0; x < 8; x++) {
                int newHeat = (heat[f][x][y-1] +
                              heat[f][(x+1)%8][y-1] +
                              heat[f][(x+7)%8][y-1]) / 3;
                heat[f][x][y] = newHeat;
            }
        }
    }

    // From sides to top
    for (int x = 0; x < 8; x++) {
        heat[4][x][0] = max(heat[4][x][0], heat[0][7-x][7] - 30);      // from front
        heat[4][x][7] = max(heat[4][x][7], heat[1][x][7] - 30);        // from back
        heat[4][0][x] = max(heat[4][0][x], heat[2][7-x][7] - 30);      // from left
        heat[4][7][x] = max(heat[4][7][x], heat[3][x][7] - 30);        // from right
    }

    // Draw fire
    for (int f = 0; f < 6; f++) {
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                if (heat[f][x][y] > 0) {
                    uint32_t color = heatToColor(heat[f][x][y]);
                    faces[f]->setPixel(x, y, color);
                }
            }
        }
    }

    cube.render();
}

uint32_t EffectFire3D::heatToColor(uint8_t temperature) {
    // Heat color: black -> red -> orange -> yellow -> white
    uint8_t r, g, b;

    if (temperature < 85) {
        r = temperature * 3;
        g = 0;
        b = 0;
    } else if (temperature < 170) {
        r = 255;
        g = (temperature - 85) * 3;
        b = 0;
    } else {
        r = 255;
        g = 255;
        b = (temperature - 170) * 3;
    }

    return (r << 16) | (g << 8) | b;
}
