#include "cube.h"
#include <cmath>
#include <vector>
#include <cstdlib>

uint32_t EffectSpiritWind::getRandomBlueShade() {
    uint8_t blue = 200 + (rand() % 55);  // Оттенки голубого
    uint8_t green = 150 + (rand() % 50); // Слабый зеленоватый оттенок
    return (0 << 16) | (green << 8) | blue;
}

void EffectSpiritWind::render(Cube& cube, unsigned long deltaTime) {
    timeSinceLastSpawn += deltaTime / 1000.0f;

    if (timeSinceLastSpawn > spawnRate) {
        timeSinceLastSpawn = 0;
        Particle p;
        p.x = 4; // Центр волны
        p.y = 4;
        float angle = ((rand() % 360) * M_PI) / 180.0f;
        p.vx = cos(angle) * waveSpeed;
        p.vy = sin(angle) * waveSpeed;
        p.color = getRandomBlueShade();
        p.life = 1.0f;
        p.initialDistance = 0;
        particles.push_back(p);
    }

    for (auto& face : cube.getFaces()) {
        face.get().clear();
    }

    for (auto& p : particles) {
        p.x += p.vx;
        p.y += p.vy;
        p.initialDistance += waveSpeed;
        p.life *= fadeRate;

        if (p.x < 0 || p.x >= 8 || p.y < 0 || p.y >= 8 || p.life < 0.1f) {
            p.life = 0; // Удаление частиц
        } else {
            uint8_t r = 0, g = (uint8_t)((p.color >> 8) & 0xFF), b = (uint8_t)(p.color & 0xFF);
            g *= p.life;
            b *= p.life;
            uint32_t fadedColor = (r << 16) | (g << 8) | b;

            for (auto& face : cube.getFaces()) {
                face.get().setPixel((int)p.x, (int)p.y, fadedColor);
            }
        }
    }

    particles.erase(std::remove_if(particles.begin(), particles.end(), [](const Particle& p) {
        return p.life < 0.1f;
    }), particles.end());

    cube.render();
}
