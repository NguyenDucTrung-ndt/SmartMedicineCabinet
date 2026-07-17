# Tủ Thuốc Thông Minh (Smart Medicine Cabinet)
Dự án tủ thuốc thông minh bảo quản y tế có bảo mật hai lớp (Mật khẩu Keypad + Thẻ từ RFID), tự động chốt khóa bằng động cơ Servo, cảnh báo nhiệt độ/độ ẩm vượt ngưỡng an toàn bằng còi hú/đèn LED, và hiển thị thời gian thực (RTC DS1307) lên màn hình LCD 20x4.
Dự án được xây dựng và chạy mô phỏng bằng công cụ **PlatformIO IDE** và **Wokwi Simulator** trực tiếp trên VS Code.
---
## 📂 Cấu trúc thư mục dự án
```text
SmartMedicineCabinet/
├── include/                # Thư mục chứa các file khai báo Header (.h)
│   ├── Config.h            # Cấu hình hằng số, pinout, password, UID thẻ RFID
│   ├── AlarmManager.h      # Quản lý còi Buzzer phát âm thanh
│   ├── DHTManager.h        # Quản lý đọc cảm biến DHT22
│   ├── Display.h           # Quản lý các màn hình hiển thị trên LCD 20x4
│   ├── LockManager.h       # Quản lý đóng/mở chốt Servo
│   ├── PasswordManager.h   # Xử lý quét phím Keypad và logic mật khẩu
│   ├── RFIDManager.h       # Xử lý đọc thẻ từ MFRC522
│   └── RTCManager.h        # Quản lý đọc và định dạng thời gian thực từ DS1307
├── src/                    # Thư mục chứa code chạy chính (.cpp)
│   ├── main.cpp            # Khởi tạo luồng chạy loop chính
│   ├── Config.cpp          
│   ├── AlarmManager.cpp    
│   ├── DHTManager.cpp      
│   ├── Display.cpp         
│   ├── LockManager.cpp     
│   ├── PasswordManager.cpp 
│   ├── RFIDManager.cpp     
│   └── RTCManager.cpp      
├── diagram.json            # Bản vẽ mạch điện mô phỏng trên Wokwi
├── platformio.ini          # Cấu hình nạp chip, thư viện dự án của PlatformIO
└── wokwi.toml              # Đường dẫn trỏ file chạy hex cho trình mô phỏng
🔌 Sơ đồ kết nối chân (Pinout Arduino Uno)
1. Bàn phím Keypad 4x4
Hàng R1, R2, R3, R4 -> Chân A0, A1, A2, A3
Cột C1, C2, C3, C4 -> Chân 7, 8, 1 (TX), 0 (RX)
2. Mạch đọc thẻ RFID RC522
SDA (SS) -> Chân 10
SCK -> Chân 13
MOSI -> Chân 11
MISO -> Chân 12
RST -> Chân 9
VCC -> Nguồn 3.3V của Arduino
GND -> GND
3. Màn hình LCD 20x4 (Giao tiếp I2C) & RTC DS1307 (I2C)
SDA -> Chân A4
SCL -> Chân A5
VCC -> Nguồn 5V
GND -> GND
4. Thiết bị ngoại vi khác
Cảm biến DHT22: Chân dữ liệu SDA -> Chân 2
Động cơ Servo SG90: Chân điều khiển PWM -> Chân 3
Còi Buzzer: Chân cực dương -> Chân 4
LED Xanh (Green): Chân cực dương -> Chân 5
LED Đỏ (Red): Chân cực dương -> Chân 6
Hướng dẫn cài đặt và Chạy mô phỏng (How to run)
Bước 1: Cài đặt phần mềm hỗ trợ
Cài đặt phần mềm VS Code và cài 2 Extension sau:

PlatformIO IDE (Quản lý dự án, biên dịch mã nguồn).
Wokwi Simulator (Chạy mạch điện mô phỏng trực quan).
Bước 2: Cấu hình thư viện tự động
Đảm bảo file platformio.ini ở thư mục gốc của bạn đã có cấu hình các thư viện sau để PlatformIO tự động tải về:

ini


[env:uno]
platform = atmelavr
board = uno
framework = arduino
lib_ldf_mode = deep
lib_deps =
    adafruit/DHT sensor library @ ^1.4.6
    adafruit/Adafruit Unified Sensor @ ^1.1.14
    marcoschwartz/LiquidCrystal_I2C @ ^1.1.4
    chrisheydrick/Keypad @ ^3.1.1
    miguelbalboa/MFRC522 @ ^1.4.11
    arduino-libraries/Servo @ ^1.2.1
    adafruit/RTClib @ ^2.1.4
Bước 3: Biên dịch mã nguồn (Build)
Bấm vào biểu tượng Dấu tích (✓) ở thanh trạng thái dưới cùng của VS Code (hoặc nhấn tổ hợp phím Ctrl + Alt + B).
Đợi PlatformIO biên dịch xong báo chữ [SUCCESS] màu xanh lá cây.
Bước 4: Khởi chạy mô phỏng
Nhấn tổ hợp phím Ctrl + Shift + P (hoặc nút F1) để mở bảng lệnh của VS Code.
Gõ chữ wokwi và chọn lệnh: Wokwi: Start Simulator.
Tab Wokwi Simulator sẽ xuất hiện. Nhấn nút Play (Màu xanh) trên mạch điện ảo để chạy dự án.