#include "cube.h"

EffectSnake3D::EffectSnake3D() {
    snake.clear();
    Segment head = {0, GRID_W / 2, GRID_H / 2};
    snake.push_back(head);
    dirX = 1;
    dirY = 0;
    moveTimer = 0;
    spawnFood();
}

void EffectSnake3D::spawnFood() {
    food.face = random(6);
    food.x = random(GRID_W);
    food.y = random(GRID_H);
}

void EffectSnake3D::transitionFace(Segment& seg) {
    int newFace = seg.face;
    int newX = seg.x;
    int newY = seg.y;

    if (seg.face == 0) {
        if (seg.x < 0) { newFace = 2; newX = GRID_W - 1; }
        else if (seg.x >= GRID_W) { newFace = 3; newX = 0; }
        else if (seg.y < 0) { newFace = 4; newY = GRID_H - 1; }
        else if (seg.y >= GRID_H) { newFace = 5; newY = 0; }
    } else if (seg.face == 1) {
        if (seg.x < 0) { newFace = 3; newX = GRID_W - 1; }
        else if (seg.x >= GRID_W) { newFace = 2; newX = 0; }
        else if (seg.y < 0) { newFace = 4; newY = 0; newX = GRID_W - 1 - seg.x; }
        else if (seg.y >= GRID_H) { newFace = 5; newY = GRID_H - 1; newX = GRID_W - 1 - seg.x; }
    } else if (seg.face == 2) {
        if (seg.x < 0) { newFace = 1; newX = GRID_W - 1; }
        else if (seg.x >= GRID_W) { newFace = 0; newX = 0; }
        else if (seg.y < 0) { newFace = 4; newY = seg.x; newX = 0; }
        else if (seg.y >= GRID_H) { newFace = 5; newY = GRID_H - 1 - seg.x; newX = 0; }
    } else if (seg.face == 3) {
        if (seg.x < 0) { newFace = 0; newX = GRID_W - 1; }
        else if (seg.x >= GRID_W) { newFace = 1; newX = 0; }
        else if (seg.y < 0) { newFace = 4; newY = GRID_W - 1 - seg.x; newX = GRID_W - 1; }
        else if (seg.y >= GRID_H) { newFace = 5; newY = seg.x; newX = GRID_W - 1; }
    } else if (seg.face == 4) {
        if (seg.x < 0) { newFace = 2; newX = seg.y; newY = 0; }
        else if (seg.x >= GRID_W) { newFace = 3; newX = GRID_W - 1 - seg.y; newY = 0; }
        else if (seg.y < 0) { newFace = 1; newY = 0; newX = GRID_W - 1 - seg.x; }
        else if (seg.y >= GRID_H) { newFace = 0; newY = 0; }
    } else if (seg.face == 5) {
        if (seg.x < 0) { newFace = 2; newX = GRID_W - 1 - seg.y; newY = GRID_H - 1; }
        else if (seg.x >= GRID_W) { newFace = 3; newX = seg.y; newY = GRID_H - 1; }
        else if (seg.y < 0) { newFace = 0; newY = GRID_H - 1; }
        else if (seg.y >= GRID_H) { newFace = 1; newY = GRID_H - 1; newX = GRID_W - 1 - seg.x; }
    }

    seg.face = newFace;
    seg.x = newX;
    seg.y = newY;
}

void EffectSnake3D::moveSnake() {
    // Move head
    Segment newHead = snake[0];
    newHead.x += dirX;
    newHead.y += dirY;

    if (newHead.x < 0 || newHead.x >= GRID_W || newHead.y < 0 || newHead.y >= GRID_H) {
        transitionFace(newHead);
    }

    snake.insert(snake.begin(), newHead);

    // Check food
    if (newHead.face == food.face && newHead.x == food.x && newHead.y == food.y) {
        spawnFood();
        if ((int)snake.size() > maxLength) {
            snake.pop_back();
        }
    } else {
        snake.pop_back();
    }

    // Random turn
    if (random(100) < 15) {
        int temp = dirX;
        dirX = (random(2) == 0) ? -dirY : dirY;
        dirY = (dirX == -dirY) ? temp : -temp;
    }
}

void EffectSnake3D::render(Cube& cube, unsigned long deltaTime) {
    moveTimer += speed;
    if (moveTimer >= 1.0f) {
        moveTimer = 0;
        moveSnake();
    }

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Draw snake
    for (size_t i = 0; i < snake.size(); i++) {
        Segment& seg = snake[i];
        float brightness = 1.0f - (float)i / snake.size() * 0.7f;
        uint8_t g = (uint8_t)(255 * brightness);
        faces[seg.face]->setPixel(seg.x, seg.y, (g << 8));
    }

    // Draw food
    faces[food.face]->setPixel(food.x, food.y, 0xFF0000);

    cube.render();
}
