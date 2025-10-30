#include "Sensor.h"
#include <DHTesp.h>

#define DHT_PIN 5
DHTesp dht_temp;

void DHTTemperature::run() {
  dht_temp.setup(DHT_PIN, DHTesp::DHT11);
  Serial.println("DHT Temperature Sensor started");
}

void DHTTemperature::read() {
  TempAndHumidity d = dht_temp.getTempAndHumidity();
  if (isnan(d.temperature)) {
    Serial.println("Failed to read temperature from DHT");
    return;
  }

  m_data = d.temperature;
 // m_numberOfSensorData++;
  if (m_numberOfSensorData >= m_maxSize) m_numberOfSensorData = 0;

  // コンテンツ名をICN形式で設定
 // m_contentName = String("/sensor/temp/") + String(m_numberOfSensorData);
}

String DHTTemperature::getData() {
  return m_data;
}

/*String DHTTemperature::getContentName() {
  return m_contentName;
}*/
