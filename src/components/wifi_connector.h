#pragma once

#include <Arduino.h>
#include <IPAddress.h>
#include <ESP8266WiFi.h>

#include "millis_timer.h"


struct WifiConfig {
    const IPAddress ESP_IP;
    const IPAddress GATEWAY;
    const IPAddress SUBNET_MASK;
    const IPAddress PRIMARY_DNS;
    const IPAddress SECONDARY_DNS;
    const char* SSID;
    const char* PASSWORD;
    const uint8_t INDICATION_PIN = LED_BUILTIN;  // Cветодиод esp8266
};


enum class WifiState {
    CONNECTED,
    CONNECTING,
    RESETTING
};


class WifiConnector {
    WifiConfig *config = nullptr;
    MillisTimer timer = MillisTimer(5000);
    WifiState state = WifiState::CONNECTING;
    uint8_t retries = 0;

    void reset()
        {
            WiFi.disconnect();  // Обрываем WIFI соединения
            WiFi.softAPdisconnect();  // Отключаем точку доступа, т.е режим роутера
            WiFi.mode(WIFI_OFF);  // Отключаем WIFI
        }

    public:
        WifiConnector(WifiConfig &config) : config(&config) {}

        ~WifiConnector() {}

        void setup()
            {
                pinMode(config->INDICATION_PIN, OUTPUT);
                digitalWrite(config->INDICATION_PIN, HIGH);
            }

        void maintain()
            {
                switch (state)
                    {
                        case WifiState::CONNECTED:
                            {
                                if (!isConnected())
                                    {
                                        Serial.println("Initiate WiFi!");
                                        digitalWrite(config->INDICATION_PIN, HIGH);

                                        state = WifiState::CONNECTING;
                                        timer.setInterval(6000);
                                        timer.flush();
                                    }

                                break;
                            }
                        case WifiState::CONNECTING:
                            {
                                if (isConnected())
                                    {
                                        Serial.println("WiFi connected!");
                                        digitalWrite(config->INDICATION_PIN, LOW);

                                        state = WifiState::CONNECTED;
                                        retries = 0;
                                    }
                                else if (timer.isReady())
                                    {
                                        Serial.println("WiFi connecting...");
                                        retries += 1;

                                        WiFi.mode(WIFI_STA);
                                        WiFi.config(
                                            config->ESP_IP,
                                            config->GATEWAY,
                                            config->SUBNET_MASK
                                        );
                                        WiFi.begin(config->SSID, config->PASSWORD);

                                        if (retries > 3)
                                            {
                                                digitalWrite(config->INDICATION_PIN , HIGH);

                                                retries = 0;
                                                state = WifiState::RESETTING;
                                                reset();
                                                timer.setInterval(1000);
                                            }

                                        timer.refresh();
                                    }

                                break;
                            }
                        case WifiState::RESETTING:
                            {
                                if (timer.isReady())
                                    {
                                        state = WifiState::CONNECTING;
                                        timer.setInterval(6000);
                                        timer.flush();
                                    }

                                break;
                            }
                    }
            }

        bool isConnected()
            {
                return WiFi.isConnected();
            }
};
