#include "cube.h"
#include "cube_geometry.h"

// Conway's Game of Life on the cube surface with edge transitions

EffectGameOfLife3D::EffectGameOfLife3D() {
    randomize();
    stepTimer = 0;
    generation = 0;
    stagnant = 0;
}

void EffectGameOfLife3D::render(Cube& cube, unsigned long deltaTime) {
    stepTimer += deltaTime;

    if (stepTimer >= stepInterval) {
        stepTimer = 0;
        nextGeneration();
        generation++;

        // Check for stagnation and reset if needed
        if (stagnant > 20) {
            randomize();
            generation = 0;
            stagnant = 0;
        }
    }

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Render current state
    for (int f = 0; f < 6; f++) {
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                if (cells[f][x][y]) {
                    // Color based on face for visual interest
                    uint32_t faceColors[] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF, 0x00FFFF};
                    faces[f]->setPixel(x, y, faceColors[f]);
                }
            }
        }
    }

    cube.render();
}

void EffectGameOfLife3D::reset() {
    randomize();
    generation = 0;
    stagnant = 0;
    stepTimer = 0;
}

void EffectGameOfLife3D::randomize() {
    for (int f = 0; f < 6; f++) {
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                cells[f][x][y] = random(100) < (int)(density * 100);
            }
        }
    }
}

void EffectGameOfLife3D::nextGeneration() {
    bool newCells[6][8][8];
    int changes = 0;

    for (int f = 0; f < 6; f++) {
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                int neighbors = countNeighbors(f, x, y);
                bool alive = cells[f][x][y];

                if (alive) {
                    // Survive with 2-3 neighbors
                    newCells[f][x][y] = (neighbors == 2 || neighbors == 3);
                } else {
                    // Born with exactly 3 neighbors
                    newCells[f][x][y] = (neighbors == 3);
                }

                if (newCells[f][x][y] != cells[f][x][y]) {
                    changes++;
                }
            }
        }
    }

    // Copy new state
    for (int f = 0; f < 6; f++) {
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                cells[f][x][y] = newCells[f][x][y];
            }
        }
    }

    // Track stagnation
    if (changes < 3) {
        stagnant++;
    } else {
        stagnant = 0;
    }
}

int EffectGameOfLife3D::countNeighbors(int face, int x, int y) {
    int count = 0;

    // Check all 8 directions using cube geometry
    int dx[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int dy[] = {-1, -1, -1, 0, 0, 1, 1, 1};

    for (int i = 0; i < 8; i++) {
        CubePos neighbor = getNeighbor(face, x, y, dx[i], dy[i]);
        if (cells[neighbor.face][neighbor.x][neighbor.y]) {
            count++;
        }
    }

    return count;
}
