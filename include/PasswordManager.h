#ifndef PASSWORD_MANAGER_H
#define PASSWORD_MANAGER_H

#include <Arduino.h>
#include <Keypad.h>
#include "Config.h"

class PasswordManager {
public:
    explicit PasswordManager(Keypad& keypadRef);
    void begin();
    char update();                       
    void reset();                        
    bool isCorrect() const;              
    const char* getInput() const;        
    void clear();                        
    bool hasInput() const;               
    bool isLocked() const;               
    unsigned long getLockRemainingMs() const;
    bool wasLastAttemptWrong() const;
    void resetWrongAttemptState();

private:
    Keypad& keypad;
    char input[PASSWORD_LENGTH + 1];
    uint8_t inputLength;
    bool correct;
    bool locked;
    unsigned long lockUntilMs;
    uint8_t wrongAttempts;
    bool lastAttemptWrong;
};

#endif