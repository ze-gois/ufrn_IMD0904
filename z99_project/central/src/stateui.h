#include <Keypad.h>
#include <LiquidCrystal.h>

#ifndef STATEUI_H
#define STATEUI_H
extern LiquidCrystal lcd;
extern Keypad keypad;
enum class StateUI {
    MAIN_MENU,
    WAIT_FOR_INPUT,
    REGISTER_USER,
    VERIFY_WIFI,
    CONFIGURE_WIFI,
    DISPLAY_SUCCESS,
    DISPLAY_ERROR
};
#endif

// Function prototypes
void handleMainMenu();
void handleWaitForInput();
void handleRegisterUser();
void handleVerifyWiFi();
void handleConfigureWiFi();
void handleDisplaySuccess();
void handleDisplayError();
void transitionToStateUI(StateUI newState);
void updateLCD(const char* message);
