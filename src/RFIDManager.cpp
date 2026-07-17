#include "RFIDManager.h"
#include <SPI.h>

RFIDManager::RFIDManager(MFRC522& rfidRef)
    : rfid(rfidRef), cardDetected(false), authorized(false), uidSize(0) {
    memset(uid, 0, sizeof(uid));
}

void RFIDManager::begin() {
    SPI.begin();
    rfid.PCD_Init();
    clearCardState();
}

void RFIDManager::update() {
    cardDetected = false;
    authorized = false;

    if (!rfid.PICC_IsNewCardPresent()) { return; }
    if (!rfid.PICC_ReadCardSerial()) { return; }

    cardDetected = true;
    uidSize = rfid.uid.size;
    memcpy(uid, rfid.uid.uidByte, uidSize);

    

    // Đối chiếu thông minh khớp cả chiều dài thẻ
    for (uint8_t i = 0; i < VALID_CARD_COUNT; ++i) {
        if (uidSize == VALID_CARDS[i].size) {
            bool matched = true;
            for (uint8_t j = 0; j < uidSize; ++j) {
                if (uid[j] != VALID_CARDS[i].uid[j]) {
                    matched = false;
                    break;
                }
            }
            if (matched) {
                authorized = true;
                break;
            }
        }
    }
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
}

bool RFIDManager::isCardDetected() const { return cardDetected; }
bool RFIDManager::isAuthorized() const { return authorized; }
const uint8_t* RFIDManager::getUID() const { return uid; }
void RFIDManager::clearCardState() {
    cardDetected = false;
    authorized = false;
    uidSize = 0;
    memset(uid, 0, sizeof(uid));
}