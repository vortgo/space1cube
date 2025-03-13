#include "cube.h"
#include <cmath>
#include <vector>
#include <cstdlib>

EffectDynamicGroups::EffectDynamicGroups() {
    srand(millis());
    lastUpdateTime = 0;
    for (int i = 0; i < 3; i++) {
        addNewGroup();
    }
}

void EffectDynamicGroups::addNewGroup() {
    std::vector<std::pair<int, int>> availablePixels = getAvailablePixels();
    if (availablePixels.empty()) return;

    Group newGroup;
    newGroup.color = getRandomColor();
    newGroup.startTime = millis();
    newGroup.duration = (rand() % 4000 + 4000); // 4-8 сек
    newGroup.brightness = 0.0f;

    int groupSize = std::max(1, static_cast<int>(availablePixels.size()) / std::max(1, (4 - static_cast<int>(groups.size()))));
    for (int i = 0; i < groupSize && !availablePixels.empty(); i++) {
        int index = rand() % availablePixels.size();
        newGroup.pixels.push_back(availablePixels[index]);
        availablePixels.erase(availablePixels.begin() + index);
    }
    groups.push_back(newGroup);
}

uint32_t EffectDynamicGroups::getRandomColor() {
    return ((rand() % 156 + 100) << 16) | ((rand() % 156 + 100) << 8) | (rand() % 156 + 100);
}

std::vector<std::pair<int, int>> EffectDynamicGroups::getAvailablePixels() {
    bool occupied[8][8] = {};
    for (const auto& group : groups) {
        for (const auto& p : group.pixels) {
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

void EffectDynamicGroups::cleanupExpiredGroups() {
    groups.erase(
        std::remove_if(groups.begin(), groups.end(), [](const Group& group) {
            return millis() - group.startTime >= group.duration;
        }),
        groups.end()
    );
    while (groups.size() < 3) {
        addNewGroup();
    }
}

void EffectDynamicGroups::render(Cube& cube, unsigned long deltaTime) {
    lastUpdateTime += deltaTime;
    cleanupExpiredGroups();
    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    for (auto &face : faces) {
        face.get().clear();
        unsigned long currentTime = millis();
        for (auto& group : groups) {
            unsigned long elapsed = currentTime - group.startTime;
            float progress = (float)elapsed / group.duration;

            if (progress < 0.3f) {
                group.brightness = progress / 0.3f;
            } else if (progress < 0.8f) {
                group.brightness = 1.0f;
            } else if (progress < 1.0f) {
                group.brightness = (1.0f - (progress - 0.8f) / 0.2f);
            } else {
                group.brightness = 0.0f;
            }

            uint8_t r = ((group.color >> 16) & 0xFF) * group.brightness;
            uint8_t g = ((group.color >> 8) & 0xFF) * group.brightness;
            uint8_t b = (group.color & 0xFF) * group.brightness;
            uint32_t fadedColor = (r << 16) | (g << 8) | b;
            for (const auto& p : group.pixels) {
                face.get().setPixel(p.first, p.second, fadedColor);
            }
        }
    }
    cube.render();
}
