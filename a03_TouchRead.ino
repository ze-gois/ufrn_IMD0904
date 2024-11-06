int LED_amarelo = 25;
int touch_amarelo = 15;
int pntos_amarelo = 0;

int LED_vermelho = 26;
int touch_vermelho = 14;
int pntos_vermelho = 0;

int LED_verde = 27;
int touch_verde = 4;
int pntos_verde = 0;

bool HAS_BEGAN;

void setup() {
  Serial.begin(115200);

  Serial.println("Let the game begin.");
  
  pinMode(LED_amarelo,OUTPUT);
  pinMode(LED_vermelho,OUTPUT);
  pinMode(LED_verde,OUTPUT);

  digitalWrite(LED_amarelo,HIGH);

  HAS_BEGAN = false;
}

bool touchRead2(int pin){
  return touchRead(pin) < 30;
}

unsigned long begin_time = 0;

void loop() {
  if (!HAS_BEGAN) {
    Serial.print("Rodada: ");
    Serial.println(pntos_amarelo);

    Serial.println("Placar: ");
    Serial.print("\tVermelho:");
    Serial.println(pntos_vermelho);
    Serial.print("\tVerde:");
    Serial.println(pntos_verde);

    digitalWrite(LED_amarelo,HIGH);
    while (!touchRead2(touch_amarelo)) {
    }

    Serial.println("Início de rodada");
    digitalWrite(LED_amarelo,LOW);
    delay(200);
    digitalWrite(LED_amarelo,HIGH);
    delay(200);
    digitalWrite(LED_amarelo,LOW);
    HAS_BEGAN = true;
    begin_time = millis();
  
  } else {
    while(HAS_BEGAN){
      if(touchRead2(touch_verde)){
        Serial.print("Delay: ");
        Serial.println(begin_time - millis());
        HAS_BEGAN = false;
        pntos_verde += 1;
        digitalWrite(LED_verde,HIGH);
        delay(1000);
        digitalWrite(LED_verde,LOW);
        delay(10);

      }
      if(touchRead2(touch_vermelho)){
        Serial.print("Delay: ");
        Serial.println(begin_time - millis());
        HAS_BEGAN = false;
        pntos_vermelho += 1;
        digitalWrite(LED_vermelho,HIGH);
        delay(1000);
        digitalWrite(LED_vermelho,LOW);
        delay(10);
      }
      
    }
  } 
}
