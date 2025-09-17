#include <Arduino.h>

#ifndef RECORD_H
#define RECORD_H
// extern bool do_record;
#endif

void record(const char *msg);
std::string recordf(const char *format, ...);
std::string recordfln(const char *format, ...);