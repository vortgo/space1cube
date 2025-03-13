#include "cube.h"
#include <cmath>
#include <vector>
#include <cstdlib>

EffectCyberGhost::EffectCyberGhost() {
    srand(millis());
    lastUpdateTime = 0;
    for (int i = 0; i < 3; i++) {
        addNewFlash();
    }
}

void EffectCyberGhost::addNewFlash() {
    std::vector<std::pair<int, int>> availablePixels = getAvailablePixels();
    if (availablePixels.empty()) return;

    Flash newFlash;
    newFlash.color = getRandomColor();
    newFlash.startTime = millis();
    newFlash.duration = (rand() % 1000 + 1000); // 1-2 сек
    newFlash.brightness = 0.0f;

    int flashSize = std::min(5, static_cast<int>(availablePixels.size()));
    for (int i = 0; i < flashSize && !availablePixels.empty(); i++) {
        int index = rand() % availablePixels.size();
        newFlash.particles.push_back(availablePixels[index]);
        availablePixels.erase(availablePixels.begin() + index);
    }
    flashes.push_back(newFlash);
}

uint32_t EffectCyberGhost::getRandomColor() {
    return ((rand() % 156 + 100) << 16) | ((rand() % 156 + 100) << 8) | (rand() % 156 + 100);
}

std::vector<std::pair<int, int>> EffectCyberGhost::getAvailablePixels() {
    bool occupied[8][8] = {};
    for (const auto& flash : flashes) {
        for (const auto& p : flash.particles) {
            occupied[p.first][p.second] = true;
        }
    }
    std::vector<std::pair<int, int>> available;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (!occupied[x][y]) {
                available.emplace_back(x, y);
            }
        }
    }
    return available;
}

void EffectCyberGhost::cleanupExpiredFlashes() {
    flashes.erase(
        std::remove_if(flashes.begin(), flashes.end(), [](const Flash& flash) {
            return millis() - flash.startTime >= flash.duration;
        }),
        flashes.end()
    );
    while (flashes.size() < 3) {
        addNewFlash();
    }
}

void EffectCyberGhost::render(Cube& cube, unsigned long deltaTime) {
    lastUpdateTime += deltaTime;
    cleanupExpiredFlashes();
    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    for (auto &face : faces) {
        face.get().clear();
        unsigned long currentTime = millis();
        for (auto& flash : flashes) {
            unsigned long elapsed = currentTime - flash.startTime;
            float progress = (float)elapsed / flash.duration;

            if (progress < 0.3f) {
                flash.brightness = progress / 0.3f;
            } else if (progress < 0.8f) {
                flash.brightness = 1.0f;
            } else if (progress < 1.0f) {
                flash.brightness = (1.0f - (progress - 0.8f) / 0.2f);
            } else {
                flash.brightness = 0.0f;
            }

            for (const auto& p : flash.particles) {
                int offsetX = (rand() % 3) - 1; // -1, 0, 1
                int offsetY = (rand() % 3) - 1;
                int newX = std::max(0, std::min(7, p.first + offsetX));
                int newY = std::max(0, std::min(7, p.second + offsetY));

                uint8_t r = ((flash.color >> 16) & 0xFF) * flash.brightness;
                uint8_t g = ((flash.color >> 8) & 0xFF) * flash.brightness;
                uint8_t b = (flash.color & 0xFF) * flash.brightness;
                uint32_t fadedColor = (r << 16) | (g << 8) | b;
                face.get().setPixel(newX, newY, fadedColor);
            }
        }
    }
    cube.render();
}