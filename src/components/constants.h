#pragma once

#include <Arduino.h>


constexpr const char* apiGetEspFreeHeapJsonTemplate = R"({
    "freeHeap": %d
})";

constexpr const char* apiGetLightStatusJsonTemplate = R"({
    "enabled": %s,
    "remained": %d
})";

constexpr const char* apiGetPreferenceJsonTemplate = R"({
    "triggerOnDrivewayGates": %s,
    "triggerOnYardGate": %s,
    "triggerOnFrontDoor": %s,
    "offDelay": %d
})";

constexpr const char* apiSavePreferenceJsonTemplate = R"({
    "saved": %s
})";
