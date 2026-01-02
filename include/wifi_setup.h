#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <WiFi.h>

#define AP_SSID "space1cube"
#define AP_PASS "11223344"

void setupWiFi() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASS);

    // Снижаем мощность WiFi для уменьшения помех на LED
    // WIFI_POWER_2dBm - минимум, хватит на 1-2 метра
    // WIFI_POWER_5dBm - чуть больше дальность
    // WIFI_POWER_8_5dBm - баланс
    WiFi.setTxPower(WIFI_POWER_2dBm);

    Serial.println();
    Serial.print("AP Started: ");
    Serial.println(AP_SSID);
    Serial.print("IP: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("TX Power: ");
    Serial.println(WiFi.getTxPower());
}

#endif // WIFI_SETUP_H
