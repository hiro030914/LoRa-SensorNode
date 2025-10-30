#include "LoRaWan_APP.h"
#include "Sensor.h"
#include <Arduino.h>

#define RF_FREQUENCY                925000000
#define TX_OUTPUT_POWER             14
#define LORA_BANDWIDTH              0
#define LORA_SPREADING_FACTOR       7
#define LORA_CODINGRATE             1
#define LORA_PREAMBLE_LENGTH        8
#define LORA_FIX_LENGTH_PAYLOAD_ON  false
#define LORA_IQ_INVERSION_ON        false

static RadioEvents_t RadioEvents;
bool lora_idle = true;

uint64_t chipid = ESP.getEfuseMac();

// Sensor objects
DHTTemperature tempSensor;
DHTHumidity humidSensor;

void OnTxDone(void);
void OnTxTimeout(void);

struct SensorPacket {
  uint32_t node_id;
  char contentName[20];
  float value;
};

void setup() {
  Serial.begin(115200);
  Serial.println("LoRa Node Initialized");

  tempSensor.run();
  humidSensor.run();

  Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  Radio.Init(&RadioEvents);
  Radio.SetChannel(RF_FREQUENCY);
  Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                    LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                    LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                    true, 0, 0, LORA_IQ_INVERSION_ON, 3000);
}

void loop() {
  if (lora_idle) {
    delay(2000);

    tempSensor.read();
    humidSensor.read();

    SensorPacket packet;
    SensorPacket test;
    packet.node_id = (uint32_t)(chipid & 0xFFFFFFFF);
    test.node_id = (uint32_t)(chipid & 0xFFFFFFFF);

    // 温度送信
    strcpy(packet.contentName, tempSensor.getContentName().c_str());
    packet.value = tempSensor.getData().toFloat();
    Radio.Send((uint8_t*)&packet, sizeof(packet));
    Serial.printf("TX -> %s : %.1f°C\n", packet.contentName, packet.value);

    delay(1000);

    // 湿度送信
    strcpy(packet.contentName, humidSensor.getContentName().c_str());
    packet.value = humidSensor.getData().toFloat();
    Radio.Send((uint8_t*)&packet, sizeof(packet));
    Serial.printf("TX -> %s : %.1f%%\n", packet.contentName, packet.value);

    

    lora_idle = false;
  }
  Radio.IrqProcess();
}

void OnTxDone(void) {
  Serial.println("TX done");
  lora_idle = true;
}

void OnTxTimeout(void) {
  Radio.Sleep();
  Serial.println("TX timeout");
  lora_idle = true;
}
