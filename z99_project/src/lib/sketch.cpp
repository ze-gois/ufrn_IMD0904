




// void salvarEstadoLed(int estado) {
//   File file = SPIFFS.open("/estado_led.txt", FILE_WRITE);
//   if (file) {
//     file.println(estado);
//     file.close();
//     Serial.println("Estado do LED salvo.");
//   } else {
//     Serial.println("Falha ao salvar estado do LED.");
//   }
// }

// int carregarEstadoLed() {
//   File file = SPIFFS.open("/estado_led.txt", FILE_READ);
//   if (file) {
//     String estado = file.readStringUntil('\n');
//     file.close();
//     Serial.println("Estado do LED carregado.");
//     return estado.toInt();
//   } else {
//     Serial.println("Falha ao carregar estado do LED.");
//     return LOW;
//   }
// }

// void registrarEvento(const String& evento) {
//   File file = SPIFFS.open("/eventos.txt", FILE_APPEND);
//   if (file) {
//     String timestamp = timeClient.getFormattedTime();
//     file.println(timestamp + " - " + evento);
//     file.close();
//     Serial.println("Evento registrado: " + evento);
//   } else {
//     Serial.println("Falha ao registrar evento.");
//   }
// }

// void publicarEstadoContador() {
//   char s_contador[8];
//   dtostrf(contador, 1, 0, s_contador);
//   client.publish("samuel_costa/feeds/contador", s_contador);
//   Serial.print("Publicando contador: ");
//   Serial.println(s_contador);
// }

// void publicarEstadoLuz() {
//   int estadoAtual = digitalRead(ledVermelho);
//   if (estadoAtual != ultimoEstadoLed) {
//     const char* estadoLuz = estadoAtual == HIGH ? "1" : "0";
//     client.publish("samuel_costa/feeds/luz-da-sala", estadoLuz);
//     Serial.print("Publicando estado da luz: ");
//     Serial.println(estadoLuz);
//     ultimoEstadoLed = estadoAtual;
//     salvarEstadoLed(estadoAtual);
//   }
// }

// void ajustarEstadoLuz() {
//   if (contador == 0 && digitalRead(ledVermelho) == HIGH) {
//     digitalWrite(ledVermelho, LOW);
//     client.publish("samuel_costa/feeds/interruptor", "0");
//     Serial.println("Luz apagada automaticamente. Contador = 0.");
//     publicarEstadoLuz();
//   } else if (contador > 0 && controleAutomaticoAtivo && digitalRead(ledVermelho) == LOW) {
//     digitalWrite(ledVermelho, HIGH);
//     client.publish("samuel_costa/feeds/interruptor", "1");
//     Serial.println("Luz acesa automaticamente. Contador > 0.");
//     publicarEstadoLuz();
//   }
// }

// void setup_wifi() {
//   delay(10);
//   Serial.println();
//   Serial.print("Conectando a ");
//   Serial.println(ssid);

//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, password);

//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   Serial.println("\nWiFi conectado");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());
// }

// void reconnect() {
//   while (!client.connected()) {
//     Serial.print("Tentando conexão MQTT...");
//     String clientId = "ESP32_Sala";
//     clientId += String(random(0xffff), HEX);
//     if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
//       Serial.println("Conectado!");
//       client.subscribe("samuel_costa/feeds/interruptor");
//     } else {
//       Serial.print("Falha na conexão, rc=");
//       Serial.println(client.state());
//       delay(5000);
//     }
//   }
// }

// void callback(char* topic, byte* payload, unsigned int length) {
//   Serial.print("Mensagem recebida [");
//   Serial.print(topic);
//   Serial.print("] ");

//   String message;
//   for (int i = 0; i < length; i++) {
//     message += (char)payload[i];
//   }
//   Serial.println(message);

//   if (String(topic) == "samuel_costa/feeds/interruptor") {
//     if (message == "1") {
//       digitalWrite(ledVermelho, HIGH);
//       controleAutomaticoAtivo = true;
//       tempoUltimaInteracaoManual = millis();
//       Serial.println("Luz ligada via interruptor no Adafruit IO.");
//     } else if (message == "0" && contador > 0) {
//       digitalWrite(ledVermelho, LOW);
//       controleAutomaticoAtivo = false;
//       tempoUltimaInteracaoManual = millis();
//       Serial.println("Luz desligada via interruptor no Adafruit IO.");
//     }
//     publicarEstadoLuz();
//   }
// }

