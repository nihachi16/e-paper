#include <WiFi.h>
#include <HTTPClient.h>
#include <SD.h>
#include <SPI.h>

// Wi-Fi接続情報
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// ファイルのURL
String fileURL = "http://example.com/yourfile.txt";
// String fileURL = "http://192.168.1.100/yourfile.txt";

// SDカードのピン設定
#define SD_CS_PIN 5

// バッファのサイズを定義
#define BUFFER_SIZE 512
uint8_t buf[BUFFER_SIZE];

void setup() {
  // シリアル通信の初期化
  Serial.begin(115200);

  // Wi-Fi接続
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wi-Fi connected");

  // SDカードの初期化
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialization failed");
    return;
  }
  Serial.println("SD card initialized");

  // ファイルのダウンロードとSDカードへの書き込み
  downloadAndSaveFile(fileURL);
}

void loop() {
}

void downloadAndSaveFile(String url) {
  HTTPClient httpClient;
  httpClient.begin(url);

  // GETリクエストを実行し、httpコードを取得
  int httpCode = httpClient.GET();
  
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      // ファイル名をURLから取得
      String fileName = url.substring(url.lastIndexOf("/") + 1);
      
      // SDカード上にファイルを作成
      File file = SD.open(fileName, FILE_WRITE);
      
      if (file) {
        // コンテンツサイズを取得
        int contentLength = httpClient.getSize();
        
        // ダウンロードしたデータを書き込む
        WiFiClient stream = httpClient.getStream();
        int bytesWritten = 0;
        int bytesRead = 0;
        while (contentLength) {
          bytesRead = stream.readBytes(buf, sizeof(buf));
          file.write(buf, bytesRead);
          bytesWritten += bytesRead;
          contentLength -= bytesRead;
        }
        
        // ファイルを閉じる
        file.close();
        
        Serial.print("File ");
        Serial.print(fileName);
        Serial.println(" downloaded and saved successfully");
      } else {
        Serial.println("Error creating file on SD card");
      }
    } else {
      Serial.printf("Error downloading file. HTTP code: %d\n", httpCode);
    }
  } else {
    Serial.println("Error during HTTP connection");
  }
  // リソースを解放
  httpClient.end();
}