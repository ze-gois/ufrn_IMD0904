#include <SoftwareSerial.h>

SoftwareSerial ArduinoMaster(10, 11);

#define ledPin 12

String msg="";
int ledVal=0;
int intVal = 0, oldIntVal = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  ArduinoMaster.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  readMasterPort();
  convertMsgToCmd();
  
  if(oldIntVal!=intVal){
    Serial.print("Master sent: ");
    Serial.println(intVal);

    ledVal=map(intVal, 0, 1023, 0, 255);
    Serial.print("led value: ");
    Serial.println(ledVal);
    ArduinoMaster.print(ledVal);
    analogWrite(ledPin, ledVal);
    oldIntVal=intVal;
  }
}

void readMasterPort(){
  while(ArduinoMaster.available()){
    delay(10);
    if(ArduinoMaster.available() > 0){
      char c = ArduinoMaster.read();

      msg += c;
    }
  }
  ArduinoMaster.flush();
}

void convertMsgToCmd(){
  if(msg.length() >0){
    Serial.print("message length :");
    Serial.println(msg.length());

    if (msg == "a"){
      intVal = 1023;
    } else {
      char carrayl[6];
      msg.toCharArray(carrayl, sizeof(carrayl));
      intVal = atoi(carrayl);
    }
    
    msg="";
  }
}