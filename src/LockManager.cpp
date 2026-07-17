#include "LockManager.h"
#include "Config.h"

LockManager::LockManager(Servo& servoRef)
    : servo(servoRef), open(false), openStartedAtMs(0) {}

void LockManager::begin() {
    servo.attach(SERVO_PIN);
    closeDoor();
}

void LockManager::openDoor() {
    if (!servo.attached()) {
        servo.attach(SERVO_PIN);
    }
    open = true;
    openStartedAtMs = millis();
    servo.write(90);
}

void LockManager::closeDoor() {
    if (!servo.attached()) {
        servo.attach(SERVO_PIN);
    }
    open = false;
    servo.write(0);
    delay(150); 
    servo.detach(); 
}

void LockManager::update() {
    if (open && (millis() - openStartedAtMs >= DOOR_OPEN_DURATION_MS)) {
        closeDoor();
    }
}

bool LockManager::isOpen() const { return open; }