#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define ntp_url "pool.ntp.org"

#ifndef TIME_H
#define TIME_H
static struct tm time_local;
static WiFiUDP ntpUDP;
static NTPClient timeClient(ntpUDP, ntp_url);
#endif

void time_local_update();

