//code for sender

#include <WiFi.h>
#include <esp_now.h>
#include <DHT.h>
 
// Define DHT11 parameters
#define DHTPin 27
#define DHTType DHT11 
 
// Create DHT Object
DHT dht(DHTPin, DHTType);

int trigPin = 33;    // TRIG pin
int echoPin = 32;    // ECHO pin

float duration_us, distance;
 
// Variables for temperature 
float temp;

// Responder MAC Address (Replace with your responders MAC Address)
uint8_t broadcastAddress[] = {0xB8, 0xD6, 0x1A, 0xB3, 0x09, 0x7C};
 
// Define data structure
typedef struct struct_message {
  float a;
  float b;
  
} struct_message;
 
struct_message myData; // Create structured data object

 
// Register peer
esp_now_peer_info_t peerInfo;
 
// Sent data callback function
void OnDataSent(const uint8_t *macAddr, esp_now_send_status_t status)
{
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  Serial.begin(115200);
  delay(100);
    		// configure the trigger pin to output mode
  pinMode(trigPin, OUTPUT);
  		// configure the echo pin to input mode
  pinMode(echoPin, INPUT);
 		 // Initiate DHT11
  dht.begin();
 
 		 // Set ESP32 WiFi mode to Station temporarly
  WiFi.mode(WIFI_STA);
  // Initialize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
 		 // Define callback
  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
 
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }}
 
void loop() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(echoPin, HIGH);

  // calculate the distance
  distance = 0.017 * duration_us;

  // print the value to Serial Monitor
  Serial.print("distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500);
 
  // Read DHT22 module values
  temp = dht.readTemperature();
  delay(10);
  
 
  Serial.print("Temp: ");
  Serial.println(temp);
  Serial.print("Distance: ");
  Serial.println(distance);
 
  // Add to structured data object
  myData.a = temp;
  myData.b = distance;
 
  // Send data
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
 
  delay(2000);
}
