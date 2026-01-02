#include "cube.h"
#include <cmath>

EffectSpinningInnerCube::EffectSpinningInnerCube() {
    angleX = 0;
    angleY = 0;
    angleZ = 0;
    cubeColor = 0x00FFFF;
}

void EffectSpinningInnerCube::project3DPoint(float x, float y, float z, int& px, int& py, float& depth) {
    // Rotate around X
    float cosX = cos(angleX), sinX = sin(angleX);
    float y1 = y * cosX - z * sinX;
    float z1 = y * sinX + z * cosX;

    // Rotate around Y
    float cosY = cos(angleY), sinY = sin(angleY);
    float x2 = x * cosY + z1 * sinY;
    float z2 = -x * sinY + z1 * cosY;

    // Rotate around Z
    float cosZ = cos(angleZ), sinZ = sin(angleZ);
    float x3 = x2 * cosZ - y1 * sinZ;
    float y3 = x2 * sinZ + y1 * cosZ;

    // Project to 2D (simple orthographic)
    px = (int)(x3 + GRID_W / 2);
    py = (int)(y3 + GRID_H / 2);
    depth = z2;
}

void EffectSpinningInnerCube::render(Cube& cube, unsigned long deltaTime) {
    angleX += speed * deltaTime * 0.001f;
    angleY += speed * deltaTime * 0.0013f;
    angleZ += speed * deltaTime * 0.0007f;

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Inner cube vertices
    float half = cubeSize / 2.0f;
    float vertices[8][3] = {
        {-half, -half, -half}, {half, -half, -half},
        {half, half, -half}, {-half, half, -half},
        {-half, -half, half}, {half, -half, half},
        {half, half, half}, {-half, half, half}
    };

    // Edges
    int edges[12][2] = {
        {0,1}, {1,2}, {2,3}, {3,0},
        {4,5}, {5,6}, {6,7}, {7,4},
        {0,4}, {1,5}, {2,6}, {3,7}
    };

    // Draw on front face (main view)
    for (int e = 0; e < 12; e++) {
        int v1 = edges[e][0];
        int v2 = edges[e][1];

        int x1, y1, x2, y2;
        float d1, d2;
        project3DPoint(vertices[v1][0], vertices[v1][1], vertices[v1][2], x1, y1, d1);
        project3DPoint(vertices[v2][0], vertices[v2][1], vertices[v2][2], x2, y2, d2);

        // Draw line using Bresenham
        int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
        int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
        int err = dx + dy;

        float avgDepth = (d1 + d2) / 2.0f;
        float brightness = 0.5f + avgDepth / (cubeSize * 2);
        brightness = fmax(0.3f, fmin(1.0f, brightness));

        uint8_t r = (uint8_t)(((cubeColor >> 16) & 0xFF) * brightness);
        uint8_t g = (uint8_t)(((cubeColor >> 8) & 0xFF) * brightness);
        uint8_t b = (uint8_t)((cubeColor & 0xFF) * brightness);
        uint32_t c = (r << 16) | (g << 8) | b;

        while (true) {
            if (x1 >= 0 && x1 < GRID_W && y1 >= 0 && y1 < GRID_H) {
                // Draw on all faces
                for (int f = 0; f < 6; f++) {
                    faces[f]->setPixel(x1, y1, c);
                }
            }
            if (x1 == x2 && y1 == y2) break;
            int e2 = 2 * err;
            if (e2 >= dy) { err += dy; x1 += sx; }
            if (e2 <= dx) { err += dx; y1 += sy; }
        }
    }

    cube.render();
}
