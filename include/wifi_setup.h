#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <WiFi.h>

#define AP_SSID "space1cube"
#define AP_PASS "11223344"

void setupWiFi() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASS);

    Serial.println();
    Serial.print("AP Started: ");
    Serial.println(AP_SSID);
    Serial.print("IP: ");
    Serial.println(WiFi.softAPIP());
}

#endif // WIFI_SETUP_H
