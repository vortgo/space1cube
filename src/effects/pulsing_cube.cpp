#include "cube.h"
#include <cmath>

EffectPulsingCube::EffectPulsingCube() {
    phase = 0;
    breathCycle = 0;
    baseColor = getRandomColor();
}

uint32_t EffectPulsingCube::getRandomColor() {
    uint32_t colors[] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF, 0x00FFFF, 0xFFFFFF};
    return colors[random(7)];
}

void EffectPulsingCube::render(Cube& cube, unsigned long deltaTime) {
    phase += pulseSpeed * deltaTime * 0.01f;
    breathCycle += 0.001f * deltaTime;

    // Double beat like heartbeat
    float beat1 = sin(phase);
    float beat2 = sin(phase * 2 + 1.5f) * 0.5f;
    float combinedBeat = (beat1 + beat2) * 0.5f + 0.5f;

    float brightness = minBrightness + (1.0f - minBrightness) * combinedBeat;

    // Change color periodically
    if (random(1000) < 2) {
        baseColor = getRandomColor();
    }

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Calculate size of lit area based on pulse
    float size = 2.0f + combinedBeat * 3.0f;
    float centerX = GRID_W / 2.0f;
    float centerY = GRID_H / 2.0f;

    for (int f = 0; f < 6; f++) {
        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                float dx = x - centerX;
                float dy = y - centerY;
                float dist = sqrt(dx * dx + dy * dy);

                if (dist <= size) {
                    // Brighter toward center
                    float distFactor = 1.0f - dist / size;
                    float finalBright = brightness * distFactor;

                    uint8_t r = (uint8_t)(((baseColor >> 16) & 0xFF) * finalBright);
                    uint8_t g = (uint8_t)(((baseColor >> 8) & 0xFF) * finalBright);
                    uint8_t b = (uint8_t)((baseColor & 0xFF) * finalBright);

                    faces[f]->setPixel(x, y, (r << 16) | (g << 8) | b);
                }
            }
        }
    }

    cube.render();
}
