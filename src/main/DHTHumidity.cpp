#include "Sensor.h"
#include <DHT.h>

// DHTの初期化はDHTTemperatureで行う
void DHTHumidity::run() {
  Serial.println("DHT Temperature And Humidity Sensor started");
}

// 湿度データ読み込み
void DHTHumidity::read() {

  m_data = dht.readHumidity();

// データのエラーチェック
  if (isnan(m_data)) {
    Serial.println("Failed to read humidity from DHT");
    return;
  }

  /*m_numberOfSensorData++;
  if (m_numberOfSensorData >= m_maxSize) m_numberOfSensorData = 0;

   コンテンツ名をICN形式で設定
   m_contentName = String("/sensor/humid/") + String(m_numberOfSensorData);*/
}

// 湿度データ取得
float DHTHumidity::getData() {
  return m_data;
}

/*String DHTHumidity::getContentName() {
  return m_contentName;
}*/
