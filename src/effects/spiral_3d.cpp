#include "cube.h"
#include "cube_geometry.h"
#include <cmath>

// Spiral pattern that wraps around the cube

EffectSpiral3D::EffectSpiral3D() {
    angle = 0;
}

void EffectSpiral3D::render(Cube& cube, unsigned long deltaTime) {
    angle += speed;
    if (angle >= 360.0f) angle -= 360.0f;

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Create spiral effect across all faces
    for (int f = 0; f < 6; f++) {
        // Face offset for continuous spiral
        float faceOffset = f * 60.0f;  // 60 degrees per face

        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                // Calculate spiral position
                float cx = x - 3.5f;
                float cy = y - 3.5f;
                float dist = sqrt(cx*cx + cy*cy);
                float pixelAngle = atan2(cy, cx) * 180.0f / M_PI;

                // Spiral formula
                float spiralPhase = pixelAngle + dist * spiralTightness + angle + faceOffset;
                spiralPhase = fmod(spiralPhase, 360.0f);
                if (spiralPhase < 0) spiralPhase += 360.0f;

                // Create spiral arms
                float brightness = 0;
                for (int arm = 0; arm < arms; arm++) {
                    float armAngle = 360.0f / arms * arm;
                    float diff = fabs(spiralPhase - armAngle);
                    if (diff > 180) diff = 360 - diff;
                    if (diff < 30) {
                        brightness = 1.0f - diff / 30.0f;
                        break;
                    }
                }

                if (brightness > 0) {
                    // Rainbow color based on angle
                    float hue = fmod(spiralPhase + angle * 2, 360.0f);
                    uint32_t color = hsvToColor(hue, 1.0f, brightness);
                    faces[f]->setPixel(x, y, color);
                }
            }
        }
    }

    cube.render();
}

uint32_t EffectSpiral3D::hsvToColor(float h, float s, float v) {
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
