#include "cube.h"
#include <cmath>
#include <vector>
#include <cstdlib>

uint32_t EffectRaindropRipples::getRandomColor() {
    return (rand() % 256 << 16) | (rand() % 256 << 8) | (rand() % 256);
}

uint32_t EffectRaindropRipples::interpolateColor(uint32_t baseColor, float distanceFactor) {
    uint8_t r = (baseColor >> 16) & 0xFF;
    uint8_t g = (baseColor >> 8) & 0xFF;
    uint8_t b = baseColor & 0xFF;

    r = r * (1.0f - distanceFactor);
    g = g * (1.0f - distanceFactor);
    b = b * (1.0f - distanceFactor);

    return (r << 16) | (g << 8) | b;
}

void EffectRaindropRipples::spawnRaindrop() {
    ripple.x = rand() % 8;
    ripple.y = rand() % 8;
    ripple.radii[0] = 0.0f;
    ripple.radii[1] = -2.0f;
    ripple.radii[2] = -4.0f;
    ripple.time = 0.0f;
    ripple.color = getRandomColor();
    rippleActive = true;
}

void EffectRaindropRipples::render(Cube& cube, unsigned long deltaTime) {
    timeSinceLastDrop += deltaTime / 1000.0f;

    if (!rippleActive && timeSinceLastDrop > dropInterval) {
        spawnRaindrop();
        timeSinceLastDrop = 0.0f;
    }

    for (auto& face : cube.getFaces()) {
        face.get().clear();
    }

    if (rippleActive) {
        ripple.time += deltaTime / 1000.0f;
        for (int i = 0; i < 3; i++) {
            ripple.radii[i] += deltaTime / 1000.0f * (3.5f - i * 0.8f); // Разная скорость распространения волн
        }

        for (int i = 0; i < 3; i++) {
            if (ripple.radii[i] >= 8.0f) {
                rippleActive = false;
            }
        }

        for (int i = 0; i < 3; i++) {
            for (int dx = -8; dx <= 8; dx++) {
                for (int dy = -8; dy <= 8; dy++) {
                    int nx = ripple.x + dx;
                    int ny = ripple.y + dy;
                    float distance = std::sqrt(dx * dx + dy * dy);
                    if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8 && std::abs(distance - ripple.radii[i]) < 0.3f) {
                        uint32_t color = interpolateColor(ripple.color, distance / 8.0f);
                        for (auto& face : cube.getFaces()) {
                            face.get().setPixel(nx, ny, color);
                        }
                    }
                }
            }
        }
    }

    cube.render();
}

