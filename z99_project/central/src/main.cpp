#include <Wire.h>
#include <WiFi.h>

#include "./time.h"
#include "./mqtt.h"
#include "../lib/net/secrets.h"
#include "../lib/net/ntp.h"
#include "./command.h"
#include "./display.h"
#include "./input.h"
#include "./state.h"
#include "./chrono.h"

void setup() {
  Serial.begin(115200);

  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);

  new_once_command("Clear", "x",obliviate);
  new_once_command("Senha 1", "1234",handleArm);
  
  new_command("Refresh time","",nullptr,printLocalTime,2000);
  new_command("Handle state","",nullptr,handleState,0);
  new_command("Update time","",nullptr,time_local_update,0);

  beginWifi();
}

void loop() {
  char key = NO_KEY;

  if (key = assimilate() != NO_KEY ) {
    reflect();
  }

  review();

  patience();
}