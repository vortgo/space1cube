#include "cube.h"
#include <cmath>

EffectBouncingBall3D::EffectBouncingBall3D() {
    // Position in 3D space (0-7 range for 8x8 cube)
    posX = 4.0f;
    posY = 4.0f;
    posZ = 4.0f;

    // Random initial velocity
    velX = (random(100) - 50) / 100.0f * 0.5f;
    velY = (random(100) - 50) / 100.0f * 0.5f;
    velZ = (random(100) - 50) / 100.0f * 0.5f;

    ballColor = 0xFF0000;
}

void EffectBouncingBall3D::updatePhysics(float dt) {
    // Update position
    posX += velX * dt * speed;
    posY += velY * dt * speed;
    posZ += velZ * dt * speed;

    // Bounce off walls
    if (posX < 0) { posX = 0; velX = -velX; ballColor = 0xFF0000; }
    if (posX >= GRID_W) { posX = GRID_W - 0.1f; velX = -velX; ballColor = 0x00FF00; }
    if (posY < 0) { posY = 0; velY = -velY; ballColor = 0x0000FF; }
    if (posY >= GRID_H) { posY = GRID_H - 0.1f; velY = -velY; ballColor = 0xFFFF00; }
    if (posZ < 0) { posZ = 0; velZ = -velZ; ballColor = 0xFF00FF; }
    if (posZ >= GRID_W) { posZ = GRID_W - 0.1f; velZ = -velZ; ballColor = 0x00FFFF; }
}

void EffectBouncingBall3D::renderOnFaces(Cube& cube) {
    Matrix* faces[] = {&cube.front, &cube.back, &cube.left, &cube.right, &cube.top, &cube.bottom};

    int bx = (int)posX;
    int by = (int)posY;
    int bz = (int)posZ;

    // Calculate distance to each face and draw with appropriate brightness
    // Front face (z = 0)
    float distFront = posZ;
    float brightFront = 1.0f - distFront / GRID_W;
    if (brightFront > 0.1f) {
        uint8_t r = (uint8_t)(((ballColor >> 16) & 0xFF) * brightFront);
        uint8_t g = (uint8_t)(((ballColor >> 8) & 0xFF) * brightFront);
        uint8_t b = (uint8_t)((ballColor & 0xFF) * brightFront);
        faces[0]->setPixel(bx, by, (r << 16) | (g << 8) | b);
    }

    // Back face (z = 7)
    float distBack = GRID_W - posZ;
    float brightBack = 1.0f - distBack / GRID_W;
    if (brightBack > 0.1f) {
        uint8_t r = (uint8_t)(((ballColor >> 16) & 0xFF) * brightBack);
        uint8_t g = (uint8_t)(((ballColor >> 8) & 0xFF) * brightBack);
        uint8_t b = (uint8_t)((ballColor & 0xFF) * brightBack);
        faces[1]->setPixel(GRID_W - 1 - bx, by, (r << 16) | (g << 8) | b);
    }

    // Left face (x = 0)
    float distLeft = posX;
    float brightLeft = 1.0f - distLeft / GRID_W;
    if (brightLeft > 0.1f) {
        uint8_t r = (uint8_t)(((ballColor >> 16) & 0xFF) * brightLeft);
        uint8_t g = (uint8_t)(((ballColor >> 8) & 0xFF) * brightLeft);
        uint8_t b = (uint8_t)((ballColor & 0xFF) * brightLeft);
        faces[2]->setPixel(bz, by, (r << 16) | (g << 8) | b);
    }

    // Right face (x = 7)
    float distRight = GRID_W - posX;
    float brightRight = 1.0f - distRight / GRID_W;
    if (brightRight > 0.1f) {
        uint8_t r = (uint8_t)(((ballColor >> 16) & 0xFF) * brightRight);
        uint8_t g = (uint8_t)(((ballColor >> 8) & 0xFF) * brightRight);
        uint8_t b = (uint8_t)((ballColor & 0xFF) * brightRight);
        faces[3]->setPixel(GRID_W - 1 - bz, by, (r << 16) | (g << 8) | b);
    }

    // Top face (y = 0)
    float distTop = posY;
    float brightTop = 1.0f - distTop / GRID_H;
    if (brightTop > 0.1f) {
        uint8_t r = (uint8_t)(((ballColor >> 16) & 0xFF) * brightTop);
        uint8_t g = (uint8_t)(((ballColor >> 8) & 0xFF) * brightTop);
        uint8_t b = (uint8_t)((ballColor & 0xFF) * brightTop);
        faces[4]->setPixel(bx, GRID_H - 1 - bz, (r << 16) | (g << 8) | b);
    }

    // Bottom face (y = 7)
    float distBottom = GRID_H - posY;
    float brightBottom = 1.0f - distBottom / GRID_H;
    if (brightBottom > 0.1f) {
        uint8_t r = (uint8_t)(((ballColor >> 16) & 0xFF) * brightBottom);
        uint8_t g = (uint8_t)(((ballColor >> 8) & 0xFF) * brightBottom);
        uint8_t b = (uint8_t)((ballColor & 0xFF) * brightBottom);
        faces[5]->setPixel(bx, bz, (r << 16) | (g << 8) | b);
    }
}

void EffectBouncingBall3D::render(Cube& cube, unsigned long deltaTime) {
    updatePhysics(deltaTime);
    cube.clear();
    renderOnFaces(cube);
    cube.render();
}
