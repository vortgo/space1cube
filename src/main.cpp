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


  cube->setActiveEffect(CubeEffects::HEART);
}

void loop()
{
  cube->tick();
  sett.tick();
}
