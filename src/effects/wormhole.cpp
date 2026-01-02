#include "cube.h"
#include <cmath>

EffectWormhole::EffectWormhole() {
    depth = 0;
    rotation = 0;
}

uint32_t EffectWormhole::getColorAtDepth(float d) {
    // Shift colors based on depth
    float hue = fmod(d * 50 + rotation * 30, 360.0f);

    // HSV to RGB
    float c = 1.0f;
    float x = c * (1.0f - fabs(fmod(hue / 60.0f, 2.0f) - 1.0f));

    float r, g, b;
    if (hue < 60) { r = c; g = x; b = 0; }
    else if (hue < 120) { r = x; g = c; b = 0; }
    else if (hue < 180) { r = 0; g = c; b = x; }
    else if (hue < 240) { r = 0; g = x; b = c; }
    else if (hue < 300) { r = x; g = 0; b = c; }
    else { r = c; g = 0; b = x; }

    // Fade based on depth
    float fade = 0.3f + 0.7f * (1.0f - d / 10.0f);
    fade = fmax(0.0f, fmin(1.0f, fade));

    return ((uint8_t)(r * 255 * fade) << 16) |
           ((uint8_t)(g * 255 * fade) << 8) |
           (uint8_t)(b * 255 * fade);
}

void EffectWormhole::render(Cube& cube, unsigned long deltaTime) {
    depth += speed * deltaTime * 0.01f;
    rotation += speed * deltaTime * 0.005f;

    if (depth > 10) depth = 0;

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    float centerX = GRID_W / 2.0f;
    float centerY = GRID_H / 2.0f;

    // Draw tunnel effect on front and back
    for (int f = 0; f < 2; f++) {
        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                float dx = x - centerX;
                float dy = y - centerY;
                float dist = sqrt(dx * dx + dy * dy);

                // Ring pattern
                float ring = fmod(dist - depth + (f == 1 ? 5 : 0), tunnelRadius);
                float brightness = 1.0f - ring / tunnelRadius;
                brightness = fmax(0.0f, brightness);

                // Spiral
                float angle = atan2(dy, dx) + rotation;
                float spiral = sin(angle * 3 + depth) * 0.3f + 0.7f;

                if (brightness > 0.1f && dist < tunnelRadius * 2) {
                    uint32_t c = getColorAtDepth(dist + depth);
                    uint8_t r = (uint8_t)(((c >> 16) & 0xFF) * brightness * spiral);
                    uint8_t g = (uint8_t)(((c >> 8) & 0xFF) * brightness * spiral);
                    uint8_t b = (uint8_t)((c & 0xFF) * brightness * spiral);
                    faces[f]->setPixel(x, y, (r << 16) | (g << 8) | b);
                }
            }
        }
    }

    // Side faces show the tunnel walls
    for (int f = 2; f < 6; f++) {
        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                float d = (f < 4) ? (float)x / GRID_W : (float)y / GRID_H;
                float wave = sin(d * 6.28f + depth + rotation) * 0.5f + 0.5f;

                uint32_t c = getColorAtDepth(d * 5 + depth);
                uint8_t r = (uint8_t)(((c >> 16) & 0xFF) * wave * 0.5f);
                uint8_t g = (uint8_t)(((c >> 8) & 0xFF) * wave * 0.5f);
                uint8_t b = (uint8_t)((c & 0xFF) * wave * 0.5f);
                faces[f]->setPixel(x, y, (r << 16) | (g << 8) | b);
            }
        }
    }

    cube.render();
}
