/*

  https://randomnerdtutorials.com/esp32-ntp-client-date-time-arduino-ide/
*/
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

const char* ssid     = "npiti-iot";
const char* password = "npiti-iot";

// Set the NTP server to query (for example, "pool.ntp.org")
const char* ntpServer = "pool.ntp.org";

// Create an instance of the WiFiUDP class
WiFiUDP udp;

// Create an instance of the NTPClient class
NTPClient timeClient(udp, ntpServer, 0, 60000);  // Update every 60 seconds (60000 ms)


/*
  Includes para o sistema de arquivo da Spressif
*/
#include <FS.h>
#include "SPIFFS.h"


int pin_led = 0;
int pin_btn = 4;

int anterior;
int atual;
long long counter;

bool primeiro;

void connectWiFi(){
    WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

}

void setup() {  
  timeClient.begin();

  Serial.begin(9600);
  connectWiFi();

  pinMode(pin_led,OUTPUT);
  pinMode(pin_btn,INPUT);

  anterior = 0;
  atual = 0;
  counter = 0;

  primeiro = true;
}

void loop() {
  atual = digitalRead(pin_btn);

  if (primeiro || (atual == 1 && anterior == 0)){
    counter += 1;

    if ((counter % 2) == 0) {
      digitalWrite(pin_led,LOW);  
    } else {
      digitalWrite(pin_led,HIGH);
      timeClient.update();
  
        // Print the current time to the Serial Monitor
        Serial.print("Current time: ");
        Serial.println(timeClient.getFormattedTime());  // Format: HH:MM:SS
    }
    
    primeiro = false;
    anterior = 1;
  } else if (atual == 0 && anterior == 1) {
    anterior = 0;
  }

  delay(50);
}
