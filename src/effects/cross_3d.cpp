#include "cube.h"
#include "cube_geometry.h"

// Test effect: 3 crossing strips using cube_geometry transitions
// Ring 1 (red): horizontal ring through the equator
// Ring 2 (green): vertical ring through right/left
// Ring 3 (blue): vertical ring through front/back

EffectCross3D::EffectCross3D() {
    pos1 = 0;
    pos2 = 0;
    pos3 = 0;
}

void EffectCross3D::render(Cube& cube, unsigned long deltaTime) {
    pos1 += speed * deltaTime;
    pos2 += speed * deltaTime * 0.7f;
    pos3 += speed * deltaTime * 1.3f;

    // Each ring is 32 pixels (4 faces * 8 pixels)
    while (pos1 >= 32.0f) pos1 -= 32.0f;
    while (pos2 >= 32.0f) pos2 -= 32.0f;
    while (pos3 >= 32.0f) pos3 -= 32.0f;

    cube.clear();

    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Ring 1 (RED): Horizontal ring - starts on front, moves right (dx=1)
    // Path: front → right → back → left → front
    {
        // Starting position: front face, center Y, moving right
        CubePos startPos = {0, 0, 3};  // front, x=0, y=3 (center)

        // Move to current position in the ring
        CubePos currentPos = startPos;
        int offset = (int)pos1;
        for (int step = 0; step < offset; step++) {
            moveOnCube(currentPos, 1, 0);  // move right
        }

        // Draw strip (4 pixels long, 2 pixels wide)
        for (int i = 0; i < stripLength; i++) {
            // Draw at current position (2 pixels wide in Y)
            for (int w = 0; w < stripWidth; w++) {
                CubePos drawPos = getNeighbor(currentPos.face, currentPos.x, currentPos.y, 0, w);
                faces[drawPos.face]->setPixel(drawPos.x, drawPos.y, color1);
            }
            // Move to next position
            moveOnCube(currentPos, 1, 0);
        }
    }

    // Ring 2 (GREEN): Vertical ring through right/left
    // Path: right(down) → bottom → left(up) → top → right
    {
        // Starting position: right face, center X, top, moving down
        CubePos startPos = {3, 3, 0};  // right, x=3 (center), y=0 (top)

        CubePos currentPos = startPos;
        int offset = (int)pos2;
        for (int step = 0; step < offset; step++) {
            moveOnCube(currentPos, 0, 1);  // move down
        }

        for (int i = 0; i < stripLength; i++) {
            for (int w = 0; w < stripWidth; w++) {
                CubePos drawPos = getNeighbor(currentPos.face, currentPos.x, currentPos.y, w, 0);
                faces[drawPos.face]->setPixel(drawPos.x, drawPos.y, color2);
            }
            moveOnCube(currentPos, 0, 1);
        }
    }

    // Ring 3 (BLUE): Vertical ring through front/back
    // Path: front(up) → top → back(down) → bottom → front
    {
        // Starting position: front face, center X, bottom, moving up
        CubePos startPos = {0, 3, 7};  // front, x=3 (center), y=7 (bottom)

        CubePos currentPos = startPos;
        int offset = (int)pos3;
        for (int step = 0; step < offset; step++) {
            moveOnCube(currentPos, 0, -1);  // move up
        }

        for (int i = 0; i < stripLength; i++) {
            for (int w = 0; w < stripWidth; w++) {
                CubePos drawPos = getNeighbor(currentPos.face, currentPos.x, currentPos.y, w, 0);
                faces[drawPos.face]->setPixel(drawPos.x, drawPos.y, color3);
            }
            moveOnCube(currentPos, 0, -1);
        }
    }

    cube.render();
}
