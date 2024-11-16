#include <Arduino.h>

#include "config.h"

#include "components/data.h"
#include "components/app.h"
#include "components/wifi_connector.h"
#include "components/eeprom_manager.h"
#include "components/trigger_sensors_manager.h"

// index.js - меняем минуты / секунды


WifiConnector wifiConnector(wifiConfig);
EEPromManager<LightsData> eePromLightsManager;
TriggerSensorsManager triggerSensorsManager(triggerSensorsPinConfig);

App app(
    wifiConnector,
    eePromLightsManager,
    triggerSensorsManager,
    appConfig
);


void setup()
    {
        Serial.begin(115200);
        app.setup();
    }


void loop()
    {
        app.run();
    }
