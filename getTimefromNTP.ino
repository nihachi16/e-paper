#include <WiFi.h>
#include "TimeLib.h"
#include "ezTime.h"

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

const char* ntpServer = "ntp.nict.jp";
const long timezoneOffset = 9 * 3600; // 日本のタイムゾーンオフセット (秒)
const int update_interval = 10000;

unsigned long last_time_update = 0;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Wi-Fiに接続
  Serial.println();
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

  // NTPサーバの設定
  configTime(timezoneOffset, 0, ntpServer);
  set_zone(timezoneOffset);

}

void loop() {
  if (millis() - last_time_update >= update_interval) {
    last_time_update = millis();
    printLocalTime();
  }
  delay(1000);
}

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.print("Current time: ");
  Serial.print(timeinfo.tm_year + 1900);  // 年
  Serial.print("/");
  Serial.print(timeinfo.tm_mon + 1);      // 月
  Serial.print("/");
  Serial.print(timeinfo.tm_mday);         // 日
  Serial.print(" ");
  Serial.print(timeinfo.tm_hour);         // 時
  Serial.print(":");
  Serial.print(timeinfo.tm_min);          // 分
  Serial.print(":");
  Serial.print(timeinfo.tm_sec);          // 秒
  Serial.println();
}
