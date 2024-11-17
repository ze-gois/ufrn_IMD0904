#include <SoftwareSerial.h>

SoftwareSerial ArduinoSlave(10, 11);
String answer;
String msg;
int intVal = 0, oldIntVal = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  ArduinoSlave.begin(9600);
  PORTH |= (1 << 6);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(9)==LOW){
    ArduinoSlave.print("a");
  }

  intVal=analogRead(A0);
  readSlavePort();

  if(oldIntVal!=intVal){
    Serial.print("Master sent: ");
    Serial.println(intVal);
    ArduinoSlave.print(intVal);
    oldIntVal=intVal;
  }

  if(answer!=""){
    Serial.print("Slave LED PWM value: ");
    Serial.println(answer);
    answer="";
  }
  delay(1000);
}

void readSlavePort(){
  while(ArduinoSlave.available()){
    delay(10);
    if(ArduinoSlave.available() > 0){
      char c = ArduinoSlave.read();
      answer += c;
    }
  }
}