// ==========================================
// --- FILE 5/15: include/DHTManager.h ------
// ==========================================
#ifndef DHT_MANAGER_H
#define DHT_MANAGER_H

#include <Arduino.h>
#include <DHT.h>

class DHTManager {
public:
    explicit DHTManager(DHT& dhtSensorRef);
    void begin();
    void update(); 
    float getTemperature() const;
    float getHumidity() const;
    bool isTemperatureOutOfRange() const; // Kiểm tra nhiệt độ ngoài dải 15-30
    bool isHumidityOutOfRange() const;    // Kiểm tra độ ẩm vượt 75%

private:
    DHT& dhtSensor;
    float temperature;
    float humidity;
    unsigned long lastUpdateMs;
};

#endif