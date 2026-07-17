#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class Display {
public:
    explicit Display(LiquidCrystal_I2C& lcdRef);
    void begin();
    void showStartup();
    void showHome();
    void showWarning();
    void showPasswordEntry();
    void showPasswordAccepted();
    void showPasswordDenied();
    void showRfidAccepted();
    void showRfidDenied();
    void showLockCountdown();
    void update();
    void forceRefreshNextUpdate(); 

    void setDateTime(const char* dateTimeValue); // <-- PHẢI CÓ DÒNG KHAI BÁO NÀY
    void setTemperature(float temperatureValue);
    void setHumidity(float humidityValue);
    void setDoorOpen(bool isOpen);
    void setLocked(bool isLocked);
    void setPasswordInput(const char* inputValue);
    void setCountdown(unsigned long seconds);

private:
    enum class ScreenMode {
        Startup, Home, Warning, PasswordEntry, PasswordAccepted,
        PasswordDenied, RfidAccepted, RfidDenied, LockCountdown
    };

    LiquidCrystal_I2C& lcd;
    float temperature;
    float humidity;
    bool doorOpen;
    bool locked;
    unsigned long countdownSeconds;
    char passwordInput[21];
    char dateTime[21];
    ScreenMode screenMode;
    ScreenMode lastScreenMode;
    bool forceRefresh;
};

#endif