#include <WiFi.h>
#include <WebServer.h>

// ชื่อ Wi-Fi และรหัสผ่าน Hotspot ของคุณ
const char* ssid = "Panpan";
const char* password = "gunsoon1";

WebServer server(80);

// ประกาศใช้งานขา 1 และขา 2 
const int pin1 = 1; 
const int pin2 = 2; // ขาไฟ LED สีฟ้าบนบอร์ด

void setup() {
  Serial.begin(115200);
  
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  
  // *** แก้ไข: สั่ง HIGH เพื่อให้ไฟดับตอนเริ่มต้น (Active-LOW) ***
  digitalWrite(pin1, HIGH); 
  digitalWrite(pin2, HIGH); 

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // ดักรับคำสั่งเมื่อหน้าเว็บส่ง /on มา
  server.on("/on", []() {
    digitalWrite(pin1, LOW); // สั่ง LOW ไฟจะติด
    digitalWrite(pin2, LOW); // สั่ง LOW ไฟจะติด
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "ON");
  });

  // ดักรับคำสั่งเมื่อหน้าเว็บส่ง /off มา
  server.on("/off", []() {
    digitalWrite(pin1, HIGH); // สั่ง HIGH ไฟจะดับ
    digitalWrite(pin2, HIGH); // สั่ง HIGH ไฟจะดับ
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "OFF");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}