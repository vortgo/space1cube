#include "cube.h"
#include <cmath>
#include <vector>
#include <cstdlib>

void EffectVortex::generateNewColorScheme() {
    baseColor = getRandomVortexColor();
    colorShades.clear();
    for (int i = 0; i < 4; i++) {
        uint8_t r = ((baseColor >> 16) & 0xFF) * (0.8f + 0.05f * i);
        uint8_t g = ((baseColor >> 8) & 0xFF) * (0.8f + 0.05f * i);
        uint8_t b = (baseColor & 0xFF) * (0.8f + 0.05f * i);
        colorShades.push_back((r << 16) | (g << 8) | b);
    }
}

uint32_t EffectVortex::getShade(int index) {
    if (colorShades.empty()) {
        generateNewColorScheme();
    }
    return colorShades[index % colorShades.size()];
}

uint32_t EffectVortex::getRandomVortexColor() {
    uint8_t r = 100 + (rand() % 156);
    uint8_t g = 100 + (rand() % 156);
    uint8_t b = 100 + (rand() % 156);
    return (r << 16) | (g << 8) | b;
}

uint32_t EffectVortex::interpolateColors(uint32_t color1, uint32_t color2, float t) {
    uint8_t r1 = (color1 >> 16) & 0xFF;
    uint8_t g1 = (color1 >> 8) & 0xFF;
    uint8_t b1 = color1 & 0xFF;

    uint8_t r2 = (color2 >> 16) & 0xFF;
    uint8_t g2 = (color2 >> 8) & 0xFF;
    uint8_t b2 = color2 & 0xFF;

    uint8_t r = r1 + t * (r2 - r1);
    uint8_t g = g1 + t * (g2 - g1);
    uint8_t b = b1 + t * (b2 - b1);

    return (r << 16) | (g << 8) | b;
}

void EffectVortex::render(Cube& cube, unsigned long deltaTime) {
    timeSinceLastSpawn += deltaTime / 1000.0f;
    colorTransitionProgress += deltaTime / 5000.0f;
    if (colorTransitionProgress >= 1.0f) {
        generateNewColorScheme();
        colorTransitionProgress = 0.0f;
    }

    if (timeSinceLastSpawn > spawnRate) {
        timeSinceLastSpawn = 0;
        if (colorShades.empty()) generateNewColorScheme();
        Particle p;
        p.x = 4;
        p.y = 4;
        p.angle = (rand() % 360) * M_PI / 180.0f;
        p.radius = 0;
        p.angularSpeed = 0.05f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.1f));
        p.color = getShade(0);
        p.life = 1.0f;
        p.cycleCount = 0;
        particles.push_back(p);
    }

    for (auto& face : cube.getFaces()) {
        face.get().clear();
    }

    for (auto& p : particles) {
        p.angle += p.angularSpeed;
        p.radius += expansionRate;
        p.life *= fadeRate;

        if (colorShades.empty()) generateNewColorScheme();
        if (p.angle >= p.cycleCount * colorChangeThreshold) {
            p.cycleCount++;
            uint32_t nextColor = getShade(p.cycleCount);
            p.color = interpolateColors(p.color, nextColor, colorTransitionProgress);
        }

        p.x = 4 + cos(p.angle) * p.radius;
        p.y = 4 + sin(p.angle) * p.radius;

        if (p.x < 0 || p.x >= 8 || p.y < 0 || p.y >= 8 || p.life < 0.1f) {
            p.life = 0;
        } else {
            uint8_t r = (p.color >> 16) & 0xFF;
            uint8_t g = (p.color >> 8) & 0xFF;
            uint8_t b = p.color & 0xFF;
            r *= p.life;
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