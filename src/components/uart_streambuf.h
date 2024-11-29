#pragma once

#include <Arduino.h>
#include <streambuf>


class UARTStreamBuf : public std::streambuf {
    protected:
        int overflow(int c) override
            {
                if (c != EOF)
                    {
                        Serial.write(c);
                    }
                return c;
            }
};
