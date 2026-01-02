#include "cube.h"
#include <cmath>

EffectGravity3D::EffectGravity3D() {
    gravityFace = 5;  // Bottom
    rotateTimer = 0;

    uint32_t colors[] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF, 0x00FFFF};

    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].face = random(6);
        particles[i].x = random(GRID_W);
        particles[i].y = random(GRID_H);
        particles[i].vx = 0;
        particles[i].vy = 0;
        particles[i].color = colors[i % 6];
    }
}

void EffectGravity3D::applyGravity() {
    for (int i = 0; i < particleCount && i < MAX_PARTICLES; i++) {
        // Apply gravity toward the gravity face
        if (particles[i].face == gravityFace) {
            // On gravity face - slide toward center or stay
            particles[i].vx *= 0.9f;
            particles[i].vy *= 0.9f;
        } else if (particles[i].face == (gravityFace == 5 ? 4 : 5)) {
            // Opposite face - fall "through" to adjacent
            particles[i].vy += gravity;
        } else {
            // Side face - fall down
            particles[i].vy += gravity;
        }

        particles[i].x += particles[i].vx;
        particles[i].y += particles[i].vy;

        // Boundary handling with face transitions
        if (particles[i].y >= GRID_H) {
            if (particles[i].face != gravityFace) {
                // Transition to gravity face
                particles[i].face = gravityFace;
                particles[i].y = 0;
                particles[i].vy *= 0.5f;
            } else {
                particles[i].y = GRID_H - 1;
                particles[i].vy = -particles[i].vy * 0.3f;
            }
        }
        if (particles[i].y < 0) {
            particles[i].y = 0;
            particles[i].vy = -particles[i].vy * 0.3f;
        }
        if (particles[i].x < 0) {
            particles[i].x = 0;
            particles[i].vx = -particles[i].vx * 0.5f;
        }
        if (particles[i].x >= GRID_W) {
            particles[i].x = GRID_W - 1;
            particles[i].vx = -particles[i].vx * 0.5f;
        }
    }
}

void EffectGravity3D::render(Cube& cube, unsigned long deltaTime) {
    rotateTimer += deltaTime;

    // Change gravity direction periodically
    if (rotateTimer > 5000) {
        rotateTimer = 0;
        gravityFace = random(6);

        // Give particles some velocity when gravity changes
        for (int i = 0; i < particleCount && i < MAX_PARTICLES; i++) {
            particles[i].vx = (random(100) - 50) / 100.0f * 0.5f;
            particles[i].vy = (random(100) - 50) / 100.0f * 0.5f;
        }
    }

    applyGravity();

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    for (int i = 0; i < particleCount && i < MAX_PARTICLES; i++) {
        int f = particles[i].face;
        int x = (int)particles[i].x;
        int y = (int)particles[i].y;

        if (f >= 0 && f < 6 && x >= 0 && x < GRID_W && y >= 0 && y < GRID_H) {
            faces[f]->setPixel(x, y, particles[i].color);
        }
    }

    cube.render();
}
