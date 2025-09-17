#include "./command.h"
#include <Arduino.h>
#include <vector>
#include "./input.h"
#include "./record.h"

static std::vector<Command>commands = {};

void new_command(const char * descriptor, const char * pattern, void (*once_function)(void), void (*loop_function)(void), unsigned long interval) {
  Command command;
  command.descriptor = descriptor;
  command.pattern = pattern;
  command.once_function = once_function;
  command.loop_function = loop_function;
  command.interval = interval;
  command.latter_call = 0;
  
  commands.push_back(command);
}

void new_once_command(const char * descriptor, const char * pattern, void (*once_function)(void)) {
  Command command;
  command.descriptor = descriptor;
  command.pattern = pattern;
  command.once_function = once_function;
  command.loop_function = nullptr;
  command.interval = 0;
  command.latter_call = 0;
  
  commands.push_back(command);
}

bool obey(Command cmd) {
  if (!parecer(cmd.pattern)) return false;
  if (cmd.once_function) {
    recordf(">>> ");
    recordfln(cmd.descriptor);
    cmd.once_function();
  } else {
    recordfln("Command function is null.");
    return false;
  }

  return true;
}

void reflect() {
  for (auto command : commands) {
    if (obey(command)) {
      obliviate();
      return;
    }
  }
}

void review() {
  for (auto command: commands) {
    if (command.loop_function != nullptr) {
      unsigned long current_time = millis();
      if (current_time - command.latter_call > command.interval) {
        command.loop_function();
        command.latter_call = current_time;
      }
    }
  }
}