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

    // Ring 1 (RED): Horizontal ring through back(1)→left(2)→front(0)→right(3)
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

    // Ring 2 (GREEN): Vertical ring through top(4)→right(3)→bottom(5)→left(2)
    // On right/left: moves in Y, constant X (center x=3,4)
    // On top/bottom: moves in X, constant Y (center y=3,4) - perpendicular to blue!
    {
        int startPos = (int)pos2;

        for (int i = 0; i < stripLength; i++) {
            int ringPos = (startPos + i) % 32;
            int segment = ringPos / 8;
            int localPos = ringPos % 8;

            int faceIdx, x, y;

            switch (segment) {
                case 0: // top: x goes 0→7 (right to left), constant y=3,4
                    faceIdx = 4;
                    x = localPos;
                    y = 3;
                    break;
                case 1: // right: y goes 0→7, constant x=3,4
                    faceIdx = 3;
                    x = 3;
                    y = localPos;
                    break;
                case 2: // bottom: x goes 7→0 (left to right), constant y=3,4
                    faceIdx = 5;
                    x = 7 - localPos;
                    y = 3;
                    break;
                case 3: // left: y goes 7→0, constant x=3,4
                    faceIdx = 2;
                    x = 3;
                    y = 7 - localPos;
                    break;
                default:
                    continue;
            }

            // Draw 2-pixel wide strip
            for (int w = 0; w < stripWidth; w++) {
                if (segment == 0 || segment == 2) {
                    // On top/bottom: width in Y direction
                    faces[faceIdx]->setPixel(x, y + w, color2);
                } else {
                    // On right/left: width in X direction
                    faces[faceIdx]->setPixel(x + w, y, color2);
                }
            }
        }
    }

    // Ring 3 (BLUE): Vertical ring through front(0)→top(4)→back(1)→bottom(5)
    // Direction: front(UP) → top(toward back) → back(DOWN) → bottom(toward front)
    // On front/back: moves in Y, constant X (center x=3,4)
    // On top/bottom: moves in Y, constant X (need to account for 180° rotation)
    {
        int startPos = (int)pos3;

        for (int i = 0; i < stripLength; i++) {
            int ringPos = (startPos + i) % 32;
            int segment = ringPos / 8;
            int localPos = ringPos % 8;

            int faceIdx, x, y;

            switch (segment) {
                case 0: // front: y goes 7→0 (going UP toward top)
                    faceIdx = 0;
                    x = 3;
                    y = 7 - localPos;
                    break;
                case 1: // top: y goes 0→7 (from front edge toward back edge)
                    // front x=3 → top x=W-1-3=4 (due to 180° rotation)
                    // top y=0 is near front, y=7 is near back
                    faceIdx = 4;
                    x = 4;
                    y = localPos;
                    break;
                case 2: // back: y goes 0→7 (going DOWN from top)
                    // top x=4 enters back... back is not rotated relative to top exit
                    // back x should match visually
                    faceIdx = 1;
                    x = 3;
                    y = localPos;
                    break;
                case 3: // bottom: y goes 7→0 (from back edge toward front edge)
                    // back x=3 → bottom x=3 (same, not inverted for back→bottom)
                    // bottom y=7 is near back, y=0 is near front
                    faceIdx = 5;
                    x = 4;
                    y = 7 - localPos;
                    break;
                default:
                    continue;
            }

            // Draw 2-pixel wide strip (width in X direction)
            for (int w = 0; w < stripWidth; w++) {
                faces[faceIdx]->setPixel(x + w, y, color3);
            }
        }
    }

    cube.render();
}
