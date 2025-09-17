#include <Arduino.h>

#include "lib/net/mqtt.h"
#include "lib/fs/spiffs.h"
#include "lib/time/time.h"
#include "lib/net/wifi.h"

#include "main.h"

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  Serial.println("OK");
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}