#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// --- LCD Configuration ---
constexpr uint8_t LCD_ADDRESS = 0x27;   
constexpr uint8_t LCD_COLUMNS = 20;     
constexpr uint8_t LCD_ROWS = 4;         

// --- Pin configurations ---
constexpr uint8_t DHT_PIN = 2;          
constexpr uint8_t SERVO_PIN = 3;        
constexpr uint8_t BUZZER_PIN = 4;       
constexpr uint8_t LED_GREEN_PIN = 5;    
constexpr uint8_t LED_RED_PIN = 6;      

// --- Keypad pin configurations ---
constexpr uint8_t KEYPAD_ROW_PIN_1 = A0; 
constexpr uint8_t KEYPAD_ROW_PIN_2 = A1; 
constexpr uint8_t KEYPAD_ROW_PIN_3 = A2; 
constexpr uint8_t KEYPAD_ROW_PIN_4 = A3; 
constexpr uint8_t KEYPAD_COL_PIN_1 = 7;  
constexpr uint8_t KEYPAD_COL_PIN_2 = 8;  
constexpr uint8_t KEYPAD_COL_PIN_3 = 1;  
constexpr uint8_t KEYPAD_COL_PIN_4 = 0;  

// --- RFID configuration ---
constexpr uint8_t RFID_SS_PIN = 10;     
constexpr uint8_t RFID_RST_PIN = 9;     

// --- Security and Timing thresholds ---
constexpr uint8_t PASSWORD_LENGTH = 4;   
constexpr uint8_t MAX_WRONG_ATTEMPTS = 3; 
constexpr unsigned long DOOR_OPEN_DURATION_MS = 5000UL; //5 giay 
constexpr unsigned long LOCK_DURATION_MS = 30000UL;     //30 giay 
constexpr unsigned long DHT_SAMPLE_INTERVAL_MS = 2000UL; //2 giay
// --- Đã sửa: Cấu hình giới hạn nhiệt độ 15-30 độ C và độ ẩm tối đa 75% ---
constexpr float TEMPERATURE_MIN = 15.0f;          
constexpr float TEMPERATURE_MAX = 50.0f;          
constexpr float HUMIDITY_MAX = 75.0f;           

// --- Cấu trúc quản lý thẻ động hỗ trợ cả 4-byte và 7-byte ---
struct RFIDCard {
    uint8_t size;        // Độ dài UID (4 hoặc 7)
    uint8_t uid[7];      // Chứa tối đa 7 byte UID
};

extern const char PASSWORD[];
extern const RFIDCard VALID_CARDS[];
extern const uint8_t VALID_CARD_COUNT;

#endif