#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <WiFi.h>

#define WIFI_SSID ""
#define WIFI_PASS ""

void setupWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    
    uint8_t tries = 20;
    while (WiFi.status() != WL_CONNECTED) {
        delay(1500);
        Serial.print(".");
        if (!--tries) break;
    }
    Serial.println();
    Serial.print("Connected: ");
    Serial.println(WiFi.localIP());
}

#endif // WIFI_SETUP_H
