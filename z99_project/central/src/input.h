#include <Keypad.h>

struct InputMemory {
  char c;
  InputMemory *ancestral;
  InputMemory *descendant;
};

struct InputSequence {
  InputMemory *former;
  InputMemory *latter;
};

static InputSequence sequence{nullptr, nullptr};
// static InputSequence *sequence = new InputSequence{nullptr, nullptr};
static unsigned long last_input_time;
static const unsigned long MAX_IDLE_INPUT_TIME = 3000;

#ifndef INPUT_H
#define INPUT_H
const uint8_t ROWS = 4;
const uint8_t COLS = 4;

extern char keys[ROWS][COLS];
extern uint8_t colPins[COLS];
extern uint8_t rowPins[ROWS];

void detect_key();
char assimilate();
bool parecer(const char* wit);
void obliviate();
void patience();
#endif