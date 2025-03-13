#include "cube.h"
#include <cmath>
#include <vector>
#include <cstdlib>

void EffectPacman::drawPacman(Cube& cube) {
    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    for (auto& face : faces) {
        face.get().clear();
    }

    uint8_t pacmanOpen[8][8] = {
        {0,1,1,1,1,1,0,0},
        {1,1,1,1,1,1,1,0},
        {1,1,0,1,1,0,1,0},
        {1,1,1,1,0,0,0,0},
        {1,1,1,1,0,0,0,0},
        {1,1,1,1,1,0,1,0},
        {1,1,1,1,1,1,1,0},
        {0,1,1,1,1,1,0,0}
    };

    uint8_t pacmanClosed[8][8] = {
        {0,1,1,1,1,1,0,0},
        {1,1,1,1,1,1,1,0},
        {1,1,0,1,1,1,1,0},
        {1,1,1,1,1,1,1,0},
        {1,1,1,1,1,1,1,0},
        {1,1,1,1,1,1,1,0},
        {1,1,1,1,1,1,1,0},
        {0,1,1,1,1,1,0,0}
    };

    uint8_t (*pacmanShape)[8] = mouthOpen ? pacmanOpen : pacmanClosed;

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 7; x++) {
            if (pacmanShape[y][x]) {
                for (auto& face : faces) {
                    face.get().setPixel(x, y, yellow);
                }
            }
        }
    }

    // Глаз Pacman
    for (auto& face : faces) {
        face.get().setPixel(2, 1, black);
    }

    for (const auto& pellet : pellets) {
        for (auto& face : faces) {
            face.get().setPixel(pellet.x, pellet.y, pellet.color);
        }
    }
}

void EffectPacman::spawnPellet() {
    Pellet p;
    p.x = 7;
    p.y = 3 + (rand() % 2);
    p.color = (rand() % 256 << 16) | (rand() % 256 << 8) | (rand() % 256);
    pellets.push_back(p);
}

void EffectPacman::render(Cube& cube, unsigned long deltaTime) {
    mouthTimer += deltaTime / 1000.0f;
    timeSinceLastPellet += deltaTime / 1000.0f;

    if (mouthTimer > 0.6f) { // Более плавная анимация рта
        mouthOpen = !mouthOpen;
        mouthTimer = 0.0f;
    }

    if (timeSinceLastPellet > pelletSpawnRate) {
        spawnPellet();
        timeSinceLastPellet = 0.0f;
    }

    for (auto it = pellets.begin(); it != pellets.end();) {
        it->x -= 1;

        if (it->x == 5 && (it->y == 3 || it->y == 4) && mouthOpen) {
            it = pellets.erase(it); // Pacman съедает еду, когда рот открыт
        } else {
            ++it;
        }
    }

    drawPacman(cube);
    cube.render();
}

