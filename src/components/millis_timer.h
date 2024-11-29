#pragma once

#include <Arduino.h>


class MillisTimer {
    uint32_t interval;
    uint32_t startedAt;

    public:
        MillisTimer(uint32_t interval)
            : interval(interval),
              startedAt(0) {}

        void setInterval(uint32_t newInterval)  // Устанавливаем новое значение интервала
            {
                interval = newInterval;
            }

        void refresh()  // Просто сдвигаем  интервал при наличии активных сигналов дверей и ворот
            {
                startedAt = millis();
            }

        void flush()
            {
                startedAt = 0;
            }

        uint32_t remains() const
            {
                const int remined =
                    startedAt > 0
                        ? interval - static_cast<uint32_t>(millis() - startedAt)
                        : 0;

                if (remined < 0)
                    {
                        return 0;
                    }

                return remined;
            }

        bool isReady() const
            {
                if (!startedAt)
                    {
                        return true;
                    }

                return static_cast<uint32_t>(millis() - startedAt) >= interval;
            }
};
