// ==========================================
// --- FILE 12/15: src/PasswordManager.cpp --
// ==========================================
#include "PasswordManager.h"

PasswordManager::PasswordManager(Keypad& keypadRef)
    : keypad(keypadRef), inputLength(0), correct(false), locked(false),
      lockUntilMs(0), wrongAttempts(0), lastAttemptWrong(false) {
    input[0] = '\0';
}

void PasswordManager::begin() { reset(); }

char PasswordManager::update() {
    if (locked) {
        if (millis() >= lockUntilMs) {
            locked = false;
            wrongAttempts = 0;
            lastAttemptWrong = false;
            clear();
        } else {
            return '\0';
        }
    }

    char key = keypad.getKey();
    if (!key) { return '\0'; }

    if (key == '*') {
        clear();
        return key; 
    }

    if (key == '#') {
        if (inputLength > 0) {
            correct = (strncmp(input, PASSWORD, PASSWORD_LENGTH) == 0);
            if (correct) {
                lastAttemptWrong = false;
                wrongAttempts = 0;
                inputLength = 0;
                input[0] = '\0';
            } else {
                lastAttemptWrong = true;
                wrongAttempts++;
                inputLength = 0;
                input[0] = '\0';
                if (wrongAttempts >= MAX_WRONG_ATTEMPTS) {
                    locked = true;
                    lockUntilMs = millis() + LOCK_DURATION_MS;
                }
            }
        }
        return key; 
    }

    if (isdigit(key) && inputLength < PASSWORD_LENGTH) {
        input[inputLength++] = key;
        input[inputLength] = '\0';
        correct = false;
        lastAttemptWrong = false;
        return key; 
    }
    
    return key;
}

void PasswordManager::reset() {
    clear();
    correct = false;
    locked = false;
    lastAttemptWrong = false;
    wrongAttempts = 0;
    lockUntilMs = 0;
}

bool PasswordManager::isCorrect() const { return correct; }
const char* PasswordManager::getInput() const { return input; }
void PasswordManager::clear() {
    inputLength = 0;
    input[0] = '\0';
    correct = false;
    lastAttemptWrong = false;
}
bool PasswordManager::hasInput() const { return inputLength > 0; }
bool PasswordManager::isLocked() const { return locked; }

unsigned long PasswordManager::getLockRemainingMs() const {
    if (!locked) { return 0; }
    unsigned long now = millis();
    return (lockUntilMs > now) ? (lockUntilMs - now) : 0;
}

bool PasswordManager::wasLastAttemptWrong() const { return lastAttemptWrong; }
void PasswordManager::resetWrongAttemptState() { lastAttemptWrong = false; }