// void medirDistancia(int trigPin, int echoPin, float &distanciaCm) {
//   digitalWrite(trigPin, LOW);
//   delayMicroseconds(2);
//   digitalWrite(trigPin, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(trigPin, LOW);
//   long duracaoPulso = pulseIn(echoPin, HIGH);
//   distanciaCm = duracaoPulso * SOUND_SPEED / 2;
// }

// void verificarSensores() {
//   medirDistancia(trigPin_entrada, echoPin_entrada, distanciaCm_entrada);
//   medirDistancia(trigPin_saida, echoPin_saida, distanciaCm_saida);

//   static bool etapaEntrada1 = false, etapaEntrada2 = false, etapaEntrada3 = false;
//   static bool etapaSaida1 = false, etapaSaida2 = false, etapaSaida3 = false;
//   static bool bloqueioEntrada = false, bloqueioSaida = false;

//   if (!bloqueioEntrada && !etapaEntrada1 && distanciaCm_entrada < 10 && distanciaCm_saida >= 10) {
//     etapaEntrada1 = true;
//     bloqueioSaida = true;
//   }
//   if (etapaEntrada1 && !etapaEntrada2 && distanciaCm_entrada < 10 && distanciaCm_saida < 10) {
//     etapaEntrada2 = true;
//   }
//   if (etapaEntrada2 && !etapaEntrada3 && distanciaCm_entrada >= 10 && distanciaCm_saida < 10) {
//     etapaEntrada3 = true;
//   }
//   if (etapaEntrada3 && distanciaCm_entrada >= 10 && distanciaCm_saida >= 10) {
//     contador++;
//     etapaEntrada1 = etapaEntrada2 = etapaEntrada3 = false;
//     bloqueioSaida = false;
//     Serial.println("Pessoa entrou. Contador: " + String(contador));
//     publicarEstadoContador();
//     ajustarEstadoLuz();
//   }

//   if (!bloqueioSaida && !etapaSaida1 && distanciaCm_saida < 10 && distanciaCm_entrada >= 10) {
//     etapaSaida1 = true;
//     bloqueioEntrada = true;
//   }
//   if (etapaSaida1 && !etapaSaida2 && distanciaCm_entrada < 10 && distanciaCm_saida < 10) {
//     etapaSaida2 = true;
//   }
//   if (etapaSaida2 && !etapaSaida3 && distanciaCm_saida < 10 && distanciaCm_entrada >= 10) {
//     etapaSaida3 = true;
//   }
//   if (etapaSaida3 && distanciaCm_saida >= 10 && distanciaCm_entrada >= 10) {
//     if (contador > 0) {
//       contador--;
//       Serial.println("Pessoa saiu. Contador: " + String(contador));
//       publicarEstadoContador();
//       ajustarEstadoLuz();
//     }
//     etapaSaida1 = etapaSaida2 = etapaSaida3 = false;
//     bloqueioEntrada = false;
//   }
// }

// void setup() {
//   Serial.begin(115200);
//   pinMode(btPin, INPUT);
//   pinMode(ledVermelho, OUTPUT);
//   pinMode(trigPin_entrada, OUTPUT);
//   pinMode(echoPin_entrada, INPUT);
//   pinMode(trigPin_saida, OUTPUT);
//   pinMode(echoPin_saida, INPUT);

//   // if (!SPIFFS.begin(true)) {
//   //   Serial.println("Falha ao montar SPIFFS");
//   //   return;
//   // }

//   setup_wifi();
//   client.setServer(mqttserver, mqttport);
//   client.setCallback(callback);
//   timeClient.begin();

//   digitalWrite(ledVermelho, carregarEstadoLed());
// }

// void loop() {
//   if (!client.connected()) {
//     reconnect();
//   }
//   client.loop();

//   verificarSensores();

//   leitura = digitalRead(btPin);
//   if (leitura == HIGH && ultimoEstado == LOW && (millis() - tempoUltimoClique) > debounceDelay) {
//     tempoUltimoClique = millis();
//     if (digitalRead(ledVermelho) == HIGH) {
//       digitalWrite(ledVermelho, LOW);
//       registrarEvento("Luz desligada pelo botão");
//       client.publish("samuel_costa/feeds/interruptor", "0");
//       controleAutomaticoAtivo = false;
//       tempoUltimaInteracaoManual = millis();
//     } else if (digitalRead(ledVermelho) == LOW && contador > 0) {
//       digitalWrite(ledVermelho, HIGH);
//       registrarEvento("Luz ligada pelo botão");
//       client.publish("samuel_costa/feeds/interruptor", "1");
//       controleAutomaticoAtivo = true;
//       tempoUltimaInteracaoManual = millis();
//     }
//     publicarEstadoLuz();
//   }
//   ultimoEstado = leitura;
// }