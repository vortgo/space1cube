#include <Arduino.h>
#include "wifi_setup.h"
#include "database.h"
#include "settings.h"
#include "logger.h"
#include "cube.h"

void setup()
{
  Serial.begin(115200);
  Serial.println();

  logger.println("setupWiFi");
  setupWiFi();
  logger.println("setupDatabase");
  setupDatabase();
  logger.println("setupSettings");
  setupSettings();
  logger.println("initMatrices");
  initCube();

  cube->setActiveEffect(CubeEffects::FALING_STAR);
  std::vector<std::reference_wrapper<Matrix>> faces = cube->getFaces();
  for (int f = 0; f < faces.size(); f++)
  {
    faces[f].get().maxBrightness = 0.8f;
  }
}

void loop()
{
  cube->tick();
  sett.tick();
}
