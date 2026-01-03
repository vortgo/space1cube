#include "cube.h"
#include "cube_geometry.h"

// Maze on cube surface with animated solver

EffectMaze3D::EffectMaze3D() {
    generateMaze();
    solverPos = {0, 0, 0};
    solving = false;
    solveTimer = 0;
    solveDelay = 100;
}

void EffectMaze3D::render(Cube& cube, unsigned long deltaTime) {
    solveTimer += deltaTime;

    if (solving && solveTimer >= solveDelay) {
        solveTimer = 0;
        solveStep();
    }

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Draw maze on all faces
    for (int f = 0; f < 6; f++) {
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                if (maze[f][x][y] == 1) {
                    // Wall
                    faces[f]->setPixel(x, y, wallColor);
                } else if (maze[f][x][y] == 2) {
                    // Visited path
                    faces[f]->setPixel(x, y, pathColor);
                }
                // 0 = empty (black)
            }
        }
    }

    // Draw solver position
    if (solving) {
        faces[solverPos.face]->setPixel(solverPos.x, solverPos.y, solverColor);
    }

    // Draw goal
    faces[goalPos.face]->setPixel(goalPos.x, goalPos.y, 0x00FF00);

    cube.render();
}

void EffectMaze3D::generateMaze() {
    // Initialize all as walls
    for (int f = 0; f < 6; f++) {
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                maze[f][x][y] = 1;
            }
        }
    }

    // Simple maze generation using recursive backtracking
    // Start from front face center
    carve(0, 1, 1);

    // Set start and goal
    solverPos = {0, 1, 1};
    goalPos = {1, 6, 6}; // Back face

    // Make sure goal is reachable
    maze[goalPos.face][goalPos.x][goalPos.y] = 0;

    // Clear path stack
    while (!pathStack.empty()) pathStack.pop();

    solving = true;
}

void EffectMaze3D::carve(int face, int x, int y) {
    maze[face][x][y] = 0;

    // Directions to try (randomized)
    int dirs[4][2] = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}};

    // Shuffle directions
    for (int i = 3; i > 0; i--) {
        int j = random(i + 1);
        int tx = dirs[i][0]; int ty = dirs[i][1];
        dirs[i][0] = dirs[j][0]; dirs[i][1] = dirs[j][1];
        dirs[j][0] = tx; dirs[j][1] = ty;
    }

    for (int i = 0; i < 4; i++) {
        int nx = x + dirs[i][0];
        int ny = y + dirs[i][1];

        // Check if within bounds (for simplicity, stay on same face)
        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            if (maze[face][nx][ny] == 1) {
                // Carve passage
                maze[face][x + dirs[i][0]/2][y + dirs[i][1]/2] = 0;
                carve(face, nx, ny);
            }
        }
    }
}

void EffectMaze3D::solveStep() {
    if (solverPos.face == goalPos.face &&
        solverPos.x == goalPos.x &&
        solverPos.y == goalPos.y) {
        // Reached goal - regenerate maze
        generateMaze();
        return;
    }

    // Mark current as visited
    maze[solverPos.face][solverPos.x][solverPos.y] = 2;

    // Try to find unvisited neighbor
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};

    bool moved = false;
    for (int i = 0; i < 4; i++) {
        CubePos next = getNeighbor(solverPos.face, solverPos.x, solverPos.y, dx[i], dy[i]);

        if (maze[next.face][next.x][next.y] == 0) {
            // Can move here
            pathStack.push(solverPos);
            solverPos = next;
            moved = true;
            break;
        }
    }

    if (!moved) {
        // Backtrack
        if (!pathStack.empty()) {
            solverPos = pathStack.top();
            pathStack.pop();
        } else {
            // Stuck - regenerate
            generateMaze();
        }
    }
}

void EffectMaze3D::reset() {
    generateMaze();
}
