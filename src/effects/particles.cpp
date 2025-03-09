#include "cube.h"

EffectPhysicsParticles::EffectPhysicsParticles(int numParticles, float speed)
    : maxParticles(numParticles), baseSpeed(speed) {
    initParticles();
}

void EffectPhysicsParticles::initParticles() {
    particles.clear();
    for (int i = 0; i < maxParticles; i++) {
        float angle = (rand() % 360) * M_PI / 180.0f;
        float speed = baseSpeed * (0.8f + (rand() % 100) / 100.0f);
        particles.push_back({
            static_cast<float>(rand() % 8), static_cast<float>(rand() % 8),
            cos(angle) * speed, sin(angle) * speed,
            static_cast<uint8_t>(rand() % 256), static_cast<uint8_t>(rand() % 256), static_cast<uint8_t>(rand() % 256),
            true
        });
    }
}

uint32_t EffectPhysicsParticles::randomColor() {
    return ((rand() % 256) << 16) | ((rand() % 256) << 8) | (rand() % 256);
}

void EffectPhysicsParticles::updateParticles(unsigned long deltaTime) {
    float speedFactor = deltaTime * 0.055f; // Увеличен множитель скорости

    for (auto &p : particles) {
        if (!p.active) continue;
        p.x += p.vx * speedFactor;
        p.y += p.vy * speedFactor;

        if (p.x < 0 || p.x >= 8) { p.vx = -p.vx; p.x = std::max(0.0f, std::min(7.9f, p.x)); }
        if (p.y < 0 || p.y >= 8) { p.vy = -p.vy; p.y = std::max(0.0f, std::min(7.9f, p.y)); }
    }
    handleCollisions();
}

void EffectPhysicsParticles::handleCollisions() {
    for (size_t i = 0; i < particles.size(); i++) {
        for (size_t j = i + 1; j < particles.size(); j++) {
            float dx = particles[i].x - particles[j].x;
            float dy = particles[i].y - particles[j].y;
            float distSq = dx * dx + dy * dy;
            if (distSq < 0.5f) {
                // Рассчитываем нормализованный вектор столкновения
                float distance = sqrt(distSq) + 0.01f; // Добавляем небольшое значение для избежания деления на ноль
                float nx = dx / distance;
                float ny = dy / distance;

                // Рассчитываем разницу в скорости вдоль нормали столкновения
                float dvx = particles[i].vx - particles[j].vx;
                float dvy = particles[i].vy - particles[j].vy;
                float impact = dvx * nx + dvy * ny;

                // Если частицы уже разлетаются - игнорируем
                if (impact > 0) continue;

                // Обновляем скорости с учетом сохранения импульса
                float restitution = 1.0f; // Коэффициент упругости (1 - полностью упругое столкновение)
                float impulse = -2.0f * impact / 2.0f;
                particles[i].vx += impulse * nx;
                particles[i].vy += impulse * ny;
                particles[j].vx -= impulse * nx;
                particles[j].vy -= impulse * ny;

                // Изменение цвета только одной частицы
                if (rand() % 2 == 0) {
                    particles[i].r = rand() % 256;
                    particles[i].g = rand() % 256;
                    particles[i].b = rand() % 256;
                } else {
                    particles[j].r = rand() % 256;
                    particles[j].g = rand() % 256;
                    particles[j].b = rand() % 256;
                }
            }
        }
    }
}

void EffectPhysicsParticles::render(Cube& cube, unsigned long deltaTime) {
    updateParticles(deltaTime);

    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    for (auto &face : faces) {
        face.get().clear();
        for (const auto &p : particles) {
            if (!p.active) continue;
            face.get().setPixel(static_cast<int>(p.x), static_cast<int>(p.y), (p.r << 16) | (p.g << 8) | p.b);
        }
    }
    cube.render();
}