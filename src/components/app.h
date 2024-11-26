#pragma once

#include <Arduino.h>
#include <iostream>
#include <Esp.h>
#if defined(ESP32)
    #include <SPIFFS.h>
#elif defined(ESP8266)
    #include <FS.h>
#else
  #error "This code is for ESP32 or ESP8266 only."
#endif
#include <EEPROM.h>

#include <ESPAsyncWebServer.h>

#include "components/constants.h"
#include "components/data.h"
#include "components/eeprom_manager.h"
#include "components/millis_timer.h"
#include "components/trigger_sensors_manager.h"
#include "components/utils.h"
#include "components/validation.h"
#include "components/webserver_supervisor.h"
#include "components/wifi_connector.h"


struct AppConfig {
    const uint8_t ESP_RELAY_PIN;  // Вывод выхода сигнала на реле вкл/выкл освещения
    const bool RELAY_LEVEL_ON = HIGH;
    const uint16_t WEB_SERVER_PORT = 80;
};


bool isAnyEnabledTriggers(LightsData &data)
    {
        return data.triggerOnDrivewayGates || data.triggerOnYardGate || data.triggerOnFrontDoor;
    }


bool serializeLightsData(AsyncWebServerRequest *request, LightsData &data)
    {
        bool triggerOnDrivewayGates, triggerOnYardGate, triggerOnFrontDoor;
        uint16_t offDelay;

        const bool isValid = (
            validateQuery(request, "triggerOnDrivewayGates", triggerOnDrivewayGates) &&
            validateQuery(request, "triggerOnYardGate", triggerOnYardGate) &&
            validateQuery(request, "triggerOnFrontDoor", triggerOnFrontDoor) &&
            validateQuery(request, "offDelay", offDelay) &&
            offDelay >= 1 && offDelay <= 1200  // Значения задержки храним и валиируем в секундах, а отображаем в минутах
        );

        if (!isValid)
            {
                return false;
            }

        data.triggerOnDrivewayGates = triggerOnDrivewayGates;
        data.triggerOnYardGate = triggerOnYardGate;
        data.triggerOnFrontDoor = triggerOnFrontDoor;
        data.offDelay = offDelay;

        return true;
    }


class App {
    private:
		LightsData lightsData;  // Данные из eeprom
		AsyncWebServer *webServer;
        WebServerSupervisor *webServerSupervisor = nullptr;

        WifiConnector *wifiConnector = nullptr;
        EEPromManager<LightsData> *eepromManager = nullptr;
        TriggerSensorsManager *triggerSensorsManager = nullptr;
        AppConfig *config = nullptr;

        MillisTimer readSensorsTimer = MillisTimer(500);  // Время опрса дверей
        MillisTimer offDelayTimer = MillisTimer(10000);  // Сколько горит свет
        bool isLightsActive = false;  // Вкл или выкл реле

    void main()
        {
            if (readSensorsTimer.isReady())
                {
                    const bool isAnyActiveSensor = triggerSensorsManager->isAnyActiveFromMask(lightsData);
                    if (isAnyActiveSensor)
                        {
                            if (!isLightsActive)
                                {
                                    isLightsActive = true;
                                    digitalWrite(config->ESP_RELAY_PIN, config->RELAY_LEVEL_ON);
                                }

                            offDelayTimer.refresh();
                        }

                    readSensorsTimer.refresh();
                }

            if (isLightsActive && offDelayTimer.isReady())
                {
                    digitalWrite(config->ESP_RELAY_PIN, !config->RELAY_LEVEL_ON);
                    isLightsActive = false;
                }
        }

