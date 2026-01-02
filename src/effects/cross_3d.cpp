#include "cube.h"
#include "cube_geometry.h"

// Test effect: 3 crossing strips using cube_geometry transitions
// Each ring needs direction change when transitioning between faces

EffectCross3D::EffectCross3D() {
    pos1 = 0;
    pos2 = 0;
    pos3 = 0;
}

// Move one step along a ring, adjusting direction based on current face
// ringType: 0 = horizontal (dx), 1 = vertical right/left, 2 = vertical front/back
void moveAlongRing(CubePos& pos, int ringType) {
    int dx = 0, dy = 0;

    if (ringType == 0) {
        // Horizontal ring: always move dx=+1
        dx = 1;
    }
    else if (ringType == 1) {
        // Vertical ring through right/left/top/bottom
        // right: dy=+1 (down), bottom: dx=+1, left: dy=-1 (up), top: dx=-1
        switch (pos.face) {
            case 3: dy = 1; break;   // right: down
            case 5: dx = 1; break;   // bottom: toward left (dx+)
            case 2: dy = -1; break;  // left: up
            case 4: dx = -1; break;  // top: toward right (dx-)
            default: dy = 1; break;
        }
    }
    else if (ringType == 2) {
        // Vertical ring through front/top/back/bottom
        // front: dy=-1 (up), top: dy=+1 (toward back), back: dy=+1 (down), bottom: dy=-1 (toward front)
        switch (pos.face) {
            case 0: dy = -1; break;  // front: up
            case 4: dy = 1; break;   // top: toward back
            case 1: dy = 1; break;   // back: down
            case 5: dy = -1; break;  // bottom: toward front
            default: dy = -1; break;
        }
    }

    moveOnCube(pos, dx, dy);
}

void EffectCross3D::render(Cube& cube, unsigned long deltaTime) {
    pos1 += speed * deltaTime;
    pos2 += speed * deltaTime * 0.7f;
    pos3 += speed * deltaTime * 1.3f;

    while (pos1 >= 32.0f) pos1 -= 32.0f;
    while (pos2 >= 32.0f) pos2 -= 32.0f;
    while (pos3 >= 32.0f) pos3 -= 32.0f;

    cube.clear();

    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Ring 1 (RED): Horizontal ring
    // Path: front → right → back → left → front (moving dx=+1 always)
    {
        CubePos startPos = {0, 0, 3};  // front, x=0, y=3
        CubePos currentPos = startPos;

        int offset = (int)pos1;
        for (int step = 0; step < offset; step++) {
            moveAlongRing(currentPos, 0);
        }

        for (int i = 0; i < stripLength; i++) {
            for (int w = 0; w < stripWidth; w++) {
                CubePos drawPos = getNeighbor(currentPos.face, currentPos.x, currentPos.y, 0, w);
                faces[drawPos.face]->setPixel(drawPos.x, drawPos.y, color1);
            }
            moveAlongRing(currentPos, 0);
        }
    }

    // Ring 2 (GREEN): Vertical ring through right/left
    // Path: right(down) → bottom(dx+) → left(up) → top(dx-) → right
    {
        CubePos startPos = {3, 3, 0};  // right, x=3, y=0
        CubePos currentPos = startPos;

        int offset = (int)pos2;
        for (int step = 0; step < offset; step++) {
            moveAlongRing(currentPos, 1);
        }

        for (int i = 0; i < stripLength; i++) {
            for (int w = 0; w < stripWidth; w++) {
                CubePos drawPos = getNeighbor(currentPos.face, currentPos.x, currentPos.y, w, 0);
                faces[drawPos.face]->setPixel(drawPos.x, drawPos.y, color2);
            }
            moveAlongRing(currentPos, 1);
        }
    }

    // Ring 3 (BLUE): Vertical ring through front/back
    // Path: front(up) → top(dy+) → back(down) → bottom(dy-) → front
    {
        CubePos startPos = {0, 3, 7};  // front, x=3, y=7
        CubePos currentPos = startPos;

        int offset = (int)pos3;
        for (int step = 0; step < offset; step++) {
            moveAlongRing(currentPos, 2);
        }

        for (int i = 0; i < stripLength; i++) {
            for (int w = 0; w < stripWidth; w++) {
                CubePos drawPos = getNeighbor(currentPos.face, currentPos.x, currentPos.y, w, 0);
                faces[drawPos.face]->setPixel(drawPos.x, drawPos.y, color3);
            }
            moveAlongRing(currentPos, 2);
        }
    }

    cube.render();
}
