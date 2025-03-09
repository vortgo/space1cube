#include "cube.h"


EffectSnake::EffectSnake() {
    resetGame();
}

void EffectSnake::resetGame() {
    snake.clear();
    int startX = rand() % 6 + 1;
    int startY = rand() % 6 + 1;
    uint32_t startColor = (rand() % 256 << 16) | (rand() % 256 << 8) | (rand() % 256);
    for (int i = 0; i < 4; i++) {
        snake.push_back({startX - i, startY, startColor});
    }
    directionX = 1;
    directionY = 0;
    moveInterval = 200; // 500 мс на шаг
    lastUpdateTime = 0;
    generateFood();
}

void EffectSnake::generateFood() {
    do {
        foodX = rand() % 8;
        foodY = rand() % 8;
    } while (checkCollision(foodX, foodY));
    foodColor = (rand() % 256 << 16) | (rand() % 256 << 8) | (rand() % 256);
}

bool EffectSnake::checkCollision(int x, int y) {
    for (const auto& segment : snake) {
        if (segment.x == x && segment.y == y) {
            return true;
        }
    }
    return false;
}

void EffectSnake::updateDirection() {
    int headX = snake.front().x;
    int headY = snake.front().y;
    int prevDirX = directionX;
    int prevDirY = directionY;

    // Возможные направления движения
    struct Direction {
        int dx, dy;
        bool collision;
    };

    std::vector<Direction> directions = {
        {1, 0, checkCollision(headX + 1, headY)},
        {-1, 0, checkCollision(headX - 1, headY)},
        {0, 1, checkCollision(headX, headY + 1)},
        {0, -1, checkCollision(headX, headY - 1)}
    };

    // Исключаем поворот назад
    for (auto& dir : directions) {
        if (dir.dx == -prevDirX && dir.dy == -prevDirY) {
            dir.collision = true;
        }
    }

    // Приоритет: 1) Безопасное движение к еде 2) Любое безопасное движение 3) Куда угодно (врезаться в себя)
    Direction* bestChoice = nullptr;
    for (auto& dir : directions) {
        if (!dir.collision) {
            if ((dir.dx == 1 && foodX > headX) || (dir.dx == -1 && foodX < headX) ||
                (dir.dy == 1 && foodY > headY) || (dir.dy == -1 && foodY < headY)) {
                bestChoice = &dir;
                break;
            }
        }
    }

    if (!bestChoice) {
        for (auto& dir : directions) {
            if (!dir.collision) {
                bestChoice = &dir;
                break;
            }
        }
    }

    if (!bestChoice) {
        bestChoice = &directions[0]; // В крайнем случае врезаемся
    }

    directionX = bestChoice->dx;
    directionY = bestChoice->dy;
}

void EffectSnake::move() {
    updateDirection();
    int newX = snake.front().x + directionX;
    int newY = snake.front().y + directionY;

    // Проверяем столкновение с краями
    if (newX < 0 || newX >= 8 || newY < 0 || newY >= 8 || checkCollision(newX, newY)) {
        resetGame();
        return;
    }

    // Проверяем, съела ли змея еду
    if (newX == foodX && newY == foodY) {
        uint32_t newColor = foodColor;
        for (auto &segment : snake) {
            segment.color = newColor;
        }
        snake.insert(snake.begin(), {newX, newY, newColor});
        generateFood();
    } else {
        snake.insert(snake.begin(), {newX, newY, snake.front().color});
        snake.pop_back();
    }
}

void EffectSnake::render(Cube& cube, unsigned long deltaTime) {
    lastUpdateTime += deltaTime;
    if (lastUpdateTime >= moveInterval) {
        move();
        lastUpdateTime = 0;
    }

    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    for (auto &face : faces) {
        face.get().clear();
        for (const auto& segment : snake) {
            face.get().setPixel(segment.x, segment.y, segment.color);
        }
        face.get().setPixel(foodX, foodY, foodColor);
    }
    cube.render();
}
