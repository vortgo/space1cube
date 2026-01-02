#include "cube.h"
#include <cmath>

EffectHeartbeatPulse::EffectHeartbeatPulse() {
    time = 0;
    currentRadius = 0;
}

void EffectHeartbeatPulse::render(Cube& cube, unsigned long deltaTime) {
    float beatDuration = 60000.0f / bpm; // мс на удар
    time += deltaTime;

    // Фаза в цикле удара (0-1)
    float phase = fmodf(time, beatDuration) / beatDuration;

    // Паттерн сердцебиения: два быстрых удара, потом пауза
    float intensity = 0;
    if (phase < 0.1f) {
        // Первый удар
        intensity = sinf(phase / 0.1f * M_PI);
    } else if (phase >= 0.15f && phase < 0.25f) {
        // Второй удар
        intensity = sinf((phase - 0.15f) / 0.1f * M_PI) * 0.7f;
    }

    currentRadius = intensity * 6.0f;

    std::vector<std::reference_wrapper<Matrix>> faces = cube.getFaces();
    for (auto& faceRef : faces) {
        Matrix& face = faceRef.get();

        float cx = GRID_W / 2.0f - 0.5f;
        float cy = GRID_H / 2.0f - 0.5f;

        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                float dx = x - cx;
                float dy = y - cy;
                float dist = sqrtf(dx * dx + dy * dy);

                float brightness = 0;
                if (dist <= currentRadius) {
                    brightness = 1.0f - (dist / (currentRadius + 0.1f));
                }

                if (brightness > 0) {
                    uint8_t r = (uint8_t)(((pulseColor >> 16) & 0xFF) * brightness);
                    uint8_t g = (uint8_t)(((pulseColor >> 8) & 0xFF) * brightness);
                    uint8_t b = (uint8_t)((pulseColor & 0xFF) * brightness);
                    face.setPixel(x, y, (r << 16) | (g << 8) | b);
                } else {
                    face.setPixel(x, y, 0);
                }
            }
        }
    }

    cube.render();
}
