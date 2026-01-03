#include "cube.h"
#include "cube_geometry.h"

// Fire effect rising on all faces of the cube

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

    // Update fire for each face
    for (int f = 0; f < 6; f++) {
        // Cool down every cell
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                int cooldown = random(0, cooling);
                if (heat[f][x][y] > cooldown) {
                    heat[f][x][y] -= cooldown;
                } else {
                    heat[f][x][y] = 0;
                }
            }
        }

        // Heat rises - drift heat upward
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 7; y++) {
                heat[f][x][y] = (heat[f][x][y + 1] +
                                 heat[f][x][(y + 2) % 8] +
                                 heat[f][(x + 7) % 8][y + 1] +
                                 heat[f][(x + 1) % 8][y + 1]) / 4;
            }
        }

        // Random sparks at bottom
        for (int x = 0; x < 8; x++) {
            if (random(100) < sparking) {
                heat[f][x][7] = random(160, 255);
            }
        }

        // Map heat to color and draw
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                uint32_t color = heatToColor(heat[f][x][y]);
                faces[f]->setPixel(x, y, color);
            }
        }
    }

    cube.render();
}

uint32_t EffectFire3D::heatToColor(uint8_t temperature) {
    // Scale temperature to 0-191
    uint8_t t192 = (temperature * 191) / 255;

    // Calculate color based on heat level
    uint8_t r, g, b;

    if (t192 < 64) {
        // Black to red
        r = t192 * 4;
        g = 0;
        b = 0;
    } else if (t192 < 128) {
        // Red to yellow
        r = 255;
        g = (t192 - 64) * 4;
        b = 0;
    } else {
        // Yellow to white
        r = 255;
        g = 255;
        b = (t192 - 128) * 4;
    }

    return (r << 16) | (g << 8) | b;
}
