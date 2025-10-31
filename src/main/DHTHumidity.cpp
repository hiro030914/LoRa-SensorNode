#include "Sensor.h"
#include <DHT.h>
  
void DHTHumidity::run() {
  Serial.println("DHT Temperature And Humidity Sensor started");
}

void DHTHumidity::read() {

  m_data = dht.readHumidity();

  if (isnan(m_data)) {
    Serial.println("Failed to read humidity from DHT");
    return;
  }

  /*m_numberOfSensorData++;
  if (m_numberOfSensorData >= m_maxSize) m_numberOfSensorData = 0;

   コンテンツ名をICN形式で設定
   m_contentName = String("/sensor/humid/") + String(m_numberOfSensorData);*/
}

float DHTHumidity::getData() {
  return m_data;
}

/*String DHTHumidity::getContentName() {
  return m_contentName;
}*/
