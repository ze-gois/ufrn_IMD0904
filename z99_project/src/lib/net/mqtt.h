#pragma once

#include <PubSubClient.h>
#include "./secrets.h"
#include "./wifi.h"

const char* mqttserver = "io.adafruit.com";
const int mqttport = 1883;
const char* mqttUser = IO_USERNAME;
const char* mqttPassword = IO_KEY;

PubSubClient client(espClient);