#include "cube.h"
#include <cmath>

EffectExplosion3D::EffectExplosion3D() {
    timeSinceExplosion = cooldown;
    exploding = false;
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].active = false;
    }
}

void EffectExplosion3D::triggerExplosion() {
    exploding = true;
    timeSinceExplosion = 0;

    // Create particles from center
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].face = 0;
        particles[i].x = GRID_W / 2;
        particles[i].y = GRID_H / 2;

        // Random velocity in all directions
        float angle = random(360) * M_PI / 180.0f;
        float speed = 0.5f + (random(100) / 100.0f) * 1.5f;
        particles[i].vx = cos(angle) * speed;
        particles[i].vy = sin(angle) * speed;
        particles[i].vface = (random(100) - 50) / 100.0f;

        particles[i].life = 1.0f;
        particles[i].active = true;

        // Random color
        uint32_t colors[] = {0xFF0000, 0xFF5500, 0xFFAA00, 0xFFFF00, 0xFFFFFF};
        particles[i].color = colors[random(5)];
    }
}

void EffectExplosion3D::render(Cube& cube, unsigned long deltaTime) {
    timeSinceExplosion += deltaTime;

    if (timeSinceExplosion >= cooldown && !exploding) {
        triggerExplosion();
    }

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    bool anyActive = false;

    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particles[i].active) continue;
        anyActive = true;

        // Update position
        particles[i].x += particles[i].vx * explosionSpeed * deltaTime;
        particles[i].y += particles[i].vy * explosionSpeed * deltaTime;

        // Apply gravity
        particles[i].vy += 0.001f * deltaTime;

        // Fade
        particles[i].life -= 0.0005f * deltaTime;
        if (particles[i].life <= 0) {
            particles[i].active = false;
            continue;
        }

        // Handle face transitions
        int f = (int)particles[i].face;
        if (f < 0) f = 0;
        if (f > 5) f = 5;

        if (particles[i].x < 0) {
            particles[i].x = 0;
            particles[i].vx = -particles[i].vx * 0.5f;
            f = (f + 1) % 4; // Move to adjacent face
        }
        if (particles[i].x >= GRID_W) {
            particles[i].x = GRID_W - 1;
            particles[i].vx = -particles[i].vx * 0.5f;
            f = (f + 3) % 4;
        }
        if (particles[i].y < 0) {
            particles[i].y = 0;
            particles[i].vy = -particles[i].vy * 0.5f;
            f = 4;
        }
        if (particles[i].y >= GRID_H) {
            particles[i].y = GRID_H - 1;
            particles[i].vy = -particles[i].vy * 0.5f;
            f = 5;
        }
        particles[i].face = f;

        // Draw
        int px = (int)particles[i].x;
        int py = (int)particles[i].y;
        if (px >= 0 && px < GRID_W && py >= 0 && py < GRID_H) {
            float brightness = particles[i].life;
            uint32_t c = particles[i].color;
            uint8_t r = (uint8_t)(((c >> 16) & 0xFF) * brightness);
            uint8_t g = (uint8_t)(((c >> 8) & 0xFF) * brightness);
            uint8_t b = (uint8_t)((c & 0xFF) * brightness);
            faces[f]->setPixel(px, py, (r << 16) | (g << 8) | b);
        }
    }

    if (!anyActive) {
        exploding = false;
    }

    cube.render();
}
