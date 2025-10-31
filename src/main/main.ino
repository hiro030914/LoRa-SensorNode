#include "LoRaWan_APP.h"
#include "Sensor.h"
#include <Arduino.h>

#define RF_FREQUENCY                925000000  // LoRa周波数
#define TX_OUTPUT_POWER             14         // 送信出力(dBm)
#define LORA_BANDWIDTH              0          // 125 kHz
#define LORA_SPREADING_FACTOR       7          // SF7
#define LORA_CODINGRATE             1          // CR4/5
#define LORA_PREAMBLE_LENGTH        8          // Preamble length
#define LORA_FIX_LENGTH_PAYLOAD_ON  false
#define LORA_IQ_INVERSION_ON        false

static RadioEvents_t RadioEvents;    // イベントハンドラ
bool lora_idle = true;               // LoRaアイドル状態フラグ

uint64_t chipid = ESP.getEfuseMac(); // nodeID取得

// Sensorオブジェクト作成
DHTTemperature tempSensor;
DHTHumidity humidSensor;

void OnTxDone(void);
void OnTxTimeout(void);

// センサデータパケット構造体
struct SensorPacket {
  uint32_t node_id;
  //char contentName[20];
  float temp_value;
  float humi_value;
};

void setup() {
  Serial.begin(115200);
  Serial.println("LoRa Node Initialized");

  // 温湿度センサ初期化
  tempSensor.run();
  humidSensor.run();

  // LoRa初期化
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
    delay(10000);    // 送信間隔10秒

    // センサデータ読み込み
    tempSensor.read();
    humidSensor.read();

    SensorPacket packet;
    packet.node_id = (uint32_t)(chipid & 0xFFFFFFFF);

    // 温湿度取得
    packet.temp_value = tempSensor.getData();
    packet.humi_value = humidSensor.getData();

    // パケット送信
    Radio.Send((uint8_t*)&packet, sizeof(packet));
    Serial.printf("TX -> %lu : %.1f°C %.1f%%\n", packet.node_id, packet.temp_value, packet.humi_value);

    lora_idle = false;
  }
  Radio.IrqProcess();   // 疑似割り込みによる送信完了処理
}

// loRa送信完了コールバック
void OnTxDone(void) {
  Serial.println("TX done");
  Radio.Sleep();
  lora_idle = true;
}

// loRa送信タイムアウトコールバック
void OnTxTimeout(void) {
  Radio.Sleep();
  Serial.println("TX timeout");
  lora_idle = true;
}
