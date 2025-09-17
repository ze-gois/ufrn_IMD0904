#include <Arduino.h>
#include <vector>

typedef struct {
  const char * descriptor;
  const char * pattern;
  void (*once_function)(void);
  void (*loop_function)(void);
  unsigned long interval;
  unsigned long latter_call;
} Command;

#ifndef COMMAND_H
#define COMMAND_H
extern void new_command(const char * descriptor, const char * pattern, void (*once_function)(void),void (*loop_function)(void),unsigned long interval);
extern void new_once_command(const char * descriptor, const char * pattern, void (*once_function)(void));
extern bool obey(Command cmd);
extern void reflect();
extern void review();
#endif