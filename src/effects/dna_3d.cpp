#include "cube.h"
#include "cube_geometry.h"
#include <cmath>

// DNA double helix wrapping around the cube

EffectDNA3D::EffectDNA3D() {
    rotation = 0;
}

void EffectDNA3D::render(Cube& cube, unsigned long deltaTime) {
    rotation += speed;
    if (rotation >= 360.0f) rotation -= 360.0f;

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Draw DNA helix on side faces (horizontal ring)
    // The helix wraps around: front -> right -> back -> left -> front

    for (int f = 0; f < 4; f++) {
        // Face order in the ring
        int faceIdx;
        switch (f) {
            case 0: faceIdx = 0; break;  // front
            case 1: faceIdx = 3; break;  // right
            case 2: faceIdx = 1; break;  // back
            case 3: faceIdx = 2; break;  // left
            default: faceIdx = 0;
        }

        for (int x = 0; x < 8; x++) {
            // Position along the helix (0-32 for full wrap)
            float helixPos = f * 8 + x;
            float angle = helixPos * helixTwist + rotation;

            // Two strands offset by 180 degrees
            for (int strand = 0; strand < 2; strand++) {
                float strandAngle = angle + strand * 180.0f;
                float radians = strandAngle * M_PI / 180.0f;

                // Y position based on sine wave
                float yPos = 3.5f + sin(radians) * radius;
                int y = (int)(yPos + 0.5f);

                if (y >= 0 && y < 8) {
                    // Strand color
                    uint32_t color = (strand == 0) ? 0xFF0066 : 0x00FF66;

                    // Brightness based on "depth"
                    float depth = cos(radians);
                    float brightness = 0.4f + (depth + 1.0f) * 0.3f;

                    uint8_t r = ((color >> 16) & 0xFF) * brightness;
                    uint8_t g = ((color >> 8) & 0xFF) * brightness;
                    uint8_t b = (color & 0xFF) * brightness;

                    faces[faceIdx]->setPixel(x, y, (r << 16) | (g << 8) | b);
                }
            }

            // Draw connecting "rungs" between strands
            float rungAngle = angle;
            float radians = rungAngle * M_PI / 180.0f;

            // Only draw rungs at certain intervals
            if (((int)helixPos % 2) == 0) {
                float y1 = 3.5f + sin(radians) * radius;
                float y2 = 3.5f + sin(radians + M_PI) * radius;

                int yMin = min((int)y1, (int)y2);
                int yMax = max((int)y1, (int)y2);

                for (int y = yMin; y <= yMax; y++) {
                    if (y >= 0 && y < 8) {
                        // Rung color (blue/cyan)
                        float brightness = 0.3f;
                        faces[faceIdx]->setPixel(x, y, (uint32_t)(0x004488 * brightness));
                    }
                }
            }
        }
    }

    // Top and bottom faces - show helix "end view" (circular pattern)
    for (int topBot = 0; topBot < 2; topBot++) {
        int faceIdx = (topBot == 0) ? 4 : 5;

        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                float cx = x - 3.5f;
                float cy = y - 3.5f;
                float dist = sqrt(cx * cx + cy * cy);
                float pixelAngle = atan2(cy, cx) * 180.0f / M_PI;

                // Check if pixel is on helix circle
                if (fabs(dist - radius) < 1.0f) {
                    float adjustedAngle = pixelAngle + rotation;
                    adjustedAngle = fmod(adjustedAngle, 360.0f);
                    if (adjustedAngle < 0) adjustedAngle += 360.0f;

                    // Two bright spots for the two strands
                    float diff1 = fabs(adjustedAngle);
                    float diff2 = fabs(adjustedAngle - 180.0f);
                    if (diff1 > 180) diff1 = 360 - diff1;
                    if (diff2 > 180) diff2 = 360 - diff2;

                    if (diff1 < 30) {
                        float brightness = 1.0f - diff1 / 30.0f;
                        faces[faceIdx]->setPixel(x, y, (uint32_t)(0xFF0066 * brightness));
                    } else if (diff2 < 30) {
                        float brightness = 1.0f - diff2 / 30.0f;
                        faces[faceIdx]->setPixel(x, y, (uint32_t)(0x00FF66 * brightness));
                    }
                }
            }
        }
    }

    cube.render();
}
