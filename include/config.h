#pragma once

#include <Arduino.h>
#include <IPAddress.h>

#include "secret.h"

#include "components/app.h"
#include "components/wifi_connector.h"
#include "components/eeprom_manager.h"
#include "components/trigger_sensors_manager.h"


WifiConfig wifiConfig = {
    .ESP_IP = secretConfig.ESP_IP,
    .GATEWAY = secretConfig.GATEWAY,
    .SUBNET_MASK = secretConfig.SUBNET_MASK,
    .SSID = secretConfig.WIFI_SSID,
    .PASSWORD = secretConfig.WIFI_PASSWORD
    //.INDICATION_PIN = 16 // раскомментровать, если нужно изменить с LED_BUILTIN на какой-то вывод
};


TriggerSensorsPinConfig triggerSensorsPinConfig = {
    #if defined(ESP32)
        .DRIVEWAY_GATES = 12,
        .YARD_GATE = 13,
        .FRONT_DOOR = 14,
    #elif defined(ESP8266)
        .DRIVEWAY_GATES = D7,  // D7
        .YARD_GATE = D6,  // D6
        .FRONT_DOOR = D4,  // D4
    #else
        #error "This code is for ESP32 or ESP8266 only."
    #endif

    .ACTIVE_LEVEL_DRIVEWAY_GATES = LOW,
    .ACTIVE_LEVEL_YARD_GATE = LOW,
    .ACTIVE_LEVEL_FRONT_DOOR = HIGH
};


AppConfig appConfig = {
    #if defined(ESP32)
        .ESP_RELAY_PIN = 16,
    #elif defined(ESP8266)
        .ESP_RELAY_PIN = D3,  // D3 Gpio5
    #else
        #error "This code is for ESP32 or ESP8266 only."
    #endif
    .RELAY_LEVEL_ON = HIGH
};
