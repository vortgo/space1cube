#include <Arduino.h>
#include "wifi_setup.h"
#include "settings.h"
#include "logger.h"
#include "cube.h"
#define SETT_NO_DB

void setup()
{
  Serial.begin(115200);
  Serial.println();

  logger.println("setupWiFi");
  setupWiFi();
  logger.println("setupSettings");
  setupSettings();
  logger.println("initMatrices");
  initCube();

  cube->setActiveEffect(CubeEffects::DICE);
  std::vector<std::reference_wrapper<Matrix>> faces = cube->getFaces();
  for (auto face : faces)
  {
    face.get().maxBrightness = 0.8f;
  }
}

void loop()
{
  cube->tick();
  sett.tick();
}
