#pragma once

#include <Arduino.h>
#include <iostream>

#include <ESPAsyncWebServer.h>

#include "millis_timer.h"
#include "utils.h"


enum class WebServerSupervisorState
    {
        RUNNING,
        ABORTING,
        STOPPED
    };


class WebServerSupervisor {
    AsyncWebServer &webServer;
    WebServerSupervisorState state = WebServerSupervisorState::STOPPED;
    MillisTimer timer = MillisTimer(5000);
    bool isNetworkConnected = false;

    public:
        WebServerSupervisor(AsyncWebServer &webServer)
            : webServer(webServer) {}

        ~WebServerSupervisor() {}

        void maintain()
            {
                switch (state)
                    {
                        case WebServerSupervisorState::RUNNING:
                            {
                                if (!isNetworkConnected)
                                    {
                                        state = WebServerSupervisorState::ABORTING;
                                        webServer.end();
                                        timer.refresh();

                                        std::cout << "WebServer stopping..." << std::endl;
                                    }

                                break;
                            }
                        case WebServerSupervisorState::ABORTING:
                            {
                                if (timer.isReady())
                                    {
                                        state = WebServerSupervisorState::STOPPED;
                                        tcpCleanup();

                                        std::cout << "WebServer stopped!" << std::endl;
                                    }

                                break;
                            }
                        case WebServerSupervisorState::STOPPED:
                            {
                                if (isNetworkConnected)
                                    {
                                        state = WebServerSupervisorState::RUNNING;
                                        webServer.begin();

                                        std::cout << "WebServer started!" << std::endl;
                                    }

                                break;
                            }
                    }
            }

        void forceEnd()
            {
                webServer.end();
                tcpCleanup();
            }

        void onNetworkConnectionChange(bool isConnected)
            {
                isNetworkConnected = isConnected;
            }
};
