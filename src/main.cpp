#include <Arduino.h>
#include <iostream>

#include "config.h"

#include "components/data.h"
#include "components/app.h"
#include "components/wifi_connector.h"
#include "components/eeprom_manager.h"
#include "components/trigger_sensors_manager.h"


// index.js - меняем минуты / секунды
// маски уровней входов меняем в файле config.h

void wifiCallbackAdapter(bool isConnected);

WifiConnector wifiConnector(wifiConfig, wifiCallbackAdapter);
EEPromManager<LightsData> eePromLightsManager;
TriggerSensorsManager triggerSensorsManager(triggerSensorsPinConfig);

App app(
    wifiConnector,
    eePromLightsManager,
    triggerSensorsManager,
    appConfig
);


void wifiCallbackAdapter(bool isConnected)
    {
        app.onWifiStatusChange(isConnected);
    }


void setup()
    {
        Serial.begin(115200);
        std::cout << "Loaded, ready!" << std::endl;

        app.setup();
    }


void loop()
    {
        app.run();
    }
