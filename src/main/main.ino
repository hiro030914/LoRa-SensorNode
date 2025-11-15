#include "LoRaWan_APP.h"
#include "Sensor.h"
#include <Arduino.h>

// LoRaパラメータ設定
// 固定パラメータのためconstexprによる定義
constexpr uint32_t RF_FREQUENCY = 925000000;             // LoRa周波数(Hz)
constexpr int8_t TX_OUTPUT_POWER = 10;                   // 送信出力(dBm)
constexpr int LORA_BANDWIDTH = 0;                        // 125 kHz
constexpr int LORA_SPREADING_FACTOR = 7;                 // SF7
constexpr int LORA_CODINGRATE = 1;                       // CR4/5
constexpr int LORA_PREAMBLE_LENGTH = 8;                  // プレアンブル長 //同期確認
constexpr int LORA_SYMBOL_TIMEOUT = 0;                   // シンボルタイムアウト
constexpr bool LORA_FIX_LENGTH_PAYLOAD_ON = false;       // 可変長ペイロード
constexpr bool LORA_IQ_INVERSION_ON = false;             // IQ反転オフ

static RadioEvents_t RadioEvents;    // イベントハンドラ
bool lora_idle = true;               // LoRaアイドル状態フラグ

uint64_t chipid = ESP.getEfuseMac(); // nodeID取得

static uint32_t seq_counter = 1;

// Sensorオブジェクト作成
DHTSensor dhtSensor;
DHTTemperature tempSensor(dhtSensor.getDHT());
DHTHumidity humidSensor(dhtSensor.getDHT());

void OnTxDone(void);          // 送信成功コールバック
void OnTxTimeout(void);       // 送信タイムアウトコールバック

// センサデータパケット構造体
struct SensorPacket {
  uint32_t node_id;           // センサノード識別子
  float temp_value;           // 温度データ
  float humi_value;           // 湿度データ
};

struct TxPacket {
  uint32_t seq_no;
  SensorPacket payload;
};


void setup() {
  Serial.begin(115200);
  Serial.println("LoRa Node Initialized");

  // 温湿度センサ初期化
  dhtSensor.run();

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

    TxPacket packet;

    packet.payload.node_id = (uint32_t)(chipid & 0xFFFFFFFF);
    packet.seq_no = seq_counter++;

    // 温湿度取得
    packet.payload.temp_value = tempSensor.getData();
    packet.payload.humi_value = humidSensor.getData();

    // パケット送信
    Radio.Send((uint8_t*)&packet, sizeof(packet));
    Serial.printf("TX -> %lu : %.1f°C %.1f%%, %u\n", packet.payload.node_id, packet.payload.temp_value, packet.payload.humi_value, packet.seq_no);

    lora_idle = false;
  }
  Radio.IrqProcess();   // 疑似割り込みによる送信完了処理
}

// 送信完了コールバック
void OnTxDone(void) {
  Serial.println("TX done");
  Radio.Sleep();
  lora_idle = true;
}

// 送信タイムアウトコールバック
void OnTxTimeout(void) {
  Radio.Sleep();
  Serial.println("TX timeout");
  lora_idle = true;
}
