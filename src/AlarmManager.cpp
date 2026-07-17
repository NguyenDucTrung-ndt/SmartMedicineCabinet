// ==========================================
// --- FILE 4/15: src/AlarmManager.cpp ------
// ==========================================
#include "AlarmManager.h"

AlarmManager::AlarmManager(uint8_t buzzerPinValue)
    : buzzerPin(buzzerPinValue), toneMode(ToneMode::None), patternStartMs(0), toneOn(false) {}

void AlarmManager::begin() {
    pinMode(buzzerPin, OUTPUT);
    noTone(buzzerPin);
    toneMode = ToneMode::None;
}

void AlarmManager::success() {
    toneMode = ToneMode::Success;
    patternStartMs = millis();
    toneOn = false;
}

void AlarmManager::error() {
    toneMode = ToneMode::Error;
    patternStartMs = millis();
    toneOn = false;
}

void AlarmManager::warning() {
    toneMode = ToneMode::Warning;
    patternStartMs = millis();
    toneOn = false;
}

void AlarmManager::alarm() {
    toneMode = ToneMode::Alarm;
    patternStartMs = millis();
    toneOn = false;
}

void AlarmManager::stop() {
    noTone(buzzerPin);
    toneOn = false;
    toneMode = ToneMode::None;
}

void AlarmManager::update() {
    unsigned long now = millis();
    switch (toneMode) {
        case ToneMode::Success:
            if (now - patternStartMs < 80) {
                if (!toneOn) { tone(buzzerPin, 1800); toneOn = true; }
            } else if (now - patternStartMs < 180) {
                if (toneOn) { noTone(buzzerPin); toneOn = false; }
            } else if (now - patternStartMs < 260) {
                if (!toneOn) { tone(buzzerPin, 2200); toneOn = true; }
            } else if (now - patternStartMs < 360) {
                if (toneOn) { noTone(buzzerPin); toneOn = false; }
            } else {
                stop();
            }
            break;
            
        case ToneMode::Error:
            if (now - patternStartMs < 60) {
                if (!toneOn) { tone(buzzerPin, 1500); toneOn = true; }
            } else if (now - patternStartMs < 140) {
                if (toneOn) { noTone(buzzerPin); toneOn = false; }
            } else if (now - patternStartMs < 220) {
                if (!toneOn) { tone(buzzerPin, 1500); toneOn = true; }
            } else if (now - patternStartMs < 300) {
                if (toneOn) { noTone(buzzerPin); toneOn = false; }
            } else {
                stop();
            }
            break;
            
        case ToneMode::Warning:
            if ((now - patternStartMs) % 600 < 120) {
                if (!toneOn) { tone(buzzerPin, 1200); toneOn = true; }
            } else {
                if (toneOn) { noTone(buzzerPin); toneOn = false; }
            }
            break;
            
        case ToneMode::Alarm:
            if (!toneOn) { tone(buzzerPin, 1000); toneOn = true; }
            break;
            
        case ToneMode::None:
        default:
            noTone(buzzerPin);
            toneOn = false;
            break;
    }
}