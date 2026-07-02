#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b2680"

const int ledPin = 2; // ถ้ายังไม่ติด ให้ลองเปลี่ยนเป็น 5, 13 หรือ 21

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      // แก้ไขตรงนี้: ใช้ String ของ Arduino แทน std::string
      String value = pCharacteristic->getValue();
      
      if (value.length() > 0) {
        // ใช้ .toInt() เพื่อแปลงค่าที่ส่งมาเป็นตัวเลขโดยตรง
        int val = value.toInt();
        
        Serial.print("ค่าที่ได้รับ (Integer): ");
        Serial.println(val);

        // เช็คที่ค่าตัวเลขไปเลย (49 คือรหัส ASCII ของ '1', 48 คือของ '0')
        if (val == 1 || val == 49) {
            digitalWrite(ledPin, LOW); // เปิดไฟ
            Serial.println("สถานะ: ไฟติด (ON)");
        } 
        else if (val == 0 || val == 48) {
            digitalWrite(ledPin, HIGH); // ปิดไฟ
            Serial.println("สถานะ: ไฟดับ (OFF)");
        }
      }
    }
};

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  BLEDevice::init("ESP32_Gunso_BLE");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
                                       
  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->setValue("Ready");
  pService->start();
  
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  BLEDevice::startAdvertising();
  
  Serial.println("BLE พร้อม! ส่งค่า 1 หรือ 0 มาได้เลย");
}

void loop() {
  delay(1000);
}