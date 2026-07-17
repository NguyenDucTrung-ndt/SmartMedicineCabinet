#ifndef RFID_MANAGER_H
#define RFID_MANAGER_H

#include <Arduino.h>
#include <MFRC522.h>
#include "Config.h"

class RFIDManager {
public:
    explicit RFIDManager(MFRC522& rfidRef);
    void begin();
    void update();                 
    bool isCardDetected() const;
    bool isAuthorized() const;
    const uint8_t* getUID() const;
    void clearCardState();

private:
    MFRC522& rfid;
    bool cardDetected;
    bool authorized;
    uint8_t uid[10];
    uint8_t uidSize;
};

#endif