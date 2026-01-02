#include "cube.h"
#include "cube_geometry.h"
#include <cmath>

// Pulsing effect from center of cube outward

EffectPulse3D::EffectPulse3D() {
    phase = 0;
}

void EffectPulse3D::render(Cube& cube, unsigned long deltaTime) {
    phase += speed;

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Multiple pulse rings
    for (int f = 0; f < 6; f++) {
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                // Distance from center of face
                float cx = x - 3.5f;
                float cy = y - 3.5f;
                float dist = sqrt(cx*cx + cy*cy);

                // Add face-based offset to create 3D effect
                float faceOffset = 0;
                if (f == 4) faceOffset = -4;       // top closer to center
                else if (f == 5) faceOffset = -4; // bottom closer to center
                else if (f == 1) faceOffset = 2;  // back further

                float totalDist = dist + faceOffset;

                // Multiple rings
                float brightness = 0;
                for (int ring = 0; ring < rings; ring++) {
                    float ringPhase = phase - ring * 3.0f;
                    float ringRadius = fmod(ringPhase, 12.0f);

                    float diff = fabs(totalDist - ringRadius);
                    if (diff < 1.5f) {
                        float ringBright = (1.0f - diff / 1.5f) * (1.0f - ring * 0.2f);
                        if (ringBright > brightness) brightness = ringBright;
                    }
                }

                if (brightness > 0.05f) {
                    // Color shifts with each pulse
                    float hue = fmod(phase * 10.0f + totalDist * 20.0f, 360.0f);
                    uint32_t color = hsvToColor(hue, 1.0f, brightness);
                    faces[f]->setPixel(x, y, color);
                }
            }
        }
    }

    cube.render();
}

uint32_t EffectPulse3D::hsvToColor(float h, float s, float v) {
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

    return ((uint8_t)((r + m) * 255) << 16) |
           ((uint8_t)((g + m) * 255) << 8) |
           (uint8_t)((b + m) * 255);
}
