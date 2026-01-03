#include "cube.h"
#include <cstdlib>

EffectTetrisFall::EffectTetrisFall() {
    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            grid[x][y] = 0;
        }
    }
    currentPiece.active = false;
    fallTimer = 0;
    clearTimer = 0;
    lineToClear = -1;
}

void EffectTetrisFall::render(Cube& cube, unsigned long deltaTime) {
    fallTimer += deltaTime;

    // Очистка линии
    if (lineToClear >= 0) {
        clearTimer += deltaTime;
        if (clearTimer >= clearDelay) {
            // Сдвигаем всё вниз
            for (int y = lineToClear; y > 0; y--) {
                for (int x = 0; x < GRID_W; x++) {
                    grid[x][y] = grid[x][y - 1];
                }
            }
            for (int x = 0; x < GRID_W; x++) {
                grid[x][0] = 0;
            }
            lineToClear = -1;
            clearTimer = 0;
            checkLines();
        }
    }

    // Спавн новой фигуры
    if (!currentPiece.active && lineToClear < 0) {
        spawnPiece();
    }

    // Падение
    if (currentPiece.active && fallTimer >= 100.0f / fallSpeed) {
        fallTimer = 0;

        if (!checkCollision(currentPiece.x, currentPiece.y + 1)) {
            currentPiece.y++;
        } else {
            lockPiece();
            checkLines();
        }
    }

    // Рендер
    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    for (auto& faceRef : faces) {
        Matrix& face = faceRef.get();

        // Рисуем сетку
        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                // Мигание линии при очистке
                if (lineToClear == y) {
                    int blink = ((int)(clearTimer / 100)) % 2;
                    face.setPixel(x, y, blink ? 0xFFFFFF : grid[x][y]);
                } else {
                    face.setPixel(x, y, grid[x][y]);
                }
            }
        }

        // Рисуем текущую фигуру
        if (currentPiece.active) {
            // Простые фигуры 1-2 пикселя
            int px = currentPiece.x;
            int py = currentPiece.y;
            if (px >= 0 && px < GRID_W && py >= 0 && py < GRID_H) {
                face.setPixel(px, py, currentPiece.color);
            }
            // Второй пиксель в зависимости от типа
            int px2 = px, py2 = py;
            switch (currentPiece.type) {
                case 0: px2 = px + 1; break; // Горизонталь
                case 1: py2 = py + 1; break; // Вертикаль
                case 2: px2 = px + 1; py2 = py + 1; break; // Диагональ
            }
            if (px2 >= 0 && px2 < GRID_W && py2 >= 0 && py2 < GRID_H) {
                face.setPixel(px2, py2, currentPiece.color);
            }
        }
    }

    cube.render();
}

void EffectTetrisFall::spawnPiece() {
    currentPiece.x = random(GRID_W - 1);
    currentPiece.y = 0;
    currentPiece.type = random(3);
    currentPiece.color = getRandomColor();
    currentPiece.active = true;

    // Проверяем game over
    if (checkCollision(currentPiece.x, currentPiece.y)) {
        // Очищаем поле
        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                grid[x][y] = 0;
            }
        }
    }
}

void EffectTetrisFall::lockPiece() {
    int px = currentPiece.x;
    int py = currentPiece.y;
    if (px >= 0 && px < GRID_W && py >= 0 && py < GRID_H) {
        grid[px][py] = currentPiece.color;
    }
    int px2 = px, py2 = py;
    switch (currentPiece.type) {
        case 0: px2 = px + 1; break;
        case 1: py2 = py + 1; break;
        case 2: px2 = px + 1; py2 = py + 1; break;
    }
    if (px2 >= 0 && px2 < GRID_W && py2 >= 0 && py2 < GRID_H) {
        grid[px2][py2] = currentPiece.color;
    }
    currentPiece.active = false;
}

bool EffectTetrisFall::checkCollision(int nx, int ny) {
    if (ny >= GRID_H) return true;
    if (nx < 0 || nx >= GRID_W) return true;
    if (ny >= 0 && grid[nx][ny] != 0) return true;

    int px2 = nx, py2 = ny;
    switch (currentPiece.type) {
        case 0: px2 = nx + 1; break;
        case 1: py2 = ny + 1; break;
        case 2: px2 = nx + 1; py2 = ny + 1; break;
    }
    if (py2 >= GRID_H) return true;
    if (px2 < 0 || px2 >= GRID_W) return true;
    if (py2 >= 0 && grid[px2][py2] != 0) return true;

    return false;
}

void EffectTetrisFall::checkLines() {
    for (int y = GRID_H - 1; y >= 0; y--) {
        bool full = true;
        for (int x = 0; x < GRID_W; x++) {
            if (grid[x][y] == 0) {
                full = false;
                break;
            }
        }
        if (full) {
            lineToClear = y;
            clearTimer = 0;
            return;
        }
    }
}

uint32_t EffectTetrisFall::getRandomColor() {
    uint32_t colors[] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF, 0x00FFFF, 0xFFA500};
    return colors[random(7)];
}

void EffectTetrisFall::moveLeft() {
    if (currentPiece.active && !checkCollision(currentPiece.x - 1, currentPiece.y)) {
        currentPiece.x--;
    }
}

void EffectTetrisFall::moveRight() {
    if (currentPiece.active && !checkCollision(currentPiece.x + 1, currentPiece.y)) {
        currentPiece.x++;
    }
}

void EffectTetrisFall::drop() {
    if (currentPiece.active) {
        while (!checkCollision(currentPiece.x, currentPiece.y + 1)) {
            currentPiece.y++;
        }
        lockPiece();
        checkLines();
    }
}

void EffectTetrisFall::rotate() {
    if (currentPiece.active) {
        int oldType = currentPiece.type;
        currentPiece.type = (currentPiece.type + 1) % 3;
        if (checkCollision(currentPiece.x, currentPiece.y)) {
            currentPiece.type = oldType;
        }
    }
}
