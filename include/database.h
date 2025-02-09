#ifndef DATABASE_H
#define DATABASE_H

#include <GyverDBFile.h>
#include <LittleFS.h>

GyverDBFile db(&LittleFS, "/data2.db");

void setupDatabase() {
#ifdef ESP32
    LittleFS.begin(true);
#else
    LittleFS.begin();
#endif
    db.begin();
}

#endif // DATABASE_H
