#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include "../lib/net/secrets.h"

const int trigPin1 = 35;
const int echoPin1 = 34;
const int trigPin2 = 33;
const int echoPin2 = 32;
const int trigPin3 = 26;
const int echoPin3 = 25;
const int trigPin4 = 27;
const int echoPin4 = 14;
const int detectionThreshold = 80;

WiFiClient espClient;
PubSubClient client(espClient);

bool systemEnabled = false;

void setup_wifi() {
  Serial.print("Conectando ao WiFi ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    if (client.connect("ESP32_HCSR04", mqttUser, mqttPassword)) {
      Serial.println("Conectado ao MQTT");
      client.subscribe("samuel_costa/feeds/controlador");
    } else {
      Serial.print("Falha, rc=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (String(topic) == "samuel_costa/feeds/controlador") {
    if (message == "1") {
      systemEnabled = true;
      Serial.println("Sistema ativado");
    } else if (message == "0") {
      systemEnabled = false;
      Serial.println("Sistema desativado");
    }
  }
}

long measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;

  return distance;
}

void setup() {
  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqttserver, 1883);
  client.setCallback(callback);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);

  Serial.println("Sistema de Detecção Iniciado");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (systemEnabled) {
    long distance1 = measureDistance(trigPin1, echoPin1);
    if (distance1 > 0 && distance1 < detectionThreshold) {
      Serial.print("Objeto detectado pelo sensor 1 a ");
      Serial.print(distance1);
      Serial.println(" cm de distância");
      client.publish("samuel_costa/feeds/sensor-externo", "Sensor 1 detectou objeto");
    }

    long distance2 = measureDistance(trigPin2, echoPin2);
    if (distance2 > 0 && distance2 < detectionThreshold) {
      Serial.print("Objeto detectado pelo sensor 2 a ");
      Serial.print(distance2);
      Serial.println(" cm de distância");
      client.publish("samuel_costa/feeds/sensor-externo", "Sensor 2 detectou objeto");
    }

    long distance3 = measureDistance(trigPin3, echoPin3);
    if (distance3 > 0 && distance3 < detectionThreshold) {
      Serial.print("Objeto detectado pelo sensor 3 a ");
      Serial.print(distance3);
      Serial.println(" cm de distância");
      client.publish("samuel_costa/feeds/sensor-externo", "Sensor 3 detectou objeto");
    }

    long distance4 = measureDistance(trigPin4, echoPin4);
    if (distance4 > 0 && distance4 < detectionThreshold) {
      Serial.print("Objeto detectado pelo sensor 4 a ");
      Serial.print(distance4);
      Serial.println(" cm de distância");
      client.publish("samuel_costa/feeds/sensor-externo", "Sensor 4 detectou objeto");
    }
  }

  delay(100);
}
