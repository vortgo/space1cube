#ifndef SETTINGS_H
#define SETTINGS_H

#include <SettingsGyver.h>
#include "ui_builder.h"

SettingsGyver sett("Space1Cube");

void setupSettings() {
    sett.begin();
    sett.onBuild(build);
    sett.onUpdate(update);
    sett.config.updateTout = 0;
    sett.config.sliderTout = 0;
}

#endif // SETTINGS_H
