#include "RTCManager.h"

RTCManager::RTCManager(RTC_DS1307& rtcRef)
    : rtc(rtcRef), lastUpdateMs(0) {
    formattedBuffer[0] = '\0';
}

void RTCManager::begin() {
    if (rtc.begin()) {
        if (!rtc.isrunning()) {
            rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        }
    }
}

void RTCManager::update() {
    unsigned long now = millis();
    // Tần suất cập nhật RTC mỗi 1 giây (1000ms) để giữ hiệu suất vi điều khiển
    if (formattedBuffer[0] != '\0' && (now - lastUpdateMs < 1000UL)) {
        return;
    }
    lastUpdateMs = now;

    DateTime dt = rtc.now();
    snprintf(formattedBuffer, sizeof(formattedBuffer), "%02d:%02d:%02d  %02d/%02d/%02d", 
             dt.hour(), dt.minute(), dt.second(), 
             dt.day(), dt.month(), dt.year() % 100);
}

const char* RTCManager::getFormattedDateTime() {
    return formattedBuffer;
}