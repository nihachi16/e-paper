#include "esp_sleep.h"

void setup() {
  Serial.begin(115200);
  
  // ディープスリープに設定されるまで待機
  delay(1000); 
  
  // ディープスリープのタイマーを設定（5時間*60分*60秒*1000000マイクロ秒）
  // esp_sleep_enable_timer_wakeupの引数はマイクロ秒です
  uint64_t sleepTime = 5 * 60 * 60 * 1000000ULL;
  esp_sleep_enable_timer_wakeup(sleepTime);