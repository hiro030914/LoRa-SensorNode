#include "Sensor.h"
#include <DHT.h>

DHTHumidity::DHTTemperature(DHT& dht_ref)
  : dhtTemp(dht_ref)
{}

// 温度データ読み込み
void DHTTemperature::read() {
  m_data = dhtTemp.readTemperature();

  // データのエラーチェック
  if (isnan(m_data)) {
    Serial.println("Failed to read temperature from DHT");
    return;
  }
}

// 温度データ取得
float DHTTemperature::getData() {
  return m_data;
}