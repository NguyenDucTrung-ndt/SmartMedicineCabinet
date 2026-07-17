#include "DHTManager.h"
#include "Config.h"

DHTManager::DHTManager(DHT& dhtSensorRef)
    : dhtSensor(dhtSensorRef), temperature(NAN), humidity(NAN), lastUpdateMs(0) {}

void DHTManager::begin() {
    dhtSensor.begin();
    lastUpdateMs = 0;
    temperature = NAN;
    humidity = NAN;
    
    float newTemperature = dhtSensor.readTemperature();
    float newHumidity = dhtSensor.readHumidity();
    if (!isnan(newTemperature) && !isnan(newHumidity)) {
        temperature = newTemperature;
        humidity = newHumidity;
    }
}

void DHTManager::update() {
    unsigned long now = millis();
    if (now - lastUpdateMs < DHT_SAMPLE_INTERVAL_MS) {
        return;
    }
    lastUpdateMs = now;

    float newTemperature = dhtSensor.readTemperature();
    float newHumidity = dhtSensor.readHumidity();
    if (!isnan(newTemperature) && !isnan(newHumidity)) {
        temperature = newTemperature;
        humidity = newHumidity;
    }
}

float DHTManager::getTemperature() const { return temperature; }
float DHTManager::getHumidity() const { return humidity; }
// Trả về true nếu nhiệt độ dưới 15 độ C hoặc trên 30 độ C
bool DHTManager::isTemperatureOutOfRange() const {
    if (isnan(temperature)) return false;
    return (temperature < TEMPERATURE_MIN || temperature > TEMPERATURE_MAX);
}

// Trả về true nếu độ ẩm vượt quá 75%
bool DHTManager::isHumidityOutOfRange() const {
    if (isnan(humidity)) return false;
    return (humidity > HUMIDITY_MAX);
}