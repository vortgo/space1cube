#include "cube.h"
#include "cube_geometry.h"

// Multiple particles orbiting around the cube's horizontal ring

EffectOrbits3D::EffectOrbits3D() {
    // Initialize orbits with different speeds and colors
    uint32_t colors[] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF};
    for (int i = 0; i < MAX_ORBITS; i++) {
        orbits[i].position = i * (32.0f / MAX_ORBITS);  // Spread evenly
        orbits[i].speed = 0.08f + (i * 0.02f);  // Different speeds
        orbits[i].color = colors[i % 5];
        orbits[i].trailLength = 6;
    }
}

void EffectOrbits3D::render(Cube& cube, unsigned long deltaTime) {
    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Update and draw each orbit
    for (int o = 0; o < orbitCount; o++) {
        // Update position
        orbits[o].position += orbits[o].speed * speed * deltaTime * 0.01f;
        if (orbits[o].position >= 32.0f) {
            orbits[o].position -= 32.0f;
        }

        // Draw trail
        for (int t = 0; t < orbits[o].trailLength; t++) {
            float trailPos = orbits[o].position - t;
            if (trailPos < 0) trailPos += 32.0f;

            // Convert position to face and coordinates
            int face, x, y;
            positionToFaceXY(trailPos, face, x, y);

            // Dim color for trail
            float brightness = 1.0f - (float)t / orbits[o].trailLength;
            uint32_t color = dimColor(orbits[o].color, brightness);

            // Draw on horizontal ring (y = ringY)
            faces[face]->setPixel(x, ringY, color);

            // Optional: draw slightly above and below for thickness
            if (ringY > 0) {
                faces[face]->setPixel(x, ringY - 1, dimColor(color, 0.3f));
            }
            if (ringY < 7) {
                faces[face]->setPixel(x, ringY + 1, dimColor(color, 0.3f));
            }
        }
    }

    cube.render();
}

void EffectOrbits3D::positionToFaceXY(float pos, int& face, int& x, int& y) {
    // Position 0-32 maps to:
    // 0-7: front (x=0-7)
    // 8-15: right (x=0-7)
    // 16-23: back (x=0-7)
    // 24-31: left (x=0-7)
    int ipos = (int)pos;
    if (ipos < 8) {
        face = 0;  // front
        x = ipos;
    } else if (ipos < 16) {
        face = 3;  // right
        x = ipos - 8;
    } else if (ipos < 24) {
        face = 1;  // back
        x = ipos - 16;
    } else {
        face = 2;  // left
        x = ipos - 24;
    }
    y = ringY;
}

uint32_t EffectOrbits3D::dimColor(uint32_t color, float factor) {
    uint8_t r = ((color >> 16) & 0xFF) * factor;
    uint8_t g = ((color >> 8) & 0xFF) * factor;
    uint8_t b = (color & 0xFF) * factor;
    return (r << 16) | (g << 8) | b;
}
