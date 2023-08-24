#include "FS.h"
#include "SD.h"
#include "SPI.h"

//4GB~32GBはFAT32でフォーマット

#define SCK_PIN 18 // SPIのクロックピン
#define MISO_PIN 19 // SPIのMISOピン
#define MOSI_PIN 23 // SPIのMOSIピン
#define CS_PIN 5 // SDカードのチップセレクトピン

void setup() {
  Serial.begin(115200);
  
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
  if (!SD.begin(CS_PIN)) {
    Serial.println("SDカードの初期化に失敗しました");
    return;
  }
  Serial.println("SDカードの初期化に成功しました");

  // ファイルを読み込む
  File scriptFile = SD.open("/my_script.txt");
  if (!scriptFile) {
    Serial.println("ファイルが見つかりません");
    return;
  }

  String scriptContent = "";
  // ファイルを読み込んで、フラッシュメモリに格納（今回はシリアルコンソールに出力）
  while (scriptFile.available()) {
    char c = scriptFile.read();
    scriptContent += c;
  }
  scriptFile.close();

  // ここに、フラッシュメモリにプログラムを書き込む処理を実装できます。
  Serial.println("------------スクリプト内容------------");
  Serial.print(scriptContent);
  Serial.println("-----------スクリプトの終わり-----------");
}

void loop() {
  // 何もしない
}