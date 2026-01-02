#include "cube.h"
#include "cube_geometry.h"

// Snake game across all cube faces

EffectSnake3D::EffectSnake3D() {
    reset();
}

void EffectSnake3D::reset() {
    snake.clear();
    snake.push_back({0, 4, 4});  // Start on front
    snake.push_back({0, 3, 4});
    snake.push_back({0, 2, 4});
    dirX = 1;
    dirY = 0;
    moveTimer = 0;
    spawnFood();
}

void EffectSnake3D::spawnFood() {
    // Random position on random face
    food.face = random(6);
    food.x = random(8);
    food.y = random(8);

    // Make sure food is not on snake
    for (const auto& seg : snake) {
        if (seg.face == food.face && seg.x == food.x && seg.y == food.y) {
            spawnFood();
            return;
        }
    }
}

void EffectSnake3D::render(Cube& cube, unsigned long deltaTime) {
    moveTimer += speed;

    if (moveTimer >= 1.0f) {
        moveTimer = 0;

        // Get new head position
        CubePos newHead = getNeighbor(snake[0].face, snake[0].x, snake[0].y, dirX, dirY);

        // Check self collision
        for (size_t i = 0; i < snake.size(); i++) {
            if (snake[i].face == newHead.face &&
                snake[i].x == newHead.x &&
                snake[i].y == newHead.y) {
                reset();
                return;
            }
        }

        // Add new head
        snake.insert(snake.begin(), newHead);

        // Check food
        if (newHead.face == food.face && newHead.x == food.x && newHead.y == food.y) {
            // Grow snake (don't remove tail)
            spawnFood();
            if (snake.size() > (size_t)maxLength) {
                snake.pop_back();
            }
        } else {
            // Remove tail
            snake.pop_back();
        }

        // Random turn (10% chance)
        if (random(100) < 10) {
            int temp = dirX;
            dirX = (random(2) == 0) ? dirY : -dirY;
            dirY = (dirX == 0) ? temp : -temp;
            if (dirX == 0 && dirY == 0) dirX = 1;
        }
    }

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Draw snake with gradient
    for (size_t i = 0; i < snake.size(); i++) {
        float brightness = 1.0f - (float)i / snake.size() * 0.7f;
        uint8_t g = 255 * brightness;
        uint32_t color = (g << 8);  // Green snake
        faces[snake[i].face]->setPixel(snake[i].x, snake[i].y, color);
    }

    // Draw food (blinking red)
    if ((millis() / 200) % 2 == 0) {
        faces[food.face]->setPixel(food.x, food.y, 0xFF0000);
    }

    cube.render();
}
