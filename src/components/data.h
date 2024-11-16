#pragma once

#include <Arduino.h>


struct LightsData {
    bool triggerOnDrivewayGates = false;
    bool triggerOnYardGate = false;
    bool triggerOnFrontDoor = false;
    uint16_t offDelay = 5;
};
