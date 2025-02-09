#include <Arduino.h>
#include "wifi_setup.h"
#include "database.h"
#include "settings.h"
#include "logger.h"
#include "matrix.h"

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.println("setupWiFi");
  setupWiFi();
  Serial.println("setupDatabase");
  setupDatabase();
  Serial.println("setupSettings");
  setupSettings();
  Serial.println("initMatrices");
  initMatrices();
  Serial.println("setupSettings");
}

const uint32_t TICK_INTERVAL = 5;
uint32_t lastTickTime = 0;
static bool paused = false;
static uint32_t pauseStartTime = 0;

bool up = true;
uint8_t br = 0;

void work()
{
  uint32_t now = millis();

  if ((uint32_t)(now - lastTickTime) < TICK_INTERVAL)
    return;
  if (paused && now - pauseStartTime < 150)
    return;
  paused = false;

  Serial.println("Updating brightness...");
  lastTickTime = now;

  br += (up ? 5 : -5);
  br = constrain(br, 0, 255);

  Serial.println(br);
  Serial.println("Setting pixel...");

  if (br > 220 || br < 3)
  {
    up = !up;
    paused = true;
    pauseStartTime = now;
  }

  matrices[0].setPixel(2, 2, 0x004CFF, br);
  matrices[0].show();
}

void loop()
{
  work();
  sett.tick();
}
