// ==========================================
// --- FILE 3/15: include/AlarmManager.h ----
// ==========================================
#ifndef ALARM_MANAGER_H
#define ALARM_MANAGER_H

#include <Arduino.h>

class AlarmManager {
public:
    explicit AlarmManager(uint8_t buzzerPin);
    void begin();
    void success(); 
    void error();   
    void warning(); 
    void alarm();   
    void stop();    
    void update();  

private:
    enum class ToneMode { None, Success, Error, Warning, Alarm };
    uint8_t buzzerPin;
    ToneMode toneMode;
    unsigned long patternStartMs;
    bool toneOn;
};

#endif