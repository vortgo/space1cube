#include "cube.h"

void EffectGravityParticles::pickRandomBaseColor() {
    baseR = rand() % 256;
    baseG = rand() % 256;
    baseB = rand() % 256;
}

void EffectGravityParticles::initParticles() {
    particles.clear();
    pickRandomBaseColor();
    for (int i = 0; i < maxParticles; i++) {
        particles.push_back({
            static_cast<float>(rand() % 8),
            static_cast<float>(rand() % 8),
            0.0f,
            0.0f
        });
    }
    exploding = false;
    gathered = false;
    effectTime = 0;
}

bool EffectGravityParticles::allParticlesInCenter() {
    for (const auto &p : particles) {
        if (fabs(p.x - 3.5f) > 0.5f || fabs(p.y - 3.5f) > 0.5f) {
            return false;
        }
    }
    return true;
}

void EffectGravityParticles::updateParticles() {
    effectTime += 30; // Плавное движение

    if (!exploding && allParticlesInCenter()) {
        gathered = true;
        effectTime = 0;
        exploding = true;
        pickRandomBaseColor(); // Меняем цвет при взрыве
        for (auto &p : particles) {
            float angle = (rand() % 360) * M_PI / 180.0f;
            float speed = explosionForce * (0.5f + (rand() % 100) / 100.0f);
            p.vx = cos(angle) * speed;
            p.vy = sin(angle) * speed;
        }
    }

    if (exploding && effectTime >= explosionDuration) {
        exploding = false;
        effectTime = 0;
    }

    if (!exploding && effectTime >= gatherDuration) {
        initParticles();
    }

    for (auto &p : particles) {
        float centerX = 3.5f, centerY = 3.5f;
        float dx = centerX - p.x;
        float dy = centerY - p.y;
        float distance = sqrt(dx * dx + dy * dy) + 0.1f;

        if (!exploding) {
            p.vx += gravityStrength * (dx / distance);
            p.vy += gravityStrength * (dy / distance);
        }

        p.vx *= friction;
        p.vy *= friction;

        p.x += p.vx;
        p.y += p.vy;
    }
}

void EffectGravityParticles::render(Cube& cube, unsigned long deltaTime) {
    if (particles.empty()) {
        initParticles();
    }
    updateParticles();

    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    for (auto &face : faces) {
        face.get().clear();
        for (const auto &p : particles) {
            uint8_t r = baseR * (0.5f + 0.5f * (p.x / 8.0f));
            uint8_t g = baseG * (0.5f + 0.5f * (p.y / 8.0f));
            uint8_t b = baseB * (0.5f + 0.5f * (p.vx + p.vy));
            face.get().setPixel(static_cast<int>(p.x), static_cast<int>(p.y), (r << 16) | (g << 8) | b);
        }
    }
    cube.render();
}

