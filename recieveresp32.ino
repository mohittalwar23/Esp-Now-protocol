#include <WiFi.h>
#include <esp_now.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(19, 23, 18, 14, 27, 15);
 
// Define data structure
typedef struct struct_message {
    float a;
    float b;
} struct_message;
 
struct_message myData;  // Create structured data object
 
// Callback function
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) 
{
  memcpy(&myData, incomingData, sizeof(myData));	// Get incoming data

  lcd.setCursor(10,1);
  lcd.print(myData.a);
  lcd.setCursor(14,0);
  lcd.print(myData.b);

  if (myData.a > 25){
    digitalWrite(9, LOW);
    delay(500);
    digitalWrite(9, HIGH);
    delay(500);  }
}
 
void setup() {
  pinMode(26, OUTPUT);  //FOR LED
  lcd.begin(16,2);
  lcd.print("Distance:-");
  lcd.setCursor(0,1);
  lcd.print("Temp:-");
  
  WiFi.mode(WIFI_STA);   // Start ESP32 in Station mode
 
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);
}
