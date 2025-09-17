#include "state.h"
#include "stateui.h"

StateUI currentStateUI = StateUI::MAIN_MENU;

void handleStateUI() {
    switch (currentStateUI) {
        case StateUI::MAIN_MENU:
            handleMainMenu();
            break;
        case StateUI::WAIT_FOR_INPUT:
            handleWaitForInput();
            break;
        case StateUI::REGISTER_USER:
            handleRegisterUser();
            break;
        case StateUI::VERIFY_WIFI:
            handleVerifyWiFi();
            break;
        case StateUI::CONFIGURE_WIFI:
            handleConfigureWiFi();
            break;
        case StateUI::DISPLAY_SUCCESS:
            handleDisplaySuccess();
            break;
        case StateUI::DISPLAY_ERROR:
            handleDisplayError();
            break;
        default:
            updateLCD("UI Error");
            break;
    }
}

void handleMainMenu() {
    lcd.setCursor(1,0);
    switch (currentState) {
        case AlarmState::IDLE:
            lcd.print("Sis. Desativ.");
            break;
        default:
            lcd.print("dest. unknown");
    }
}

void handleWaitForInput(){

}

void handleRegisterUser(){

}

void handleVerifyWiFi(){

}

void handleConfigureWiFi(){

}

void handleDisplaySuccess(){

}

void handleDisplayError(){

}

