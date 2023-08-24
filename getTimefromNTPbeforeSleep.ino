#include <WiFi.h>
#include "TimeLib.h"
#include "ezTime.h"

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* ntpServer = "pool.ntp.org";
const long timezoneOffset = 9 * 3600;

const uint64_t sleep_duration_us = 60000000; // 1分

RTC_DATA_ATTR int bootCount = 0;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Wi-Fi関連の設定関数
  if (bootCount == 0) {
    connect_to_wifi();
    sync_time_from_ntp();
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    Serial.println("Wi-Fi disconnected.");
  }

  ++bootCount;
  updateTime();
  printLocalTime();

  // 初回起動時のみWi-Fiに接続してNTPサーバから時刻を取得
  if (bootCount == 1) {
    Serial.println("Entering deep sleep mode...");
    esp_sleep_enable_timer_wakeup(sleep_duration_us);
    esp_deep_sleep_start();
  }
}

void loop() {
  // ディープスリープから戻ると、ここには来ないためコードは必要ありません
}

void connect_to_wifi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wi-Fi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void sync_time_from_ntp() {
  configTime(timezoneOffset, 0, ntpServer);
  set_zone(timezoneOffset);
  waitForSync();
  Serial.println("Time synchronized!");
}

void printLocalTime() {
  tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  Serial.println("Awoke from deep sleep.");
  Serial.printf("Boot count: %d\n", bootCount);
  Serial.print("Current time: ");
  Serial.print(timeinfo.tm_year + 1900);
  Serial.print("/");
  Serial.print(timeinfo.tm_mon + 1);
  Serial.print("/");
  Serial.print(timeinfo.tm_mday);
  Serial.print(" ");
  Serial.print(timeinfo.tm_hour);
  Serial.print(":");
  Serial.print(timeinfo.tm_min);
  Serial.print(":");
  Serial.println(timeinfo.tm_sec);
}
