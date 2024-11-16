#pragma once

#include <Arduino.h>


template <typename T>
class EEPromManager {
    const uint16_t MAGIC_NUMBER = 0xdead;

    size_t _dataSize;
    const uint16_t _startAddress;

    public:
        EEPromManager(uint16_t startAddress = 0) : _startAddress(startAddress)
            {
                _dataSize = sizeof(T);  // Calculate the size of the struct T
            }

        ~EEPromManager()
            {
                EEPROM.end();
            }

        void setup()
            {
                EEPROM.begin(_dataSize);
            }

        bool save(const T& data)
            {
                EEPROM.put(_startAddress, MAGIC_NUMBER);
                EEPROM.put(_startAddress + sizeof(MAGIC_NUMBER), data);
                EEPROM.commit();

                return true;
            }

        bool load(T& data)
            {
                uint16_t magic;
                EEPROM.get(_startAddress, magic);

                if (magic != MAGIC_NUMBER)
                    {
                        return false;
                    }

                EEPROM.get(_startAddress + sizeof(MAGIC_NUMBER), data);
                return true;
            }

        size_t getDataSize() const
            {
                return _dataSize;
            }
};
