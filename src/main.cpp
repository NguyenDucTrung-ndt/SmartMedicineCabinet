#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <DHT.h>
#include <Servo.h>
#include <Keypad.h>
#include <SPI.h>
#include <MFRC522.h>

// Định nghĩa chân
#define DHTPIN 2
#define SERVO_PIN 3
#define BUZZER 4
#define LED_G 5
#define LED_R 6
#define RST_PIN 9
#define SS_PIN 10

// Khởi tạo đối tượng
LiquidCrystal_I2C lcd(0x27, 20, 4);
RTC_DS1307 rtc;
DHT dht(DHTPIN, DHT22);
Servo servo;
MFRC522 rfid(SS_PIN, RST_PIN);

// Cấu hình Keypad
const byte ROWS = 4, COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'}, {'4','5','6','B'}, {'7','8','9','C'}, {'*','0','#','D'}
};
byte rowPins[ROWS] = {A0, A1, A2, A3};
byte colPins[COLS] = {7, 8, 0, 1};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  dht.begin();
  rtc.begin();
  lcd.init();
  lcd.backlight();
  
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  servo.attach(SERVO_PIN);
  
  // Test nhanh ngoại vi lúc khởi động (Bíp 1 tiếng, nháy LED)
  digitalWrite(LED_G, HIGH); digitalWrite(LED_R, HIGH);
  tone(BUZZER, 1000, 100); delay(200);
  digitalWrite(LED_G, LOW);  digitalWrite(LED_R, LOW);
  servo.write(0);
}

void loop() {
  // 1. Đọc và hiện RTC + DHT22 lên LCD
  DateTime now = rtc.now();
  lcd.setCursor(0, 0);
  lcd.print("Time: "); lcd.print(now.hour()); lcd.print(":"); lcd.print(now.minute()); lcd.print(":"); lcd.print(now.second()); lcd.print("   ");
  
  lcd.setCursor(0, 1);
  lcd.print("T:"); lcd.print(dht.readTemperature(), 1); lcd.print("C H:"); lcd.print(dht.readHumidity(), 1); lcd.print("%   ");

  // 2. Test quẹt thẻ RFID
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    Serial.println("-> RFID: Da quet the!");
    digitalWrite(LED_G, HIGH);
    tone(BUZZER, 1500, 100);
    servo.write(90); // Mở khóa
    delay(2000);
    servo.write(0);  // Đóng khóa
    digitalWrite(LED_G, LOW);
    rfid.PICC_HaltA();
  }

  // 3. Test bấm nút Keypad
  char key = keypad.getKey();
  if (key) {
    Serial.print("-> Keypad: "); Serial.println(key);
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 2000, 50); // Bíp nhẹ khi bấm nút
    delay(100);
    digitalWrite(LED_R, LOW);
  }

  delay(100); // Tốc độ lặp tối ưu giúp Wokwi giả lập siêu nhanh
}