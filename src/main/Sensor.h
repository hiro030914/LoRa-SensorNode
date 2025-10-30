#ifndef INCLUDED_Sensor_h_
#define INCLUDED_Sensor_h_

#include <Arduino.h>

class Sensor {
public:
  virtual void run() = 0;
  virtual void read() = 0;
  virtual float getData() = 0;
  //virtual String getContentName() = 0;
};

// ---- 温度センサクラス ----
class DHTTemperature : public Sensor {
public:
  void run() override;
  void read() override;
  float getData() override;
  //float getContentName() override;

private:
  float m_data;
  //String m_contentName;
  const int m_maxSize = 20;
  int m_numberOfSensorData = 0;
};

// ---- 湿度センサクラス ----
class DHTHumidity : public Sensor {
public:
  void run() override;
  void read() override;
  float getData() override;
  //String getContentName() override;

private:
  float m_data;
  //String m_contentName;
  const int m_maxSize = 20;
  int m_numberOfSensorData = 0;
};

#endif // INCLUDED_Sensor_h_
