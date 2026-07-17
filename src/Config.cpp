#include "Config.h"

const char PASSWORD[] = "1234";

// Cấu hình các thẻ RFID hợp lệ mới (Thẻ 1: 01:02:03:04, Thẻ 2: 11:22:33:44)
const RFIDCard VALID_CARDS[] = {
    {4, {0x01, 0x02, 0x03, 0x04}}, // Thẻ 1: 01:02:03:04
    {4, {0x11, 0x22, 0x33, 0x44}}  // Thẻ 2: 11:22:33:44
};

const uint8_t VALID_CARD_COUNT = sizeof(VALID_CARDS) / sizeof(VALID_CARDS[0]);