#include "./input.h"
#include "./record.h"

char keys[ROWS][COLS] = {
  { '1', '2', '3', 'x' },
  { '4', '5', '6', 'o' },
  { '7', '8', '9', '^' },
  { '<', '0', '>', 'v' }
};

uint8_t colPins[COLS] = { 18, 5, 4, 2 };
uint8_t rowPins[ROWS] = { 23, 22, 21, 19};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

char assimilate() {
  char key = keypad.getKey();

  if (key == NO_KEY){  
    return NO_KEY;
  }
  InputMemory* newcommer = new InputMemory{key, sequence.latter, nullptr};
  
  if (sequence.former == nullptr) {
    sequence.former = newcommer;
  } else {
    if (sequence.former->descendant == nullptr) {
      sequence.former->descendant = newcommer;
    } else {
      sequence.latter->descendant = newcommer;
    }
  } 
  
  sequence.latter = newcommer;
  last_input_time = millis();
  return key;
}

bool parecer(const char* wit) {
  int wit_len = strlen(wit);
  InputMemory* current = sequence.latter;

  recordfln("Input buffer: ");
  for (int i = wit_len - 1; i >= 0 && current != nullptr; i--, current = current->ancestral) {
    recordf("\t'%c'",current->c);
    recordf(": '%c'",wit[i]);
    if (current->c != wit[i]) {
      return false;
    }
  }

  return true;
}

void obliviate() {
  while (sequence.former) {
    InputMemory* fared = sequence.former;
    sequence.former = sequence.former->descendant;
    delete fared; 
  }
  sequence.latter = nullptr;
}

void patience() {
  if ((millis() - last_input_time) > MAX_IDLE_INPUT_TIME && sequence.former) {
    recordf("Input timeout, clearing buffer.");
    obliviate();
  }
}