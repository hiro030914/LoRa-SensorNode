#include "Sensor.h"
#include <DHT.h>

#define DHT_PIN 5
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

void DHTTemperature::run() {
  dht.begin();
  Serial.println("DHT Temperature Sensor started");
}

void DHTTemperature::read() {
  m_data = dht.readTemperature();

  if (isnan(m_data)) {
    Serial.println("Failed to read temperature from DHT");
    return;
  }
  
 /* m_numberOfSensorData++;
  if (m_numberOfSensorData >= m_maxSize) m_numberOfSensorData = 0;

   コンテンツ名をICN形式で設定
   m_contentName = String("/sensor/temp/") + String(m_numberOfSensorData);*/ 
}

float DHTTemperature::getData() {
  return m_data;
}

/*String DHTTemperature::getContentName() {
  return m_contentName;
}*/
