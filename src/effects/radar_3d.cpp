#include "cube.h"
#include "cube_geometry.h"
#include <cmath>

// Radar beam sweeping around the horizontal ring of the cube

EffectRadar3D::EffectRadar3D() {
    angle = 0;
    // Clear blips
    for (int i = 0; i < MAX_BLIPS; i++) {
        blips[i].active = false;
    }
}

void EffectRadar3D::render(Cube& cube, unsigned long deltaTime) {
    float dt = deltaTime / 1000.0f;
    angle += rotationSpeed * dt;
    if (angle >= 32.0f) angle -= 32.0f;

    // Randomly spawn blips
    if (random(1000) < (int)(blipChance * 1000 * dt)) {
        spawnBlip();
    }

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Draw radar sweep on horizontal ring (faces 0, 1, 2, 3)
    for (int f = 0; f < 4; f++) {
        for (int x = 0; x < 8; x++) {
            // Calculate position in the 32-unit ring
            float pos = f * 8.0f + x;

            // Distance from beam
            float dist = pos - angle;
            if (dist < -16) dist += 32;
            if (dist > 16) dist -= 32;

            if (dist >= 0 && dist < beamWidth) {
                // Main beam
                float brightness = 1.0f - (dist / beamWidth);
                uint32_t color = dimColor(0x00FF00, brightness);

                for (int y = 0; y < 8; y++) {
                    faces[f]->setPixel(x, y, color);
                }
            } else if (dist >= 0 && dist < trailLength) {
                // Trail
                float brightness = 0.3f * (1.0f - dist / trailLength);
                uint32_t color = dimColor(0x004400, brightness);

                for (int y = 0; y < 8; y++) {
                    uint32_t existing = 0; // Could blend but just overwrite for simplicity
                    faces[f]->setPixel(x, y, color);
                }
            }
        }
    }

    // Draw center dot pattern on top and bottom
    drawCenterPattern(cube.top, 0x003300);
    drawCenterPattern(cube.bottom, 0x003300);

    // Draw and fade blips
    for (int i = 0; i < MAX_BLIPS; i++) {
        if (!blips[i].active) continue;

        blips[i].life -= dt * 0.5f;
        if (blips[i].life <= 0) {
            blips[i].active = false;
            continue;
        }

        uint32_t color = dimColor(0x00FF00, blips[i].life);
        faces[blips[i].face]->setPixel(blips[i].x, blips[i].y, color);
    }

    cube.render();
}

void EffectRadar3D::drawCenterPattern(Matrix& face, uint32_t color) {
    // Draw concentric circles pattern
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            float dx = x - 3.5f;
            float dy = y - 3.5f;
            float dist = sqrt(dx*dx + dy*dy);

            // Draw rings at certain distances
            if ((int)(dist * 2) % 3 == 0) {
                face.setPixel(x, y, color);
            }
        }
    }

    // Draw cross
    for (int i = 0; i < 8; i++) {
        face.setPixel(3, i, color);
        face.setPixel(4, i, color);
        face.setPixel(i, 3, color);
        face.setPixel(i, 4, color);
    }
}

void EffectRadar3D::spawnBlip() {
    for (int i = 0; i < MAX_BLIPS; i++) {
        if (!blips[i].active) {
            blips[i].active = true;
            blips[i].face = random(4); // Only on horizontal ring
            blips[i].x = random(8);
            blips[i].y = random(8);
            blips[i].life = 1.0f;
            return;
        }
    }
}

uint32_t EffectRadar3D::dimColor(uint32_t color, float factor) {
    if (factor < 0) factor = 0;
    if (factor > 1) factor = 1;
    uint8_t r = ((color >> 16) & 0xFF) * factor;
    uint8_t g = ((color >> 8) & 0xFF) * factor;
    uint8_t b = (color & 0xFF) * factor;
    return (r << 16) | (g << 8) | b;
}
