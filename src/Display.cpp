#include "Display.h"
#include "Config.h"

Display::Display(LiquidCrystal_I2C& lcdRef)
    : lcd(lcdRef), temperature(NAN), humidity(NAN), doorOpen(false), locked(false),
      countdownSeconds(0), screenMode(ScreenMode::Startup), lastScreenMode(ScreenMode::Home), forceRefresh(false) {
    passwordInput[0] = '\0';
    dateTime[0] = '\0';
}

void Display::begin() {
    lcd.init();
    lcd.backlight();
    lcd.clear();
    screenMode = ScreenMode::Startup;
    passwordInput[0] = '\0';
    dateTime[0] = '\0';
}

void Display::showStartup() { screenMode = ScreenMode::Startup; }
void Display::showHome() { screenMode = ScreenMode::Home; }
void Display::showWarning() { screenMode = ScreenMode::Warning; }
void Display::showPasswordEntry() { screenMode = ScreenMode::PasswordEntry; }
void Display::showPasswordAccepted() { screenMode = ScreenMode::PasswordAccepted; }
void Display::showPasswordDenied() { screenMode = ScreenMode::PasswordDenied; }
void Display::showRfidAccepted() { screenMode = ScreenMode::RfidAccepted; }
void Display::showRfidDenied() { screenMode = ScreenMode::RfidDenied; }
void Display::showLockCountdown() { screenMode = ScreenMode::LockCountdown; }

// Định nghĩa hàm gán thời gian thực
void Display::setDateTime(const char* dateTimeValue) {
    strncpy(dateTime, dateTimeValue, sizeof(dateTime) - 1);
    dateTime[sizeof(dateTime) - 1] = '\0';
}

void Display::setTemperature(float temperatureValue) { temperature = temperatureValue; }
void Display::setHumidity(float humidityValue) { humidity = humidityValue; }
void Display::setDoorOpen(bool isOpen) { doorOpen = isOpen; }
void Display::setLocked(bool isLocked) { locked = isLocked; }

void Display::setPasswordInput(const char* inputValue) {
    strncpy(passwordInput, inputValue, sizeof(passwordInput) - 1);
    passwordInput[sizeof(passwordInput) - 1] = '\0';
}

void Display::setCountdown(unsigned long seconds) { countdownSeconds = seconds; }

void Display::forceRefreshNextUpdate() { forceRefresh = true; }

