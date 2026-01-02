#include "cube.h"

EffectRain3D::EffectRain3D() {
    for (int i = 0; i < MAX_DROPS; i++) {
        drops[i].active = false;
    }
    // Initialize some drops
    for (int i = 0; i < dropCount && i < MAX_DROPS; i++) {
        spawnDrop(i);
    }
}

void EffectRain3D::spawnDrop(int idx) {
    // Rain starts from top (face 4) and falls to sides, then to bottom
    drops[idx].face = 4;  // Start on top
    drops[idx].x = random(GRID_W);
    drops[idx].y = random(GRID_H);
    drops[idx].speed = 0.5f + (random(100) / 100.0f) * 0.5f;
    drops[idx].active = true;
}

void EffectRain3D::updateDrop(RainDrop& drop) {
    // On top face, move toward edge
    if (drop.face == 4) {
        // Move toward nearest edge
        float cx = GRID_W / 2.0f;
        float cy = GRID_H / 2.0f;
        float dx = drop.x - cx;
        float dy = drop.y - cy;

        if (abs(dx) > abs(dy)) {
            drop.x += (dx > 0 ? drop.speed : -drop.speed);
        } else {
            drop.y += (dy > 0 ? drop.speed : -drop.speed);
        }

        // Transition to side face
        if (drop.x < 0) { drop.face = 2; drop.x = GRID_W - 1; drop.y = 0; }
        else if (drop.x >= GRID_W) { drop.face = 3; drop.x = 0; drop.y = 0; }
        else if (drop.y < 0) { drop.face = 1; drop.x = GRID_W - 1 - (int)drop.x; drop.y = 0; }
        else if (drop.y >= GRID_H) { drop.face = 0; drop.y = 0; }
    }
    // On side faces, fall down
    else if (drop.face >= 0 && drop.face <= 3) {
        drop.y += drop.speed;

        // Transition to bottom face
        if (drop.y >= GRID_H) {
            drop.face = 5;  // Bottom
            drop.y = (drop.face == 0 || drop.face == 1) ? 0 : GRID_H / 2;
            drop.x = random(GRID_W);
        }
    }
    // On bottom face, spread and disappear
    else if (drop.face == 5) {
        drop.speed *= 0.95f;
        if (drop.speed < 0.1f) {
            drop.active = false;
        }
    }
}

void EffectRain3D::render(Cube& cube, unsigned long deltaTime) {
    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    int activeCount = 0;

    for (int i = 0; i < MAX_DROPS; i++) {
        if (!drops[i].active) {
            if (activeCount < dropCount && random(100) < 5) {
                spawnDrop(i);
            }
            continue;
        }

        activeCount++;
        updateDrop(drops[i]);

        if (!drops[i].active) continue;

        // Draw drop with trail
        int x = (int)drops[i].x;
        int y = (int)drops[i].y;
        int f = drops[i].face;

        if (f >= 0 && f < 6 && x >= 0 && x < GRID_W && y >= 0 && y < GRID_H) {
            // Drop head
            faces[f]->setPixel(x, y, 0x4444FF);

            // Trail
            for (int t = 1; t < 3; t++) {
                int ty = y - t;
                if (ty >= 0) {
                    uint8_t brightness = 255 - t * 80;
                    faces[f]->setPixel(x, ty, (brightness / 4) << 16 | (brightness / 4) << 8 | brightness);
                }
            }
        }
    }

    cube.render();
}
