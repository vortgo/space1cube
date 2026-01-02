#include <Arduino.h>
#include "cube.h"
#include "logger.h"
#include "settings.h"
#include "wifi_setup.h"
#include "../include/cube.h"

#define SETT_NO_DB
#define SETT_NO_TABLE

int analogPin = 35;
float voltage = 0.0;

// Рисует символ на одной грани
void drawCharOnFace(Matrix& face, const std::string& character, uint32_t color) {
    int colOffset = 1;
    int rowOffset = 1;

    auto it = charPatterns.find(character);
    if (it == charPatterns.end()) {
        return;
    }

    const Pattern& charPattern = it->second;
    for (int r = 0; r < 7; r++) {
        uint8_t rowPattern = charPattern[r];
        for (int c = 0; c < 5; c++) {
            if (rowPattern & (1 << (4 - c))) {
                int x = c + colOffset;
                int y = r + rowOffset;
                face.setPixel(x, y, color);
            }
        }
    }
}

// Отображает номера граней разными цветами
void showFaceNumbers() {
    cube->clear();

    // Цвета для каждой грани
    drawCharOnFace(cube->front,  "1", 0xFF0000);  // Красный
    drawCharOnFace(cube->back,   "2", 0x00FF00);  // Зелёный
    drawCharOnFace(cube->left,   "3", 0x0000FF);  // Синий
    drawCharOnFace(cube->right,  "4", 0xFFFF00);  // Жёлтый
    drawCharOnFace(cube->top,    "5", 0xFF00FF);  // Пурпурный
    drawCharOnFace(cube->bottom, "6", 0x00FFFF);  // Голубой

    cube->render();
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

  logger.println("initMatrices");
  initCube();

  // Показываем номера граней при старте
  showFaceNumbers();
  delay(500);

  logger.println("setupWiFi");
  setupWiFi();

  delay(200);
  // Меняем цвет цифр на зелёный после подключения WiFi
  drawCharOnFace(cube->front,  "1", 0x03ad00);
  drawCharOnFace(cube->back,   "2", 0x03ad00);
  drawCharOnFace(cube->left,   "3", 0x03ad00);
  drawCharOnFace(cube->right,  "4", 0x03ad00);
  drawCharOnFace(cube->top,    "5", 0x03ad00);
  drawCharOnFace(cube->bottom, "6", 0x03ad00);
  cube->render();
  delay(500);

  logger.println("setupSettings");
  setupSettings();

  cube->setActiveEffect(CubeEffects::VORTEX);
  std::vector<std::reference_wrapper<Matrix>> faces = cube->getFaces();
  for (auto face : faces)
  {
    face.get().maxBrightness = 0.5f;
  }
}

unsigned long previousMillis = 0;
const long interval = 5000;

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Считываем напряжение
    int adcValue = analogRead(analogPin);
    voltage = adcValue * (3.3 / 4095.0); // Преобразуем в напряжение
    cube->setVoltage(voltage);
    logger.print("Напряжение: ");
    logger.print(voltage);
    logger.println(" В");
  }


  cube->tick();
  sett.tick();
}
