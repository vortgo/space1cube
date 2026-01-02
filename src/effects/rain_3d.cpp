#include "cube.h"
#include "cube_geometry.h"

// Rain drops falling from top, down sides, to bottom

EffectRain3D::EffectRain3D() {
    for (int i = 0; i < MAX_DROPS; i++) {
        drops[i].active = false;
    }
    spawnTimer = 0;
}

void EffectRain3D::spawnDrop() {
    for (int i = 0; i < MAX_DROPS; i++) {
        if (!drops[i].active) {
            drops[i].active = true;
            drops[i].pos.face = 4;  // Start on top
            drops[i].pos.x = random(8);
            drops[i].pos.y = random(8);
            drops[i].speed = 0.1f + random(100) / 500.0f;
            drops[i].trail = 2 + random(4);
            drops[i].color = 0x0066FF + random(0x99);  // Blue shades

            // Random initial direction (will flow "down" based on face)
            drops[i].dirX = 0;
            drops[i].dirY = -1;
            return;
        }
    }
}

void EffectRain3D::updateDropDirection(Drop& drop) {
    // Gravity direction depends on current face
    switch (drop.pos.face) {
        case 4: // TOP - fall to any side
            drop.dirX = 0;
            drop.dirY = -1;  // Towards front edge
            break;
        case 5: // BOTTOM - collect in center
            drop.dirX = (drop.pos.x < 4) ? 1 : -1;
            drop.dirY = (drop.pos.y < 4) ? 1 : -1;
            break;
        case 0: // FRONT - fall down
        case 1: // BACK
        case 2: // LEFT
        case 3: // RIGHT
            drop.dirX = 0;
            drop.dirY = -1;
            break;
    }
}

void EffectRain3D::render(Cube& cube, unsigned long deltaTime) {
    // Spawn new drops
    spawnTimer += intensity;
    while (spawnTimer >= 1.0f) {
        spawnTimer -= 1.0f;
        spawnDrop();
    }

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Update and draw drops
    for (int i = 0; i < MAX_DROPS; i++) {
        if (!drops[i].active) continue;

        Drop& drop = drops[i];
        drop.moveAccum += drop.speed;

        if (drop.moveAccum >= 1.0f) {
            drop.moveAccum -= 1.0f;

            // Save trail position
            for (int t = drop.trail - 1; t > 0; t--) {
                drop.trailPos[t] = drop.trailPos[t-1];
            }
            drop.trailPos[0] = drop.pos;

            // Update direction based on current face
            updateDropDirection(drop);

            // Move
            CubePos newPos = getNeighbor(drop.pos.face, drop.pos.x, drop.pos.y, drop.dirX, drop.dirY);

            // Check if reached bottom and stayed there
            if (drop.pos.face == 5 && newPos.face == 5 &&
                abs(drop.pos.x - 4) <= 1 && abs(drop.pos.y - 4) <= 1) {
                drop.active = false;
                continue;
            }

            drop.pos = newPos;
        }

        // Draw drop with trail
        faces[drop.pos.face]->setPixel(drop.pos.x, drop.pos.y, drop.color);
        for (int t = 0; t < drop.trail; t++) {
            if (drop.trailPos[t].face >= 0) {
                float brightness = 1.0f - (float)(t + 1) / (drop.trail + 1);
                uint8_t b = ((drop.color & 0xFF) * brightness);
                uint8_t g = (((drop.color >> 8) & 0xFF) * brightness);
                uint8_t r = (((drop.color >> 16) & 0xFF) * brightness);
                faces[drop.trailPos[t].face]->setPixel(
                    drop.trailPos[t].x, drop.trailPos[t].y,
                    (r << 16) | (g << 8) | b
                );
            }
        }
    }

    cube.render();
}
