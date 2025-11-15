#ifndef INCLUDED_Sensor_h_
#define INCLUDED_Sensor_h_

#include <Arduino.h>
#include <DHT.h>

// ---- センサ基底クラス ----
class Sensor {
public:
  virtual void run() = 0;               // センサ初期化
  virtual void read() = 0;              // センサデータ読み込み
  virtual float getData() = 0;          // センサデータ取得
};

// ---- a -----

class DHTSensor : public Sensor {
public:
  DHTSensor();
  void run() override;
  DHT& getDHT();

private:
  static constexpr int DHT_PIN = 5;
  static constexpr int DHT_TYPE = DHT11;
  DHT dht;
};

// ---- 派生温度センサクラス ----
class DHTTemperature : public Sensor {
public:
  //void run() override;
  DHTTemperature(DHT& dht_ref);
  void read() override;
  float getData() override;

private:
  DHT& dhtTemp;
  float m_data;
};

// ---- 派生湿度センサクラス ----
class DHTHumidity : public Sensor {
public:
  //void run() override;
  DHTHumidity(DHT& dht_ref);
  void read() override;
  float getData() override;

private:
  DHT& dhtHumi;
  float m_data;
};

#endif // INCLUDED_Sensor_h_
