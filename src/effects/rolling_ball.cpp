#include "cube.h"

// Face indices: 0=front, 1=back, 2=left, 3=right, 4=top, 5=bottom
// Adjacency map: when leaving edge, which face and how coordinates transform
// Format: {nextFace, flipX, flipY, swapXY}

EffectRollingBall::EffectRollingBall() {
    pos.face = 0;
    pos.x = GRID_W / 2;
    pos.y = GRID_H / 2;
    dirX = 1;
    dirY = 0;
    moveTimer = 0;
    ballColor = getRandomColor();
}

uint32_t EffectRollingBall::getRandomColor() {
    uint32_t colors[] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF, 0x00FFFF};
    return colors[random(6)];
}

void EffectRollingBall::transitionFace() {
    // Transition logic based on face and direction
    int newFace = pos.face;
    int newX = pos.x;
    int newY = pos.y;

    // Front (0) transitions
    if (pos.face == 0) {
        if (pos.x < 0) { newFace = 2; newX = GRID_W - 1; } // to left
        else if (pos.x >= GRID_W) { newFace = 3; newX = 0; } // to right
        else if (pos.y < 0) { newFace = 4; newY = GRID_H - 1; newX = pos.x; } // to top
        else if (pos.y >= GRID_H) { newFace = 5; newY = 0; newX = pos.x; } // to bottom
    }
    // Back (1) transitions
    else if (pos.face == 1) {
        if (pos.x < 0) { newFace = 3; newX = GRID_W - 1; }
        else if (pos.x >= GRID_W) { newFace = 2; newX = 0; }
        else if (pos.y < 0) { newFace = 4; newY = 0; newX = GRID_W - 1 - pos.x; }
        else if (pos.y >= GRID_H) { newFace = 5; newY = GRID_H - 1; newX = GRID_W - 1 - pos.x; }
    }
    // Left (2) transitions
    else if (pos.face == 2) {
        if (pos.x < 0) { newFace = 1; newX = GRID_W - 1; }
        else if (pos.x >= GRID_W) { newFace = 0; newX = 0; }
        else if (pos.y < 0) { newFace = 4; newY = pos.x; newX = 0; }
        else if (pos.y >= GRID_H) { newFace = 5; newY = GRID_H - 1 - pos.x; newX = 0; }
    }
    // Right (3) transitions
    else if (pos.face == 3) {
        if (pos.x < 0) { newFace = 0; newX = GRID_W - 1; }
        else if (pos.x >= GRID_W) { newFace = 1; newX = 0; }
        else if (pos.y < 0) { newFace = 4; newY = GRID_W - 1 - pos.x; newX = GRID_W - 1; }
        else if (pos.y >= GRID_H) { newFace = 5; newY = pos.x; newX = GRID_W - 1; }
    }
    // Top (4) transitions
    else if (pos.face == 4) {
        if (pos.x < 0) { newFace = 2; newX = pos.y; newY = 0; }
        else if (pos.x >= GRID_W) { newFace = 3; newX = GRID_W - 1 - pos.y; newY = 0; }
        else if (pos.y < 0) { newFace = 1; newY = 0; newX = GRID_W - 1 - pos.x; }
        else if (pos.y >= GRID_H) { newFace = 0; newY = 0; }
    }
    // Bottom (5) transitions
    else if (pos.face == 5) {
        if (pos.x < 0) { newFace = 2; newX = GRID_W - 1 - pos.y; newY = GRID_H - 1; }
        else if (pos.x >= GRID_W) { newFace = 3; newX = pos.y; newY = GRID_H - 1; }
        else if (pos.y < 0) { newFace = 0; newY = GRID_H - 1; }
        else if (pos.y >= GRID_H) { newFace = 1; newY = GRID_H - 1; newX = GRID_W - 1 - pos.x; }
    }

    if (newFace != pos.face) {
        ballColor = getRandomColor();
    }

    pos.face = newFace;
    pos.x = newX;
    pos.y = newY;
}

void EffectRollingBall::moveBall() {
    pos.x += dirX;
    pos.y += dirY;

    // Check if we need to transition
    if (pos.x < 0 || pos.x >= GRID_W || pos.y < 0 || pos.y >= GRID_H) {
        transitionFace();
    }

    // Random direction change
    if (random(100) < 10) {
        int temp = dirX;
        dirX = -dirY;
        dirY = temp;
    }
}

void EffectRollingBall::render(Cube& cube, unsigned long deltaTime) {
    moveTimer += speed;
    if (moveTimer >= 1.0f) {
        moveTimer = 0;
        moveBall();
    }

    // Clear and draw
    cube.clear();

    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};
    Matrix& face = *faces[pos.face];

    // Draw ball
    for (int dx = 0; dx < ballSize; dx++) {
        for (int dy = 0; dy < ballSize; dy++) {
            int px = pos.x + dx;
            int py = pos.y + dy;
            if (px >= 0 && px < GRID_W && py >= 0 && py < GRID_H) {
                face.setPixel(px, py, ballColor);
            }
        }
    }

    cube.render();
}
