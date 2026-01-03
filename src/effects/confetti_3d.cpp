#include "cube.h"
#include "cube_geometry.h"

// Confetti particles falling down across all faces with proper 3D transitions

EffectConfetti3D::EffectConfetti3D() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].active = false;
    }
    spawnTimer = 0;
}

void EffectConfetti3D::render(Cube& cube, unsigned long deltaTime) {
    float dt = deltaTime / 1000.0f;
    spawnTimer += dt;

    // Spawn new particles
    while (spawnTimer >= 1.0f / spawnRate && countActive() < MAX_PARTICLES) {
        spawnTimer -= 1.0f / spawnRate;
        spawnParticle();
    }

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Update and draw particles
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particles[i].active) continue;

        Particle& p = particles[i];

        // Update position
        p.moveAccum += p.speed * dt;
        while (p.moveAccum >= 1.0f) {
            p.moveAccum -= 1.0f;

            // Move down (dy = +1 in screen coordinates)
            CubePos oldPos = p.pos;
            moveOnCube(p.pos, 0, 1);

            // Check if we've wrapped to top (falling off bottom)
            if (p.pos.face == 4 && oldPos.face != 4) {
                // Fell off bottom to top - deactivate
                p.active = false;
                continue;
            }

            p.life -= 0.05f;
            if (p.life <= 0) {
                p.active = false;
                continue;
            }
        }

        // Draw particle with fading
        if (p.active) {
            uint32_t color = dimColor(p.color, p.life);
            faces[p.pos.face]->setPixel(p.pos.x, p.pos.y, color);

            // Draw small trail
            if (p.pos.y > 0) {
                CubePos trailPos = getNeighbor(p.pos.face, p.pos.x, p.pos.y, 0, -1);
                faces[trailPos.face]->setPixel(trailPos.x, trailPos.y, dimColor(p.color, p.life * 0.3f));
            }
        }
    }

    cube.render();
}

void EffectConfetti3D::spawnParticle() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particles[i].active) {
            Particle& p = particles[i];
            p.active = true;

            // Spawn on top face or top row of side faces
            if (random(100) < 40) {
                // Spawn on top face
                p.pos.face = 4;
                p.pos.x = random(8);
                p.pos.y = random(4); // Upper half of top
            } else {
                // Spawn on top row of side faces
                p.pos.face = random(4); // front, back, left, right
                p.pos.x = random(8);
                p.pos.y = 0;
            }

            p.speed = 3.0f + random(100) / 50.0f; // 3-5 pixels per second
            p.moveAccum = 0;
            p.life = 1.0f;
            p.color = getRandomColor();
            return;
        }
    }
}

int EffectConfetti3D::countActive() {
    int count = 0;
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].active) count++;
    }
    return count;
}

uint32_t EffectConfetti3D::getRandomColor() {
    uint32_t colors[] = {
        0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00,
        0xFF00FF, 0x00FFFF, 0xFFA500, 0xFF1493,
        0x7FFF00, 0x9400D3
    };
    return colors[random(10)];
}

uint32_t EffectConfetti3D::dimColor(uint32_t color, float factor) {
    uint8_t r = ((color >> 16) & 0xFF) * factor;
    uint8_t g = ((color >> 8) & 0xFF) * factor;
    uint8_t b = (color & 0xFF) * factor;
    return (r << 16) | (g << 8) | b;
}
