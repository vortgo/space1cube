#include "cube.h"
#include "cube_geometry.h"
#include <cmath>

// Wave that propagates across all cube faces from a center point

EffectWave3D::EffectWave3D() {
    time = 0;
    center = {0, 4, 4};  // Start from front center
}

void EffectWave3D::render(Cube& cube, unsigned long deltaTime) {
    time += waveSpeed;

    // Change center periodically
    if ((int)(time * 10) % 200 == 0) {
        center.face = random(6);
        center.x = random(8);
        center.y = random(8);
    }

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // For each pixel on each face, calculate wave brightness
    for (int f = 0; f < 6; f++) {
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                // Calculate distance from center (simplified)
                float dist;
                if (f == center.face) {
                    float dx = x - center.x;
                    float dy = y - center.y;
                    dist = sqrt(dx*dx + dy*dy);
                } else {
                    // Estimate distance through cube surface
                    dist = 8.0f + abs(x - center.x) + abs(y - center.y);
                    // Adjust based on face adjacency
                    if ((f == 0 && center.face == 4) || (f == 4 && center.face == 0) ||
                        (f == 0 && center.face == 5) || (f == 5 && center.face == 0)) {
                        dist = 4.0f + abs(7 - center.x - x) + abs(y - center.y);
                    }
                }

                // Wave function
                float wave = sin(dist * frequency - time);
                wave = (wave + 1.0f) / 2.0f;  // Normalize to 0-1

                // Color based on wave phase
                float hue = fmod(dist * 20.0f + time * 50.0f, 360.0f);
                uint32_t color = hsvToColor(hue, 1.0f, wave);

                faces[f]->setPixel(x, y, color);
            }
        }
    }

    cube.render();
}

uint32_t EffectWave3D::hsvToColor(float h, float s, float v) {
    float c = v * s;
    float x = c * (1 - fabs(fmod(h / 60.0f, 2) - 1));
    float m = v - c;

    float r, g, b;
    if (h < 60) { r = c; g = x; b = 0; }
    else if (h < 120) { r = x; g = c; b = 0; }
    else if (h < 180) { r = 0; g = c; b = x; }
    else if (h < 240) { r = 0; g = x; b = c; }
    else if (h < 300) { r = x; g = 0; b = c; }
    else { r = c; g = 0; b = x; }

    uint8_t R = (r + m) * 255;
    uint8_t G = (g + m) * 255;
    uint8_t B = (b + m) * 255;
    return (R << 16) | (G << 8) | B;
}
