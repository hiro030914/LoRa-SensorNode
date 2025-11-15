#include "Sensor.h"
#include <DHT.h>

DHTHumidity::DHTHumidity(DHT& dht_ref)
  : dhtHumi(dht_ref)
{}

// 湿度データ読み込み
void DHTHumidity::read() {

  m_data = dhtHumi.readHumidity();

// データのエラーチェック
  if (isnan(m_data)) {
    Serial.println("Failed to read humidity from DHT");
    return;
  }
}

// 湿度データ取得
float DHTHumidity::getData() {
  return m_data;
}
