#include "cube.h"
#include "cube_geometry.h"

// Rolling ball that travels across all faces seamlessly

EffectRollingBall3D::EffectRollingBall3D() {
    pos = {0, 4, 4};  // Start on front face, center
    dirX = 1;
    dirY = 0;
    moveTimer = 0;
    ballColor = 0xFF0000;
    trail.clear();
}

void EffectRollingBall3D::render(Cube& cube, unsigned long deltaTime) {
    moveTimer += speed;

    while (moveTimer >= 1.0f) {
        moveTimer -= 1.0f;

        // Save current position to trail
        trail.push_back(pos);
        if (trail.size() > (size_t)trailLength) {
            trail.erase(trail.begin());
        }

        // Move ball
        moveOnCube(pos, dirX, dirY);

        // Random direction change (20% chance)
        if (random(100) < 20) {
            // Turn 90 degrees
            int temp = dirX;
            dirX = (random(2) == 0) ? dirY : -dirY;
            dirY = (dirX == 0) ? temp : -temp;
            if (dirX == 0 && dirY == 0) { dirX = 1; }  // Safety
        }

        // Change color when changing face
        static int lastFace = pos.face;
        if (pos.face != lastFace) {
            hue += 30;
            if (hue >= 360) hue -= 360;
            ballColor = hsvToRgb(hue, 255, 255);
            lastFace = pos.face;
        }
    }

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Draw trail with fading
    for (size_t i = 0; i < trail.size(); i++) {
        float brightness = (float)(i + 1) / trail.size() * 0.5f;
        uint32_t c = dimColor(ballColor, brightness);
        faces[trail[i].face]->setPixel(trail[i].x, trail[i].y, c);
    }

    // Draw ball (2x2 or 3x3)
    for (int dx = 0; dx < ballSize; dx++) {
        for (int dy = 0; dy < ballSize; dy++) {
            CubePos p = getNeighbor(pos.face, pos.x, pos.y, dx, dy);
            faces[p.face]->setPixel(p.x, p.y, ballColor);
        }
    }

    cube.render();
}

uint32_t EffectRollingBall3D::hsvToRgb(int h, int s, int v) {
    float hf = h / 60.0f;
    int i = (int)hf;
    float f = hf - i;
    int p = v * (255 - s) / 255;
    int q = v * (255 - s * f) / 255;
    int t = v * (255 - s * (1 - f)) / 255;

    int r, g, b;
    switch (i % 6) {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
        default: r = g = b = 0;
    }
    return (r << 16) | (g << 8) | b;
}

uint32_t EffectRollingBall3D::dimColor(uint32_t color, float factor) {
    uint8_t r = ((color >> 16) & 0xFF) * factor;
    uint8_t g = ((color >> 8) & 0xFF) * factor;
    uint8_t b = (color & 0xFF) * factor;
    return (r << 16) | (g << 8) | b;
}
