#include "cube.h"
#include <cmath>

EffectCubeRotation::EffectCubeRotation() {
    angle = 0;
    faceColors[0] = 0xFF0000;  // front - red
    faceColors[1] = 0x00FF00;  // back - green
    faceColors[2] = 0x0000FF;  // left - blue
    faceColors[3] = 0xFFFF00;  // right - yellow
    faceColors[4] = 0xFF00FF;  // top - magenta
    faceColors[5] = 0x00FFFF;  // bottom - cyan
}

void EffectCubeRotation::rotateFaces(Cube& cube) {
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Calculate rotation phase (0-3 for 90 degree steps)
    int phase = ((int)(angle / 90.0f)) % 4;
    float subAngle = fmod(angle, 90.0f) / 90.0f;

    // Determine which faces are visible based on rotation axis
    uint32_t displayColors[6];
    for (int i = 0; i < 6; i++) {
        displayColors[i] = faceColors[i];
    }

    if (axis == 0) { // X axis rotation
        // Front -> Top -> Back -> Bottom -> Front
        int order[] = {0, 4, 1, 5};
        for (int i = 0; i < 4; i++) {
            int from = order[(i + phase) % 4];
            displayColors[order[i]] = faceColors[from];
        }
    } else if (axis == 1) { // Y axis rotation
        // Front -> Right -> Back -> Left -> Front
        int order[] = {0, 3, 1, 2};
        for (int i = 0; i < 4; i++) {
            int from = order[(i + phase) % 4];
            displayColors[order[i]] = faceColors[from];
        }
    } else { // Z axis rotation
        // Top -> Right -> Bottom -> Left -> Top
        int order[] = {4, 3, 5, 2};
        for (int i = 0; i < 4; i++) {
            int from = order[(i + phase) % 4];
            displayColors[order[i]] = faceColors[from];
        }
    }

    // Draw with gradient effect to simulate rotation
    for (int f = 0; f < 6; f++) {
        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                float brightness = 0.5f + 0.5f * cos(subAngle * M_PI);
                uint32_t c = displayColors[f];
                uint8_t r = (uint8_t)(((c >> 16) & 0xFF) * brightness);
                uint8_t g = (uint8_t)(((c >> 8) & 0xFF) * brightness);
                uint8_t b = (uint8_t)((c & 0xFF) * brightness);
                faces[f]->setPixel(x, y, (r << 16) | (g << 8) | b);
            }
        }
    }
}

void EffectCubeRotation::render(Cube& cube, unsigned long deltaTime) {
    angle += rotationSpeed * deltaTime;
    if (angle >= 360.0f) {
        angle -= 360.0f;
        axis = (axis + 1) % 3;
    }

    cube.clear();
    rotateFaces(cube);
    cube.render();
}
