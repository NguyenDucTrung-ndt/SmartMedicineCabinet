// ==========================================
// --- FILE 17/17: src/main.cpp -------------
// ==========================================
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>
#include "Config.h"
#include "AlarmManager.h"
#include "DHTManager.h"
#include "Display.h"
#include "LockManager.h"
#include "PasswordManager.h"
#include "RFIDManager.h"
#include "RTCManager.h"

// --- Global instances ---
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);
Display display(lcd);

RTC_DS1307 rtc;
RTCManager rtcManager(rtc);

MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN);
RFIDManager rfidManager(rfid);

char keyMap[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
byte rowPins[] = {KEYPAD_ROW_PIN_1, KEYPAD_ROW_PIN_2, KEYPAD_ROW_PIN_3, KEYPAD_ROW_PIN_4};
byte colPins[] = {KEYPAD_COL_PIN_1, KEYPAD_COL_PIN_2, KEYPAD_COL_PIN_3, KEYPAD_COL_PIN_4};
Keypad keypad(makeKeymap(keyMap), rowPins, colPins, 4, 4);
PasswordManager passwordManager(keypad);

DHT dht(DHT_PIN, DHT22);
DHTManager dhtManager(dht);

AlarmManager alarmManager(BUZZER_PIN);

Servo servo;
LockManager lockManager(servo);

// --- Program States ---
bool isStartupActive = true;       
unsigned long startupTimeMs = 0;   
bool lastWarningState = false;      

enum class TemporaryMessage {
    None, PasswordAccepted, PasswordDenied, RfidAccepted, RfidDenied
};

TemporaryMessage activeMessage = TemporaryMessage::None;
unsigned long messageUntilMs = 0;

void showTemporaryMessage(TemporaryMessage message, unsigned long durationMs) {
    activeMessage = message;
    messageUntilMs = millis() + durationMs;
}

void setup() {
    // CHÚ Ý QUAN TRỌNG: Không dùng Serial.begin() để tránh liệt bàn phím.
    
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_RED_PIN, OUTPUT);
    digitalWrite(LED_GREEN_PIN, HIGH);
    digitalWrite(LED_RED_PIN, LOW);

    display.begin();
    rtcManager.begin();
    rfidManager.begin();
    passwordManager.begin();
    dhtManager.begin();
    alarmManager.begin();
    lockManager.begin();

    startupTimeMs = millis();
    isStartupActive = true;
    display.showStartup();
    display.update();
}

void loop() {
    if (isStartupActive) {
        if (millis() - startupTimeMs >= 2000UL) {
            isStartupActive = false;
            display.showHome();
        } else {
            display.update();
            return; 
        }
    }

    dhtManager.update();
    lockManager.update();
    alarmManager.update();
    rtcManager.update();

    char key = passwordManager.update();
    if (key) {
        display.forceRefreshNextUpdate();
    }

    // Lấy chuỗi thời gian đã định dạng từ RTCManager đưa sang Display
    display.setDateTime(rtcManager.getFormattedDateTime());

    float currentTemp = dhtManager.getTemperature();
    float currentHumi = dhtManager.getHumidity();
    bool warningActive = dhtManager.isTemperatureOutOfRange() || dhtManager.isHumidityOutOfRange();
    bool systemLocked = passwordManager.isLocked();

    if (!systemLocked && !lockManager.isOpen()) {
        rfidManager.update();
        if (rfidManager.isCardDetected()) {
            if (rfidManager.isAuthorized()) {
                lockManager.openDoor();
                showTemporaryMessage(TemporaryMessage::RfidAccepted, 2000UL);
                alarmManager.success();
            } else {
                showTemporaryMessage(TemporaryMessage::RfidDenied, 2000UL);
                alarmManager.error();
            }
            rfidManager.clearCardState(); 
        }
    } else {
        rfidManager.clearCardState();
    }

    if (!systemLocked && !lockManager.isOpen()) {
        if (passwordManager.isCorrect()) {
            lockManager.openDoor();
            showTemporaryMessage(TemporaryMessage::PasswordAccepted, 2000UL);
            alarmManager.success();
            passwordManager.reset();
        } else if (passwordManager.wasLastAttemptWrong()) {
            showTemporaryMessage(TemporaryMessage::PasswordDenied, 2000UL);
            alarmManager.error();
            passwordManager.resetWrongAttemptState(); 
        }
    }

    display.setTemperature(currentTemp);
    display.setHumidity(currentHumi);
    display.setDoorOpen(lockManager.isOpen());
    display.setLocked(systemLocked);
    display.setPasswordInput(passwordManager.getInput());
    display.setCountdown(passwordManager.getLockRemainingMs() / 1000UL);

    bool showMessageActive = (activeMessage != TemporaryMessage::None) && (millis() < messageUntilMs);

    if (showMessageActive) {
        switch (activeMessage) {
            case TemporaryMessage::PasswordAccepted:
                display.showPasswordAccepted();
                digitalWrite(LED_GREEN_PIN, HIGH);
                digitalWrite(LED_RED_PIN, LOW);
                break;
            case TemporaryMessage::PasswordDenied:
                display.showPasswordDenied();
                digitalWrite(LED_GREEN_PIN, LOW);
                digitalWrite(LED_RED_PIN, HIGH);
                break;
            case TemporaryMessage::RfidAccepted:
                display.showRfidAccepted();
                digitalWrite(LED_GREEN_PIN, HIGH);
                digitalWrite(LED_RED_PIN, LOW);
                break;
            case TemporaryMessage::RfidDenied:
                display.showRfidDenied();
                digitalWrite(LED_GREEN_PIN, LOW);
                digitalWrite(LED_RED_PIN, HIGH);
                break;
            default:
                break;
        }
    } else {
        activeMessage = TemporaryMessage::None;

        if (systemLocked) {
            display.showLockCountdown();
            digitalWrite(LED_GREEN_PIN, LOW);
            digitalWrite(LED_RED_PIN, HIGH);
            alarmManager.warning();
            lastWarningState = true;
        } else if (warningActive) {
            display.showWarning();
            digitalWrite(LED_GREEN_PIN, LOW);
            digitalWrite(LED_RED_PIN, HIGH);
            alarmManager.alarm();
            lastWarningState = true;
        } else {
            if (passwordManager.hasInput()) {
                display.showPasswordEntry();
            } else {
                display.showHome();
            }
            
            digitalWrite(LED_GREEN_PIN, HIGH);
            digitalWrite(LED_RED_PIN, LOW);

            if (lastWarningState) {
                alarmManager.stop();
                lastWarningState = false;
            }
        }
    }

    display.update();
}