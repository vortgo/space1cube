#ifndef SETTINGS_H
#define SETTINGS_H

#include <SettingsGyver.h>
#include "database.h"
#include "ui_builder.h"

SettingsGyver sett("Space Cube", &db);

void setupSettings() {
    sett.begin();
    sett.onBuild(build);
    sett.onUpdate(update);

    db.init(kk::matrix1Leds, "");
    

    
    db.init(kk::label, "label");
    db.init(kk::led, 1);
    db.init(kk::paragr, "paragraph");
    db.init(kk::input, "input");
    db.init(kk::number, 123456);
    db.init(kk::pass, "pass");
    db.init(kk::color, 0xff0000);
    db.init(kk::sw, true);
    db.init(kk::datew, 1728123055);
    db.init(kk::timew, 12 * 3600 + 30 * 60);
    db.init(kk::datetime, 1728123055);
    db.init(kk::slider, 33);
    db.init(kk::sel, 1);
}

#endif // SETTINGS_H
