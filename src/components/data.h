#pragma once

#include <Arduino.h>


struct __attribute__ ((packed)) LightsData {
    bool triggerOnDrivewayGates = false;
    bool triggerOnYardGate = false;
    bool triggerOnFrontDoor = false;
    uint16_t offDelay = 5;
};
