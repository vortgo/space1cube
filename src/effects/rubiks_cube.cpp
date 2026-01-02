#include "cube.h"

EffectRubiksCube::EffectRubiksCube() {
    // Цвета граней собранного кубика
    solvedColors[0] = 0xFF0000;  // Front - красный
    solvedColors[1] = 0xFF8800;  // Back - оранжевый
    solvedColors[2] = 0x0000FF;  // Left - синий
    solvedColors[3] = 0x00FF00;  // Right - зелёный
    solvedColors[4] = 0xFFFFFF;  // Top - белый
    solvedColors[5] = 0xFFFF00;  // Bottom - жёлтый

    initSolved();
    state = SCRAMBLING;
    stateTimer = 0;
    moveTimer = 0;
    scrambleMoves = 0;
    totalScrambleMoves = 20;  // Количество ходов для перемешивания
    currentMoveIdx = 0;
    moveHistory.clear();
}

void EffectRubiksCube::initSolved() {
    // Заполняем каждую грань её цветом
    for (int face = 0; face < 6; face++) {
        for (int row = 0; row < CUBE_SIZE; row++) {
            for (int col = 0; col < CUBE_SIZE; col++) {
                faceColors[face][row][col] = solvedColors[face];
            }
        }
    }
}

void EffectRubiksCube::rotateFace(int face, bool clockwise) {
    // Поворот самой грани (3x3)
    uint32_t temp[CUBE_SIZE][CUBE_SIZE];

    // Копируем грань
    for (int r = 0; r < CUBE_SIZE; r++) {
        for (int c = 0; c < CUBE_SIZE; c++) {
            temp[r][c] = faceColors[face][r][c];
        }
    }

    // Поворачиваем
    for (int r = 0; r < CUBE_SIZE; r++) {
        for (int c = 0; c < CUBE_SIZE; c++) {
            if (clockwise) {
                faceColors[face][c][CUBE_SIZE - 1 - r] = temp[r][c];
            } else {
                faceColors[face][CUBE_SIZE - 1 - c][r] = temp[r][c];
            }
        }
    }

    // Поворот соседних рядов/столбцов
    uint32_t strip[CUBE_SIZE];

    if (face == 0) {  // Front
        if (clockwise) {
            for (int i = 0; i < CUBE_SIZE; i++) strip[i] = faceColors[4][CUBE_SIZE - 1][i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[4][CUBE_SIZE - 1][i] = faceColors[2][CUBE_SIZE - 1 - i][CUBE_SIZE - 1];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[2][i][CUBE_SIZE - 1] = faceColors[5][0][i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[5][0][i] = faceColors[3][CUBE_SIZE - 1 - i][0];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[3][i][0] = strip[i];
        } else {
            for (int i = 0; i < CUBE_SIZE; i++) strip[i] = faceColors[4][CUBE_SIZE - 1][i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[4][CUBE_SIZE - 1][i] = faceColors[3][i][0];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[3][i][0] = faceColors[5][0][CUBE_SIZE - 1 - i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[5][0][i] = faceColors[2][i][CUBE_SIZE - 1];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[2][CUBE_SIZE - 1 - i][CUBE_SIZE - 1] = strip[i];
        }
    } else if (face == 1) {  // Back
        if (clockwise) {
            for (int i = 0; i < CUBE_SIZE; i++) strip[i] = faceColors[4][0][i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[4][0][i] = faceColors[3][i][CUBE_SIZE - 1];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[3][i][CUBE_SIZE - 1] = faceColors[5][CUBE_SIZE - 1][CUBE_SIZE - 1 - i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[5][CUBE_SIZE - 1][i] = faceColors[2][i][0];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[2][CUBE_SIZE - 1 - i][0] = strip[i];
        } else {
            for (int i = 0; i < CUBE_SIZE; i++) strip[i] = faceColors[4][0][i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[4][0][i] = faceColors[2][CUBE_SIZE - 1 - i][0];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[2][i][0] = faceColors[5][CUBE_SIZE - 1][i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[5][CUBE_SIZE - 1][i] = faceColors[3][CUBE_SIZE - 1 - i][CUBE_SIZE - 1];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[3][i][CUBE_SIZE - 1] = strip[i];
        }
    } else if (face == 2) {  // Left
        if (clockwise) {
            for (int i = 0; i < CUBE_SIZE; i++) strip[i] = faceColors[4][i][0];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[4][i][0] = faceColors[1][CUBE_SIZE - 1 - i][CUBE_SIZE - 1];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[1][i][CUBE_SIZE - 1] = faceColors[5][CUBE_SIZE - 1 - i][0];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[5][i][0] = faceColors[0][i][0];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[0][i][0] = strip[i];
        } else {
            for (int i = 0; i < CUBE_SIZE; i++) strip[i] = faceColors[4][i][0];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[4][i][0] = faceColors[0][i][0];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[0][i][0] = faceColors[5][i][0];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[5][i][0] = faceColors[1][CUBE_SIZE - 1 - i][CUBE_SIZE - 1];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[1][CUBE_SIZE - 1 - i][CUBE_SIZE - 1] = strip[i];
        }
    } else if (face == 3) {  // Right
        if (clockwise) {
            for (int i = 0; i < CUBE_SIZE; i++) strip[i] = faceColors[4][i][CUBE_SIZE - 1];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[4][i][CUBE_SIZE - 1] = faceColors[0][i][CUBE_SIZE - 1];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[0][i][CUBE_SIZE - 1] = faceColors[5][i][CUBE_SIZE - 1];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[5][i][CUBE_SIZE - 1] = faceColors[1][CUBE_SIZE - 1 - i][0];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[1][CUBE_SIZE - 1 - i][0] = strip[i];
        } else {
            for (int i = 0; i < CUBE_SIZE; i++) strip[i] = faceColors[4][i][CUBE_SIZE - 1];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[4][i][CUBE_SIZE - 1] = faceColors[1][CUBE_SIZE - 1 - i][0];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[1][i][0] = faceColors[5][CUBE_SIZE - 1 - i][CUBE_SIZE - 1];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[5][i][CUBE_SIZE - 1] = faceColors[0][i][CUBE_SIZE - 1];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[0][i][CUBE_SIZE - 1] = strip[i];
        }
    } else if (face == 4) {  // Top
        if (clockwise) {
            for (int i = 0; i < CUBE_SIZE; i++) strip[i] = faceColors[0][0][i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[0][0][i] = faceColors[3][0][i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[3][0][i] = faceColors[1][0][i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[1][0][i] = faceColors[2][0][i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[2][0][i] = strip[i];
        } else {
            for (int i = 0; i < CUBE_SIZE; i++) strip[i] = faceColors[0][0][i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[0][0][i] = faceColors[2][0][i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[2][0][i] = faceColors[1][0][i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[1][0][i] = faceColors[3][0][i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[3][0][i] = strip[i];
        }
    } else if (face == 5) {  // Bottom
        if (clockwise) {
            for (int i = 0; i < CUBE_SIZE; i++) strip[i] = faceColors[0][CUBE_SIZE - 1][i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[0][CUBE_SIZE - 1][i] = faceColors[2][CUBE_SIZE - 1][i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[2][CUBE_SIZE - 1][i] = faceColors[1][CUBE_SIZE - 1][i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[1][CUBE_SIZE - 1][i] = faceColors[3][CUBE_SIZE - 1][i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[3][CUBE_SIZE - 1][i] = strip[i];
        } else {
            for (int i = 0; i < CUBE_SIZE; i++) strip[i] = faceColors[0][CUBE_SIZE - 1][i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[0][CUBE_SIZE - 1][i] = faceColors[3][CUBE_SIZE - 1][i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[3][CUBE_SIZE - 1][i] = faceColors[1][CUBE_SIZE - 1][i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[1][CUBE_SIZE - 1][i] = faceColors[2][CUBE_SIZE - 1][i];
            for (int i = 0; i < CUBE_SIZE; i++) faceColors[2][CUBE_SIZE - 1][i] = strip[i];
        }
    }
}

void EffectRubiksCube::addScrambleMove() {
    Move move;
    move.face = random(6);
    move.clockwise = random(2) == 0;

    // Выполняем ход
    rotateFace(move.face, move.clockwise);

    // Сохраняем для обратного воспроизведения
    moveHistory.push_back(move);
    scrambleMoves++;
}

void EffectRubiksCube::performMove() {
    if (currentMoveIdx >= 0 && currentMoveIdx < (int)moveHistory.size()) {
        Move& move = moveHistory[currentMoveIdx];
        // Выполняем обратный ход для сборки
        rotateFace(move.face, !move.clockwise);
        currentMoveIdx--;
    }
}

uint32_t EffectRubiksCube::getCellColor(int faceIdx, int row, int col) {
    return faceColors[faceIdx][row][col];
}

void EffectRubiksCube::drawFace(Matrix& matrix, int faceIdx) {
    // Рисуем 3x3 кубик на 8x8 матрице с рамкой
    int cellSize = 2;  // Каждая ячейка 2x2 пикселя
    int offsetX = 1;   // Отступ для центрирования
    int offsetY = 1;

    // Чёрный фон/рамка
    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            matrix.setPixel(x, y, 0x111111);
        }
    }

    // Рисуем ячейки
    for (int row = 0; row < CUBE_SIZE; row++) {
        for (int col = 0; col < CUBE_SIZE; col++) {
            uint32_t color = faceColors[faceIdx][row][col];

            int startX = offsetX + col * cellSize;
            int startY = offsetY + row * cellSize;

            for (int dx = 0; dx < cellSize; dx++) {
                for (int dy = 0; dy < cellSize; dy++) {
                    matrix.setPixel(startX + dx, startY + dy, color);
                }
            }
        }
    }
}

void EffectRubiksCube::render(Cube& cube, unsigned long deltaTime) {
    stateTimer += deltaTime;
    moveTimer += deltaTime;

    switch (state) {
        case SCRAMBLING:
            if (moveTimer >= 50.0f / scrambleSpeed) {  // Быстрое перемешивание
                moveTimer = 0;
                if (scrambleMoves < totalScrambleMoves) {
                    addScrambleMove();
                } else {
                    state = SOLVING;
                    currentMoveIdx = moveHistory.size() - 1;
                    moveTimer = 0;
                }
            }
            break;

        case SOLVING:
            if (moveTimer >= 200.0f / solveSpeed) {  // Медленная сборка
                moveTimer = 0;
                if (currentMoveIdx >= 0) {
                    performMove();
                } else {
                    state = PAUSED;
                    stateTimer = 0;
                }
            }
            break;

        case PAUSED:
            if (stateTimer >= pauseDuration) {
                state = SCRAMBLING;
                scrambleMoves = 0;
                moveHistory.clear();
                initSolved();  // Начинаем с собранного
                stateTimer = 0;
            }
            break;

        default:
            break;
    }

    // Отрисовка
    cube.clear();
    drawFace(cube.front, 0);
    drawFace(cube.back, 1);
    drawFace(cube.left, 2);
    drawFace(cube.right, 3);
    drawFace(cube.top, 4);
    drawFace(cube.bottom, 5);

    cube.render();
}
