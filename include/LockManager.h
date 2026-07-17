// ==========================================
// --- FILE 9/15: include/LockManager.h -----
// ==========================================
#ifndef LOCK_MANAGER_H
#define LOCK_MANAGER_H

#include <Arduino.h>
#include <Servo.h>

class LockManager {
public:
    explicit LockManager(Servo& servoRef);
    void begin();
    void openDoor();  
    void closeDoor(); 
    void update();    
    bool isOpen() const;

private:
    Servo& servo;
    bool open;
    unsigned long openStartedAtMs;
};

#endif