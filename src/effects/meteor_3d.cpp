#include "cube.h"
#include "cube_geometry.h"

// Meteors flying across cube surface

EffectMeteor3D::EffectMeteor3D() {
    for (int i = 0; i < MAX_METEORS; i++) {
        meteors[i].active = false;
    }
    spawnTimer = 0;
}

void EffectMeteor3D::spawnMeteor() {
    for (int i = 0; i < MAX_METEORS; i++) {
        if (!meteors[i].active) {
            Meteor& m = meteors[i];
            m.active = true;

            // Start from random edge
            m.pos.face = random(6);
            int edge = random(4);  // which edge to start from
            switch (edge) {
                case 0: m.pos.x = 0; m.pos.y = random(8); m.dirX = 1; m.dirY = 0; break;
                case 1: m.pos.x = 7; m.pos.y = random(8); m.dirX = -1; m.dirY = 0; break;
                case 2: m.pos.x = random(8); m.pos.y = 0; m.dirX = 0; m.dirY = 1; break;
                case 3: m.pos.x = random(8); m.pos.y = 7; m.dirX = 0; m.dirY = -1; break;
            }

            m.speed = 0.2f + random(100) / 200.0f;
            m.length = 4 + random(6);
            m.hue = random(360);
            m.moveAccum = 0;
            m.traveled = 0;

            // Initialize trail
            for (int t = 0; t < 12; t++) {
                m.trail[t] = {-1, 0, 0};
            }
            return;
        }
    }
}

void EffectMeteor3D::render(Cube& cube, unsigned long deltaTime) {
    // Spawn new meteors
    spawnTimer += intensity;
    if (spawnTimer >= 1.0f) {
        spawnTimer = 0;
        spawnMeteor();
    }

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Update and draw meteors
    for (int i = 0; i < MAX_METEORS; i++) {
        if (!meteors[i].active) continue;

        Meteor& m = meteors[i];
        m.moveAccum += m.speed;

        while (m.moveAccum >= 1.0f) {
            m.moveAccum -= 1.0f;

            // Shift trail
            for (int t = 11; t > 0; t--) {
                m.trail[t] = m.trail[t-1];
            }
            m.trail[0] = m.pos;

            // Move meteor
            moveOnCube(m.pos, m.dirX, m.dirY);
            m.traveled++;

            // Deactivate if traveled too far
            if (m.traveled > 40) {
                m.active = false;
            }
        }

        if (!m.active) continue;

        // Draw meteor head
        uint32_t headColor = hsvToColor(m.hue, 0.3f, 1.0f);  // Bright, slightly desaturated
        faces[m.pos.face]->setPixel(m.pos.x, m.pos.y, headColor);

        // Draw trail
        for (int t = 0; t < m.length && t < 12; t++) {
            if (m.trail[t].face >= 0) {
                float brightness = 1.0f - (float)(t + 1) / m.length;
                uint32_t color = hsvToColor(m.hue, 1.0f, brightness * 0.8f);
                faces[m.trail[t].face]->setPixel(m.trail[t].x, m.trail[t].y, color);
            }
        }
    }

    cube.render();
}

uint32_t EffectMeteor3D::hsvToColor(float h, float s, float v) {
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
