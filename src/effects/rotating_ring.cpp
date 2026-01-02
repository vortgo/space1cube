#include "cube.h"
#include <cmath>

EffectRotatingRing::EffectRotatingRing() {
    position = 0;
    ringColor = getRandomColor();
}

uint32_t EffectRotatingRing::getRandomColor() {
    uint32_t colors[] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF, 0x00FFFF};
    return colors[random(6)];
}

void EffectRotatingRing::render(Cube& cube, unsigned long deltaTime) {
    position += speed;
    if (position >= GRID_W * 4) {
        position -= GRID_W * 4;
        ringColor = getRandomColor();
    }

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Ring rotates around the cube horizontally through front->right->back->left
    int phase = (int)(position / GRID_W) % 4;
    float subPos = fmod(position, (float)GRID_W);
    int ringPos = (int)subPos;

    // Faces order for horizontal ring: front(0), right(3), back(1), left(2)
    int faceOrder[] = {0, 3, 1, 2};

    // Draw ring on current and adjacent faces
    for (int i = 0; i < 4; i++) {
        int faceIdx = faceOrder[i];
        int relativePhase = (i - phase + 4) % 4;

        if (relativePhase == 0) {
            // Current face - main ring
            for (int y = 0; y < GRID_H; y++) {
                for (int w = 0; w < ringWidth; w++) {
                    int x = ringPos + w;
                    if (x >= 0 && x < GRID_W) {
                        faces[faceIdx]->setPixel(x, y, ringColor);
                    }
                }
            }
        } else if (relativePhase == 3) {
            // Previous face - ring exiting
            for (int y = 0; y < GRID_H; y++) {
                for (int w = 0; w < ringWidth; w++) {
                    int x = GRID_W - ringWidth + ringPos + w;
                    if (x >= 0 && x < GRID_W) {
                        float brightness = (float)(GRID_W - x) / ringWidth;
                        uint8_t r = (uint8_t)(((ringColor >> 16) & 0xFF) * brightness);
                        uint8_t g = (uint8_t)(((ringColor >> 8) & 0xFF) * brightness);
                        uint8_t b = (uint8_t)((ringColor & 0xFF) * brightness);
                        faces[faceIdx]->setPixel(x, y, (r << 16) | (g << 8) | b);
                    }
                }
            }
        }
    }

    // Also draw on top and bottom faces (ring goes through them)
    float ringAngle = position / (GRID_W * 4) * 2 * M_PI;
    int centerX = GRID_W / 2;
    int centerY = GRID_H / 2;

    for (int angle = 0; angle < 360; angle += 10) {
        float rad = (angle * M_PI / 180.0f) + ringAngle;
        int x = centerX + (int)(cos(rad) * 3);
        int y = centerY + (int)(sin(rad) * 3);
        if (x >= 0 && x < GRID_W && y >= 0 && y < GRID_H) {
            faces[4]->setPixel(x, y, ringColor);
            faces[5]->setPixel(x, GRID_H - 1 - y, ringColor);
        }
    }

    cube.render();
}
