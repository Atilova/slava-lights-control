#pragma once

#include <Arduino.h>

#include "data.h"


struct TriggerSensorsPinConfig {
    const uint8_t DRIVEWAY_GATES;
    const uint8_t YARD_GATE;
    const uint8_t FRONT_DOOR;

    const bool ACTIVE_LEVEL_DRIVEWAY_GATES = HIGH;
    const bool ACTIVE_LEVEL_YARD_GATE = HIGH;
    const bool ACTIVE_LEVEL_FRONT_DOOR = HIGH;
};


class TriggerSensorsManager {
    TriggerSensorsPinConfig* config = nullptr;

    public:
        TriggerSensorsManager(TriggerSensorsPinConfig &config) : config(&config) {}

        ~TriggerSensorsManager() {}

        void setup()
            {
                pinMode(config->DRIVEWAY_GATES, INPUT);
                pinMode(config->YARD_GATE, INPUT);
                pinMode(config->FRONT_DOOR, INPUT);
            }

        bool readSensor(bool mask, uint8_t pin, bool activeLevel)
            {
                return mask && (digitalRead(pin) == activeLevel);
            }

        bool isAnyActiveFromMask(LightsData& mask)
            {
                const bool drivewayGates = readSensor(mask.triggerOnDrivewayGates, config->DRIVEWAY_GATES, config->ACTIVE_LEVEL_DRIVEWAY_GATES),
                           yardGate = readSensor(mask.triggerOnYardGate, config->YARD_GATE, config->ACTIVE_LEVEL_YARD_GATE),
                           frontDoor = readSensor(mask.triggerOnFrontDoor, config->FRONT_DOOR, config->ACTIVE_LEVEL_FRONT_DOOR);

                return drivewayGates || yardGate || frontDoor;
            }
};
