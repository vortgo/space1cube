#include "cube.h"

// Test effect: 3 crossing strips to verify face transitions
// Ring 1 (red): horizontal through back→left→front→right (faces 2→3→1→4 in user numbering)
// Ring 2 (green): vertical through top→right→bottom→left (faces 5→4→6→3)
// Ring 3 (blue): vertical through front→top→back→bottom (faces 1→5→2→6)

EffectCross3D::EffectCross3D() {
    pos1 = 0;
    pos2 = 0;
    pos3 = 0;
}

void EffectCross3D::render(Cube& cube, unsigned long deltaTime) {
    // Update positions
    pos1 += speed * deltaTime;
    pos2 += speed * deltaTime * 0.7f;  // Slightly different speed
    pos3 += speed * deltaTime * 1.3f;  // Slightly different speed

    // Wrap around (4 faces * 8 pixels = 32)
    while (pos1 >= 32.0f) pos1 -= 32.0f;
    while (pos2 >= 32.0f) pos2 -= 32.0f;
    while (pos3 >= 32.0f) pos3 -= 32.0f;

    cube.clear();

    // Face references for easy access
    // Code indices: 0=front, 1=back, 2=left, 3=right, 4=top, 5=bottom
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Ring 1: Horizontal ring through back(1)→left(2)→front(0)→right(3)
    // Strip moves in X direction, constant Y (center: y=3,4)
    {
        int ringFaces[] = {1, 2, 0, 3};  // back, left, front, right
        int startX = (int)pos1;

        for (int i = 0; i < stripLength; i++) {
            int ringPos = (startX + i) % 32;
            int faceIdx = ringFaces[ringPos / 8];
            int x = ringPos % 8;

            // Draw 2-pixel wide strip (y = 3, 4)
            for (int w = 0; w < stripWidth; w++) {
                int y = 3 + w;
                faces[faceIdx]->setPixel(x, y, color1);
            }
        }
    }

    // Ring 2: Vertical ring through top(4)→right(3)→bottom(5)→left(2)
    // This ring goes: top(moving in Y+) → right(moving in Y+) → bottom(moving in Y-) → left(moving in Y-)
    {
        int startPos = (int)pos2;

        for (int i = 0; i < stripLength; i++) {
            int ringPos = (startPos + i) % 32;
            int segment = ringPos / 8;
            int localPos = ringPos % 8;

            int faceIdx, x, y;

            // X position: center (x=3,4)
            switch (segment) {
                case 0: // top: y goes 0→7 (front to back), x=3,4
                    faceIdx = 4;
                    x = 3;
                    y = localPos;
                    break;
                case 1: // right: y goes 0→7, x=3,4
                    faceIdx = 3;
                    x = 3;
                    y = localPos;
                    break;
                case 2: // bottom: y goes 7→0 (back to front), x=3,4
                    faceIdx = 5;
                    x = 3;
                    y = 7 - localPos;
                    break;
                case 3: // left: y goes 7→0, x=3,4
                    faceIdx = 2;
                    x = 3;
                    y = 7 - localPos;
                    break;
                default:
                    continue;
            }

            // Draw 2-pixel wide strip
            for (int w = 0; w < stripWidth; w++) {
                faces[faceIdx]->setPixel(x + w, y, color2);
            }
        }
    }

    // Ring 3: Vertical ring through front(0)→top(4)→back(1)→bottom(5)
    {
        int startPos = (int)pos3;

        for (int i = 0; i < stripLength; i++) {
            int ringPos = (startPos + i) % 32;
            int segment = ringPos / 8;
            int localPos = ringPos % 8;

            int faceIdx, x, y;

            switch (segment) {
                case 0: // front: y goes 7→0 (bottom to top), x=3,4
                    faceIdx = 0;
                    x = 3;
                    y = 7 - localPos;
                    break;
                case 1: // top: y goes 0→7, but x is inverted due to 180° rotation
                    // front x=3 connects to top x=4 (W-1-x), going toward back (y increasing)
                    faceIdx = 4;
                    x = 4;  // W-1-3 = 4
                    y = localPos;
                    break;
                case 2: // back: y goes 0→7 (top to bottom)
                    faceIdx = 1;
                    x = 3;
                    y = localPos;
                    break;
                case 3: // bottom: y goes 7→0, x is inverted
                    faceIdx = 5;
                    x = 4;  // W-1-3 = 4
                    y = 7 - localPos;
                    break;
                default:
                    continue;
            }

            // Draw 2-pixel wide strip
            for (int w = 0; w < stripWidth; w++) {
                faces[faceIdx]->setPixel(x + w, y, color3);
            }
        }
    }

    cube.render();
}