void Display::update() {
    unsigned long now = millis();
    bool screenChanged = (screenMode != lastScreenMode) || forceRefresh;
    
    static unsigned long lastLcdUpdateMs = 0;
    if (!screenChanged && (now - lastLcdUpdateMs < 300)) {
        return;
    }
    lastLcdUpdateMs = now;

    if (screenMode != lastScreenMode) {
        lcd.clear();
        lastScreenMode = screenMode;
    }
    forceRefresh = false;

    bool tempValid = !isnan(temperature) && (temperature > -40.0f) && (temperature < 80.0f);
    bool humidityValid = !isnan(humidity) && (humidity >= 0.0f) && (humidity <= 100.0f);

    switch (screenMode) {
        case ScreenMode::Startup:
            lcd.setCursor(0, 0); lcd.print(" SMART MEDICINE ");
            lcd.setCursor(0, 1); lcd.print("     CABINET     ");
            lcd.setCursor(0, 2); lcd.print("Dang khoi tao...    ");
            break;

        case ScreenMode::Home:
            // Dòng 1: Hiện thời gian thực
            lcd.setCursor(0, 0);
            if (dateTime[0] != '\0') {
                lcd.print(dateTime);
            } else {
                lcd.print("Loading RTC...      ");
            }

            // Dòng 2: Hiện độ ẩm và nhiệt độ
            lcd.setCursor(0, 1); 
            lcd.print("T: ");
            if (tempValid) {
                lcd.print(temperature, 1); lcd.print((char)223); lcd.print("C ");
            } else {
                lcd.print("Loi ");
            }
            lcd.print(" H: ");
            if (humidityValid) {
                lcd.print((int)humidity); lcd.print("% ");
            } else {
                lcd.print("Loi ");
            }
            lcd.print("    ");

            // Dòng 3: Cửa đóng/mở
            lcd.setCursor(0, 2); lcd.print("Cua      : ");
            lcd.print(doorOpen ? "MO   " : "KHOA ");

            // Dòng 4: Hệ thống
            lcd.setCursor(0, 3); lcd.print("He thong : AN TOAN ");
            break;

        case ScreenMode::Warning:
            lcd.setCursor(0, 0); lcd.print("   CANH BAO!       ");
            
            // Xử lý động hiển thị chi tiết nguyên nhân vượt ngưỡng
            {
                bool tempErr = (temperature < TEMPERATURE_MIN || temperature > TEMPERATURE_MAX);
                bool humiErr = (humidity > HUMIDITY_MAX);
                lcd.setCursor(0, 1);
                if (tempErr && humiErr) {
                    lcd.print("QUA NGUONG T & H   "); // Vượt cả 2
                } else if (tempErr) {
                    lcd.print("QUA NGUONG NHIET DO"); // Chỉ vượt nhiệt độ
                } else if (humiErr) {
                    lcd.print("QUA NGUONG DO AM  "); // Chỉ vượt độ ẩm
                } else {
                    lcd.print("NGOAI NGUONG AN TOAN"); // Trường hợp dự phòng
                }
            }

            lcd.setCursor(0, 2); lcd.print("T: ");
            if (tempValid) {
                lcd.print(temperature, 1); lcd.print((char)223); lcd.print("C ");
            } else {
                lcd.print("Loi ");
            }
            lcd.print("H: ");
            if (humidityValid) {
                lcd.print((int)humidity); lcd.print("% ");
            } else {
                lcd.print("Loi ");
            }
            lcd.setCursor(0, 3); lcd.print("He thong : CANH BAO ");
            break;

        case ScreenMode::PasswordEntry:
            lcd.setCursor(0, 0); lcd.print("Nhap mat khau:      ");
            lcd.setCursor(0, 1);
            {
                uint8_t len = strlen(passwordInput);
                lcd.setCursor(0, 1);
                for (uint8_t i = 0; i < PASSWORD_LENGTH; ++i) {
                    if (i < len) {
                        lcd.print("* ");
                    } else {
                        lcd.print("_ ");
                    }
                }
                lcd.print("       ");
            }
            lcd.setCursor(0, 2); lcd.print("Nhan # de xac nhan  ");
            lcd.setCursor(0, 3); lcd.print("Nhan * de xoa het   ");
            break;

        case ScreenMode::PasswordAccepted:
            lcd.setCursor(0, 0); lcd.print("  MAT KHAU DUNG    ");
            lcd.setCursor(0, 1); lcd.print("   DANG MO CUA     ");
            break;

        case ScreenMode::PasswordDenied:
            lcd.setCursor(0, 0); lcd.print("   SAI MAT KHAU    ");
            lcd.setCursor(0, 1); lcd.print("VUI LONG THU LAI  ");
            break;

        case ScreenMode::RfidAccepted:
            lcd.setCursor(0, 0); lcd.print("    THE HOP LE      ");
            lcd.setCursor(0, 1); lcd.print("   DANG MO CUA     ");
            break;

        case ScreenMode::RfidDenied:
            lcd.setCursor(0, 0); lcd.print("  THE KHONG HOP LE  ");
            lcd.setCursor(0, 1); lcd.print("  VUI LONG THU LAI  ");
            break;

        case ScreenMode::LockCountdown:
            lcd.setCursor(0, 0); lcd.print("  HE THONG BI KHOA  ");
            lcd.setCursor(0, 1); lcd.print(" Vui long doi...    ");
            lcd.setCursor(0, 2); lcd.print(" Con lai: ");
            lcd.print(countdownSeconds); lcd.print("s   ");
            break;
    }
}