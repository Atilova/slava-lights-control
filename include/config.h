#pragma once

#include <Arduino.h>
#include <IPAddress.h>

#include "secret.h"

#include "components/app.h"
#include "components/wifi_connector.h"
#include "components/eeprom_manager.h"
#include "components/trigger_sensors_manager.h"


WifiConfig wifiConfig = {
    .ESP_IP = IPAddress(192, 168, 1, 200),
    .GATEWAY = IPAddress(192, 168, 1, 1),
    .SUBNET_MASK = IPAddress(255, 255, 255, 0),
    .PRIMARY_DNS = IPAddress(192, 168, 1, 1),
    .SECONDARY_DNS = IPAddress(8, 8, 8, 8),
    .SSID = secretConfig.WIFI_SSID,
    .PASSWORD = secretConfig.WIFI_PASSWORD
};


TriggerSensorsPinConfig triggerSensorsPinConfig = {
    .DRIVEWAY_GATES = D4,  // d7
    .YARD_GATE = D6,  // d6
    .FRONT_DOOR = D7,  // d4
    .ACTIVE_LEVEL_DRIVEWAY_GATES = LOW,
    .ACTIVE_LEVEL_YARD_GATE = HIGH,
    .ACTIVE_LEVEL_FRONT_DOOR = HIGH
};


AppConfig appConfig = {
    .ESP_RELAY_PIN = D3  // d3 Gpio5
};
