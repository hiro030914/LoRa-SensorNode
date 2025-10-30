#include "Sensor.h"
#include <DHTesp.h>

#define DHT_PIN 5
DHTesp dht_humi;
  
void DHTHumidity::run() {
  dht_humi.setup(DHT_PIN, DHTesp::DHT11);
  Serial.println("DHT Humidity Sensor started");
}

void DHTHumidity::read() {
  TempAndHumidity d = dht_humi.getTempAndHumidity();
  if (isnan(d.humidity)) {
    Serial.println("Failed to read humidity from DHT");
    return;
  }

  m_data = d.humidity;
  m_numberOfSensorData++;
  if (m_numberOfSensorData >= m_maxSize) m_numberOfSensorData = 0;

  // コンテンツ名をICN形式で設定
  //m_contentName = String("/sensor/humid/") + String(m_numberOfSensorData);
}

String DHTHumidity::getData() {
  return m_data;
}

/*String DHTHumidity::getContentName() {
  return m_contentName;
}*/
