#include "cube.h"

EffectCrawlingLight::EffectCrawlingLight() {
    buildEdgePath();
    currentIdx = 0;
    moveTimer = 0;
    color = getRandomColor();
}

uint32_t EffectCrawlingLight::getRandomColor() {
    uint32_t colors[] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF, 0x00FFFF, 0xFFFFFF};
    return colors[random(7)];
}

void EffectCrawlingLight::buildEdgePath() {
    path.clear();

    // Build a path along all edges of the cube
    // Front face edges
    for (int x = 0; x < GRID_W; x++) { path.push_back({0, x, 0}); }
    // Right face top edge
    for (int x = 0; x < GRID_W; x++) { path.push_back({3, x, 0}); }
    // Back face top edge (reversed)
    for (int x = GRID_W - 1; x >= 0; x--) { path.push_back({1, x, 0}); }
    // Left face top edge (reversed)
    for (int x = GRID_W - 1; x >= 0; x--) { path.push_back({2, x, 0}); }

    // Front face right edge
    for (int y = 0; y < GRID_H; y++) { path.push_back({0, GRID_W - 1, y}); }
    // Front face bottom edge
    for (int x = GRID_W - 1; x >= 0; x--) { path.push_back({0, x, GRID_H - 1}); }
    // Front face left edge
    for (int y = GRID_H - 1; y >= 0; y--) { path.push_back({0, 0, y}); }

    // Top face perimeter
    for (int x = 0; x < GRID_W; x++) { path.push_back({4, x, 0}); }
    for (int y = 0; y < GRID_H; y++) { path.push_back({4, GRID_W - 1, y}); }
    for (int x = GRID_W - 1; x >= 0; x--) { path.push_back({4, x, GRID_H - 1}); }
    for (int y = GRID_H - 1; y >= 0; y--) { path.push_back({4, 0, y}); }

    // Bottom face perimeter
    for (int x = 0; x < GRID_W; x++) { path.push_back({5, x, 0}); }
    for (int y = 0; y < GRID_H; y++) { path.push_back({5, GRID_W - 1, y}); }
    for (int x = GRID_W - 1; x >= 0; x--) { path.push_back({5, x, GRID_H - 1}); }
    for (int y = GRID_H - 1; y >= 0; y--) { path.push_back({5, 0, y}); }
}

void EffectCrawlingLight::render(Cube& cube, unsigned long deltaTime) {
    moveTimer += speed;
    if (moveTimer >= 1.0f) {
        moveTimer = 0;
        currentIdx++;
        if (currentIdx >= (int)path.size()) {
            currentIdx = 0;
            color = getRandomColor();
        }
    }

    cube.clear();
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    // Draw trail
    for (int i = 0; i < trailLength; i++) {
        int idx = currentIdx - i;
        if (idx < 0) idx += path.size();

        EdgePoint& pt = path[idx];
        float brightness = 1.0f - (float)i / trailLength;

        uint8_t r = (uint8_t)(((color >> 16) & 0xFF) * brightness);
        uint8_t g = (uint8_t)(((color >> 8) & 0xFF) * brightness);
        uint8_t b = (uint8_t)((color & 0xFF) * brightness);

        faces[pt.face]->setPixel(pt.x, pt.y, (r << 16) | (g << 8) | b);
    }

    cube.render();
}