	void setupWebServer()
		{
			DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

			webServer->onNotFound([](AsyncWebServerRequest *request) {
				request->send(404, "text/html", "<script>location.replace(\"/\");</script>");
			});

			webServer->on("/", HTTP_GET, [](AsyncWebServerRequest *request){
				request->send(SPIFFS, "/index.html", "text/html; charset=utf-8", false);
			});

			webServer->on(
                "/api/getEspFreeHeap/",
                HTTP_GET,
                [](AsyncWebServerRequest *request) {
                    char jsonBuffer[100];
                    sprintf(
                        jsonBuffer,
                        apiGetEspFreeHeapJsonTemplate,
                        ESP.getFreeHeap()
                    );

                    request->send(200, "application/json", jsonBuffer);
                }
            );

            webServer->on(
                "/api/getLightStatus/",  // Горит не горит лампочка
                HTTP_GET,
                [this](AsyncWebServerRequest *request) {
                    char jsonBuffer[50];
                    sprintf(
                        jsonBuffer,
                        apiGetLightStatusJsonTemplate,
                        toJsonBool(this->isLightsActive)
                    );

                    request->send(200, "application/json", jsonBuffer);
                }
            );

            webServer->on(
                "/api/getPreference/",
                HTTP_GET,
                [this](AsyncWebServerRequest *request) {
                    char jsonBuffer[300];
                    sprintf(
                        jsonBuffer,
                        apiGetPreferenceJsonTemplate,
                        toJsonBool(this->lightsData.triggerOnDrivewayGates),
                        toJsonBool(this->lightsData.triggerOnYardGate),
                        toJsonBool(this->lightsData.triggerOnFrontDoor),
                        this->lightsData.offDelay
                    );

                    request->send(200, "application/json", jsonBuffer);
                }
            );

			webServer->on(
                "/api/savePreference/",  // Настройки пользователя принимаем сюда после нажатия кнопки save, валидируем и сохраняем в епром
                HTTP_POST,
                [this](AsyncWebServerRequest *request) {
                    const bool isSerialized = serializeLightsData(request, this->lightsData),
                               isSaved = isSerialized && this->eepromManager->save(this->lightsData);

                    if (isSaved)
                        {
                            this->offDelayTimer.setInterval(this->lightsData.offDelay * 1000);
                            this->offDelayTimer.refresh();

                            if (!isAnyEnabledTriggers(this->lightsData))
                                {
                                    this->offDelayTimer.flush();
                                }
                        }

                    char jsonBuffer[50];
                    sprintf(
                        jsonBuffer,
                        apiSavePreferenceJsonTemplate,
                        toJsonBool(isSaved)
                    );

                    request->send(200, "application/json", jsonBuffer);
                }
            );

			webServer->serveStatic("/static/", SPIFFS, "/static/");
		}

    public:
        App(
            WifiConnector &wifiConnector,
            EEPromManager<LightsData> &eepromManager,
            TriggerSensorsManager &triggerSensorsManager,
            AppConfig &config
        ) :
            wifiConnector(&wifiConnector),
            eepromManager(&eepromManager),
            triggerSensorsManager(&triggerSensorsManager),
            config(&config)
        {
            webServer = new AsyncWebServer(this->config->WEB_SERVER_PORT);
            webServerSupervisor = new WebServerSupervisor(*webServer);
        }

        ~App() {
            webServerSupervisor->forceEnd();

            delete webServerSupervisor;
			delete webServer;
		}

        void setup()
            {
                pinMode(config->ESP_RELAY_PIN, OUTPUT);
                digitalWrite(config->ESP_RELAY_PIN, !config->RELAY_LEVEL_ON);

                wifiConnector->setup();
                eepromManager->setup();
                triggerSensorsManager->setup();

                if (eepromManager->load(lightsData))
                    {
                        std::cout << "Successfully loaded lightsData from EEProm" << std::endl;
                        offDelayTimer.setInterval(lightsData.offDelay * 1000);
                    }

				SPIFFS.begin();
				setupWebServer();
            }

        void run()
            {
                wifiConnector->maintain();
                webServerSupervisor->maintain();
                main();
            }

        void onWifiStatusChange(bool isConnected)
            {
                webServerSupervisor->onNetworkConnectionChange(isConnected);
            }
};
