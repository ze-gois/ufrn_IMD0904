// https://randomnerdtutorials.com/esp32-ntp-client-date-time-arduino-ide

#include <FS.h>
#include "SPIFFS.h"

#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const char* ssid     = "Gois";
const char* password = "s3nh4w1f1";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

int pin_led = 18;
int pin_btn = 19;

int anterior;
int atual;

long long counter;

int pin_btn_log = 21;

int anterior2;
int atual2;

String s;

void writeFile(String state, String path, char *m) { //escreve conteúdo em um arquivo
  File rFile = SPIFFS.open(path, m);
  if (!rFile) {
    Serial.println("Erro ao abrir arquivo!");
  }
  else {
    Serial.print("tamanho");
    Serial.println(rFile.size());
    rFile.println(state);
    Serial.print("Gravou: ");
    Serial.println(state);
  }
  rFile.close();
}

String readFile(String path) {
  Serial.println("Read file");
  File fd = SPIFFS.open(path, "r");
  int count = 0;

  while (!fd && (count < 5)) {
    Serial.println("Erro ao abrir arquivo!");
    writeFile("OFF", path,"w+");
    fd = SPIFFS.open(path, "r");
    count += 1;
  }

  if  (!fd && (count < 5)) {
    s = "-1";
    return s;
  } else {
    Serial.print("----------Lendo arquivo ");
    Serial.print(path);
    Serial.println("  ---------");
    while (fd.position() < fd.size())
    {
      s = fd.readStringUntil('\n');
      s.trim();
      Serial.println(s);
    }
    fd.close();
    return s;
  }
}

void formatFile() {
  Serial.println("Formantando SPIFFS");
  SPIFFS.format();
  Serial.println("Formatou SPIFFS");
}

void openFS(void) {
  if (!SPIFFS.begin()) {
    Serial.println("\nErro ao abrir o sistema de arquivos");
  }
  else {
    Serial.println("\nSistema de arquivos aberto com sucesso!");
  }
}

void setup() {
  Serial.begin(115200);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  timeClient.begin();
  timeClient.setTimeOffset(-3*3600);
  
  /*
  Serial.print("Updating time");
  while(!timeClient.update()) {
    Serial.print(".");
    timeClient.forceUpdate();
  }
  Serial.println(" done");  
  */
  Serial.println("inicio");
  delay(200);
  //formatFile();
  Serial.println("abrir arquivo");
  openFS(); 

  Serial.println("ler arquivo");
  String test = readFile("/logsAula.txt");
  if (test == "") {
    test = "ON";
  }

  if(test == "ON"){
    digitalWrite(pin_led, HIGH);
    counter = 0;
  } else if(test == "OFF"){
    digitalWrite(pin_led, LOW);
    counter = 1;
  }
  
  pinMode(pin_led,OUTPUT);
  pinMode(pin_btn,INPUT);

  anterior = 0;
  atual = 0;

  anterior2 = 0;
  atual2 = 0;
}


int count = 0;
void loop() {
  /*
  count += 1;
  Serial.print(".");
  if (count == 100) {
    Serial.println(".");
    count = 0;
  }
  */

  atual = digitalRead(pin_btn);

  if (atual == 1 && anterior == 0){
    counter += 1;

    String formattedTime = timeClient.getFormattedTime();
    
    if ((counter % 2) == 0) {
      writeFile("OFF", "/logsAula.txt","w+");
      digitalWrite(pin_led,LOW);
      writeFile(formattedTime, "/logsAulaTotal.txt","a+");
      writeFile("OFF", "/logsAulaTotal.txt","a+");
    } else {
      digitalWrite(pin_led,HIGH);
      writeFile("ON", "/logsAula.txt","w+");
      writeFile(formattedTime, "/logsAulaTotal.txt","a+");
      writeFile("ON", "/logsAulaTotal.txt","a+");
    }
    anterior = 1;
  } else if (atual == 0 && anterior == 1) {
    anterior = 0;
  }

  atual2 = digitalRead(pin_btn_log);

  if (atual2 == 1 && anterior2 == 0) {
    Serial.print("Printing:");
    String test = readFile("/logsAulaTotal.txt");
    Serial.println(test);
    anterior2 = atual;
  } else if (atual2 == 0 && anterior2 == 1) {
    anterior2 = atual;
  }

  delay(50);
}


