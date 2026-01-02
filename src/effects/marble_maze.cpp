#include "cube.h"
#include <cmath>

EffectMarbleMaze::EffectMarbleMaze() {
    currentFace = 0;
    marbleX = GRID_W / 2;
    marbleY = GRID_H / 2;
    velX = 0;
    velY = 0;
    tiltX = 0;
    tiltY = 0;
    tiltTime = 0;
    generateMaze();
}

void EffectMarbleMaze::generateMaze() {
    // Simple maze - borders and some obstacles
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            maze[x][y] = 0;  // 0 = empty, 1 = wall
        }
    }

    // Border walls
    for (int i = 0; i < 8; i++) {
        maze[0][i] = 1;
        maze[7][i] = 1;
        maze[i][0] = 1;
        maze[i][7] = 1;
    }

    // Some internal walls
    maze[2][2] = 1; maze[2][3] = 1;
    maze[5][4] = 1; maze[5][5] = 1;
    maze[3][5] = 1;
    maze[4][2] = 1;
}

void EffectMarbleMaze::updateMarble() {
    // Apply tilt as acceleration
    velX += tiltX * 0.02f;
    velY += tiltY * 0.02f;

    // Friction
    velX *= 0.95f;
    velY *= 0.95f;

    // Update position
    float newX = marbleX + velX;
    float newY = marbleY + velY;

    // Check collision with walls
    int checkX = (int)newX;
    int checkY = (int)newY;

    if (checkX >= 0 && checkX < 8 && checkY >= 0 && checkY < 8) {
        if (maze[checkX][(int)marbleY] == 1) {
            velX = -velX * 0.5f;
            newX = marbleX;
        }
        if (maze[(int)marbleX][checkY] == 1) {
            velY = -velY * 0.5f;
            newY = marbleY;
        }
    }

    // Clamp to bounds
    if (newX < 1) { newX = 1; velX = 0; }
    if (newX > 6) { newX = 6; velX = 0; }
    if (newY < 1) { newY = 1; velY = 0; }
    if (newY > 6) { newY = 6; velY = 0; }

    marbleX = newX;
    marbleY = newY;
}

void EffectMarbleMaze::render(Cube& cube, unsigned long deltaTime) {
    tiltTime += deltaTime;

    // Slowly change tilt direction
    tiltX = sin(tiltTime * tiltSpeed * 0.001f) * 0.5f;
    tiltY = cos(tiltTime * tiltSpeed * 0.0013f) * 0.5f;

    updateMarble();

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Draw maze on all faces
    for (int f = 0; f < 6; f++) {
        // Draw walls
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                if (maze[x][y] == 1) {
                    faces[f]->setPixel(x, y, 0x333333);
                }
            }
        }
    }

    // Draw marble on current face
    int mx = (int)marbleX;
    int my = (int)marbleY;
    if (mx >= 0 && mx < GRID_W && my >= 0 && my < GRID_H) {
        faces[currentFace]->setPixel(mx, my, 0xFF0000);

        // Glow on adjacent pixels
        if (mx > 0) faces[currentFace]->setPixel(mx - 1, my, 0x440000);
        if (mx < 7) faces[currentFace]->setPixel(mx + 1, my, 0x440000);
        if (my > 0) faces[currentFace]->setPixel(mx, my - 1, 0x440000);
        if (my < 7) faces[currentFace]->setPixel(mx, my + 1, 0x440000);
    }

    cube.render();
}
