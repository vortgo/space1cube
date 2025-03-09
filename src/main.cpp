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

void setup()
{
  Serial.begin(115200);
  Serial.println();
  
  logger.println("initMatrices");
  initCube();

  cube->effectSymbol.print("@", 0x850000);
  cube->setActiveEffect(CubeEffects::SYMBOL);
  cube->tick();
  delay(500);

  logger.println("setupWiFi");
  setupWiFi();

  delay(200);
  cube->effectSymbol.print("@", 0x03ad00);
  cube->tick();
  delay(500);

  logger.println("setupSettings");
  setupSettings();

  cube->setActiveEffect(CubeEffects::PARTICLES);
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
