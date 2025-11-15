#include "Sensor.h"
#include <DHT.h>

DHTSensor::DHTSensor()
  : dht{DHT_PIN, DHT_TYPE}
{}

void DHTSensor::run() {
    dht.begin();
    Serial.println("DHT Temperature and Humidity Sensor started");
}

DHT& DHTSensor::getDHT() {
    return dht;
}