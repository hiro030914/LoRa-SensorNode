#ifndef INCLUDED_Sensor_h_
#define INCLUDED_Sensor_h_

#include <Arduino.h>
#include <DHT.h>

#define DHT_PIN 5
#define DHT_TYPE DHT11

extern DHT dht;  // DHTオブジェクトの宣言  //temp,humi両方で使用するためexternによる宣言

// ---- センサ基底クラス ----
class Sensor {
public:
  virtual void run() = 0;               // センサ初期化
  virtual void read() = 0;              // センサデータ読み込み
  virtual float getData() = 0;          // センサデータ取得
  //virtual String getContentName() = 0;
};

// ---- 派生温度センサクラス ----
class DHTTemperature : public Sensor {
public:
  void run() override;
  void read() override;
  float getData() override;
  //float getContentName() override;

private:
  float m_data;
  //String m_contentName;
  //const int m_maxSize = 20;
  //int m_numberOfSensorData = 0;
};

// ---- 派生湿度センサクラス ----
class DHTHumidity : public Sensor {
public:
  void run() override;
  void read() override;
  float getData() override;
  //String getContentName() override;

private:
  float m_data;
  //String m_contentName;
  //const int m_maxSize = 20;
  //int m_numberOfSensorData = 0;
};

#endif // INCLUDED_Sensor_h_
