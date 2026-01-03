#include "cube.h"
#include "cube_geometry.h"

// Color wave that sweeps across the entire cube surface

EffectColorWipe3D::EffectColorWipe3D() {
    position = 0;
    hue = 0;
}

void EffectColorWipe3D::render(Cube& cube, unsigned long deltaTime) {
    position += speed * deltaTime;

    // Total length: 32 pixels around horizontal ring + 16 for top/bottom
    const float totalLength = 48.0f;
    if (position >= totalLength) {
        position -= totalLength;
        hue += 30; // Change color each cycle
        if (hue >= 360) hue -= 360;
    }

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Calculate wave position for each pixel
    for (int f = 0; f < 6; f++) {
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                float pixelPos = getPixelPosition(f, x, y);
                float dist = position - pixelPos;

                // Wrap around
                if (dist < -totalLength/2) dist += totalLength;
                if (dist > totalLength/2) dist -= totalLength;

                // Wave with trail
                float brightness = 0;
                if (dist >= 0 && dist < waveWidth) {
                    brightness = 1.0f - (dist / waveWidth);
                } else if (dist < 0 && dist > -trailLength) {
                    brightness = 0.3f * (1.0f + dist / trailLength);
                }

                if (brightness > 0) {
                    uint32_t color = hsvToColor((hue + (int)(dist * 5)) % 360, 1.0f, brightness);
                    faces[f]->setPixel(x, y, color);
                }
            }
        }
    }

    cube.render();
}

float EffectColorWipe3D::getPixelPosition(int face, int x, int y) {
    // Map each pixel to a position along the "unwrapped" cube surface
    // Horizontal ring: front(0-7) -> right(8-15) -> back(16-23) -> left(24-31)
    // Top/bottom extend from y position

    switch (face) {
        case 0: // front
            return (float)x + (7 - y) * 0.1f; // Slight y offset for wave effect
        case 3: // right
            return 8.0f + (float)x + (7 - y) * 0.1f;
        case 1: // back
            return 16.0f + (float)x + (7 - y) * 0.1f;
        case 2: // left
            return 24.0f + (float)x + (7 - y) * 0.1f;
        case 4: // top - wave from front edge
            return 32.0f + (float)y + x * 0.1f;
        case 5: // bottom - wave from front edge
            return 40.0f + (7 - y) + x * 0.1f;
    }
    return 0;
}

uint32_t EffectColorWipe3D::hsvToColor(int h, float s, float v) {
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
