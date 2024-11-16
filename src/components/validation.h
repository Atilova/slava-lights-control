#pragma once

#include <ESPAsyncWebServer.h>


bool validateQuery(AsyncWebServerRequest *request, const char* key, bool& value) {
    if (request->hasParam(key))
        {
            const String innerValue = request->getParam(key)->value();
            if (innerValue == "true")
                {
                    value = true;
                    return true;
                }

            if (innerValue == "false")
                {
                    value = false;
                    return true;
                }
        }

    return false;
}


bool validateQuery(AsyncWebServerRequest *request, const char* key, uint16_t & value) {
    if (request->hasParam(key))
        {
            const String innerValue = request->getParam(key)->value();
            long longValue = innerValue.toInt();
            if (longValue >= 0 && longValue <= UINT16_MAX) {
                value = static_cast<uint16_t>(longValue);
                return true;
            }
        }

    return false;
}


bool validateQuery(AsyncWebServerRequest *request, const char* key, String& value) {
    if (request->hasParam(key))
        {
            value = request->getParam(key)->value();
            return true;
        }

    return false;
}
