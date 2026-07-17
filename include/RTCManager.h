// ==========================================
// --- FILE 15/17: include/RTCManager.h -----
// ==========================================
#ifndef RTC_MANAGER_H
#define RTC_MANAGER_H

#include <Arduino.h>
#include <RTClib.h>

class RTCManager {
public:
    explicit RTCManager(RTC_DS1307& rtcRef);
    void begin();
    void update();
    const char* getFormattedDateTime();

private:
    RTC_DS1307& rtc;
    char formattedBuffer[21];
    unsigned long lastUpdateMs;
};

#endif