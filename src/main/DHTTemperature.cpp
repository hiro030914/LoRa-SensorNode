#include "Sensor.h"
#include <DHT.h>

#define DHT_PIN 5         // DHTの接続ピン
#define DHT_TYPE DHT11    // DHTのタイプ(DHT11, DHT22など)

// DHTオブジェクトの作成
DHT dht(DHT_PIN, DHT_TYPE);

// DHTセンサの初期化
void DHTTemperature::run() {
  dht.begin();
  Serial.println("DHT Temperature Sensor started");
}

// 温度データ読み込み
void DHTTemperature::read() {
  m_data = dht.readTemperature();

  // データのエラーチェック
  if (isnan(m_data)) {
    Serial.println("Failed to read temperature from DHT");
    return;
  }
  
 /* m_numberOfSensorData++;
  if (m_numberOfSensorData >= m_maxSize) m_numberOfSensorData = 0;

   コンテンツ名をICN形式で設定
   m_contentName = String("/sensor/temp/") + String(m_numberOfSensorData);*/ 
}

// 温度データ取得
float DHTTemperature::getData() {
  return m_data;
}

/*String DHTTemperature::getContentName() {
  return m_contentName;
}*/
