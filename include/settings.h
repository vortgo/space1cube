#ifndef SETTINGS_H
#define SETTINGS_H

#include <SettingsAsync.h>
#include "ui_builder.h"

SettingsAsync  sett("Space1Cube");

void setupSettings() {
    sett.begin();
    sett.onBuild(build);
    sett.onUpdate(update);
}

#endif // SETTINGS_H
