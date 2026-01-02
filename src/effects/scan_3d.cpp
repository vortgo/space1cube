#include "cube.h"
#include "cube_geometry.h"
#include <cmath>

// Scanning plane that moves through the cube

EffectScan3D::EffectScan3D() {
    position = 0;
    direction = 1;
}

void EffectScan3D::render(Cube& cube, unsigned long deltaTime) {
    position += speed * direction;

    // Bounce at edges
    if (position >= 24.0f) {
        position = 24.0f;
        direction = -1;
        axis = (axis + 1) % 3;  // Change scan axis
    } else if (position <= 0.0f) {
        position = 0.0f;
        direction = 1;
    }

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Calculate which position in the "unfolded" cube we're at
    // Position 0-8: first side faces, 8-16: front/back, 16-24: top/bottom

    for (int f = 0; f < 6; f++) {
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                float dist = 0;

                // Calculate distance based on axis
                if (axis == 0) {
                    // X-axis scan (left to right)
                    if (f == 2) dist = x;              // left
                    else if (f == 0) dist = 8 + x;    // front
                    else if (f == 3) dist = 16 + x;   // right
                    else if (f == 1) dist = 24 - x;   // back (reverse)
                    else if (f == 4) dist = 8 + x;    // top
                    else if (f == 5) dist = 8 + x;    // bottom
                } else if (axis == 1) {
                    // Y-axis scan (bottom to top)
                    if (f == 5) dist = y;             // bottom
                    else if (f == 0) dist = 8 + y;    // front
                    else if (f == 4) dist = 16 + y;   // top
                    else if (f == 1) dist = 8 + y;    // back
                    else if (f == 2) dist = 8 + y;    // left
                    else if (f == 3) dist = 8 + y;    // right
                } else {
                    // Z-axis scan (back to front)
                    if (f == 1) dist = y;             // back
                    else if (f == 4) dist = 8 + y;    // top
                    else if (f == 0) dist = 16 + y;   // front
                    else if (f == 5) dist = 8 + (7 - y); // bottom
                    else if (f == 2) dist = 8 + x;    // left
                    else if (f == 3) dist = 8 + (7 - x); // right
                }

                // Calculate brightness based on distance from scan plane
                float diff = fabs(dist - position);
                float brightness = 0;

                if (diff < thickness) {
                    brightness = 1.0f - diff / thickness;
                }

                if (brightness > 0.05f) {
                    // Color based on position
                    float hue = fmod(position * 15.0f, 360.0f);
                    uint32_t color = hsvToColor(hue, 1.0f, brightness);
                    faces[f]->setPixel(x, y, color);
                }
            }
        }
    }

    cube.render();
}

uint32_t EffectScan3D::hsvToColor(float h, float s, float v) {
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
