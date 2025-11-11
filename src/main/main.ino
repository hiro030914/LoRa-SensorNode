#include "LoRaWan_APP.h"
#include "Sensor.h"
#include <Arduino.h>

// LoRaパラメータ設定
// 固定パラメータのためconstexprによる定義
constexpr uint32_t RF_FREQUENCY = 925000000;             // LoRa周波数(Hz)
constexpr int8_t TX_OUTPUT_POWER = 14;                   // 送信出力(dBm)
constexpr int LORA_BANDWIDTH = 0;                        // 125 kHz
constexpr int LORA_SPREADING_FACTOR = 7;                 // SF7
constexpr int LORA_CODINGRATE = 1;                       // CR4/5
constexpr int LORA_PREAMBLE_LENGTH = 8;                  // プレアンブル長 //同期確認
constexpr int LORA_SYMBOL_TIMEOUT = 0;                   // シンボルタイムアウト
constexpr bool LORA_FIX_LENGTH_PAYLOAD_ON = false;       // 可変長ペイロード
constexpr bool LORA_IQ_INVERSION_ON = false;             // IQ反転オフ

// デバッグ用 (送信成功確率)
static uint32_t sendSuccess = 0;
static uint32_t sendFail = 0;
unsigned long lastReport = 0;
uint32_t total = 0;


static RadioEvents_t RadioEvents;    // イベントハンドラ
bool lora_idle = true;               // LoRaアイドル状態フラグ

uint64_t chipid = ESP.getEfuseMac(); // nodeID取得

// Sensorオブジェクト作成
DHTTemperature tempSensor;
DHTHumidity humidSensor;

void OnTxDone(void);          // 送信成功コールバック
void OnTxTimeout(void);       // 送信タイムアウトコールバック
void printSendStatus(void);   // 送信成功確率出力

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
  Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                    LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                    LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                    0, true, 0, 0, LORA_IQ_INVERSION_ON, true);
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

  // 送信成功確率50回ごとに出力
  total = sendSuccess + sendFail;
  if (total != 0 && total % 20 == 0) {
    printSendStatus();
  }
  
}

// 送信完了コールバック
void OnTxDone(void) {
  Serial.println("TX done");
  sendSuccess++;      // 送信成功回数
  Radio.Sleep();
  lora_idle = true;
}

// 送信タイムアウトコールバック
void OnTxTimeout(void) {
  sendFail++;        // 送信失敗回数
  Radio.Sleep();
  Serial.println("TX timeout");
  lora_idle = true;
}

// 送信成功確率出力
void printSendStatus() {
  float successRate = (total > 0) ? ((float)sendSuccess / total * 100.0f) : 0.0f;
  Serial.printf("送信成功確率 : %.1f%%\n", successRate);
}