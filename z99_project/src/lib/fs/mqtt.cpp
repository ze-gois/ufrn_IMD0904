#include "./mqtt.h"

WiFiClient espClient;
PubSubClient client(espClient);

bool systemEnabled = false;

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    if (client.connect("ESP32_PIR", mqttUser, mqttPassword)) {
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

void setup() {
  Serial.begin(9600);

  setup_wifi();
  client.setServer(mqttserver, 1883);
  client.setCallback(callback);

  for (int i = 0; i < numPIR; i++) {
    pinMode(pirPins[i], INPUT);
    Serial.print("Configurando PIR ");
    Serial.println(i + 1);
  }

  Serial.println("Sistema de Segurança Iniciado");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (systemEnabled) {
    for (int i = 0; i < numPIR; i++) {
      int currentState = digitalRead(pirPins[i]);

      if (currentState == HIGH && pirStates[i] == LOW) {
        Serial.print("Movimento detectado pelo sensor PIR ");
        Serial.println(i + 1);
        pirStates[i] = HIGH;

        String topic = "samuel_costa/feeds/sensor-interno";
        String message = "Mov. Detc. ";
        message += (i + 1);
        client.publish(topic.c_str(), message.c_str());
      } else if (currentState == LOW && pirStates[i] == HIGH) {
        pirStates[i] = LOW;
      }
    }
  }

  delay(100);
}
