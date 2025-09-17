#include <iostream>
#include "./state.h"
#include "./display.h"
#include "./input.h"
#include "./record.h"
#include "./command.h"

// Current state of the alarm system
AlarmState currentState = AlarmState::IDLE;

// Main state handler
void handleState() {
    switch (currentState) {
        case AlarmState::IDLE:
            handleIdleState();
            break;
        case AlarmState::ARMING:
            handleArmedState();
            break;
        case AlarmState::ARMED:
            handleArmedState();
            break;
        case AlarmState::TRIGGERED:
            handleTriggeredState();
            break;
        case AlarmState::TAMPER:
            handleTamperState();
            break;
        case AlarmState::FAULT:
            handleFaultState();
            break;
        case AlarmState::CONFIGURATION:
            handleConfigurationState();
            break;
        default:
            recordf("Unknown state!");
            break;
    }
}

// State-specific behavior
void handleIdleState() {

}

void handleArm() {
    if (currentState == AlarmState::IDLE) {
        recordf("System is ARMING...\n");
        transitionToState(AlarmState::ARMED);
        obliviate();
    } else if (currentState == AlarmState::ARMED) {
        recordf("System is DISARMING...\n");
        transitionToState(AlarmState::IDLE);
        obliviate();
    }
}

void handleArmedState() {
    
}

void handleTriggeredState() {
    
}

void handleTamperState() {
    
}

void handleFaultState() {
    
}

void handleConfigurationState() {
    
}

// Main state handler
std::string stateString(AlarmState alarmState) {
    switch (alarmState) {
        case AlarmState::IDLE:
            return "IDLE";
        case AlarmState::ARMING:
            return "ARMING";
        case AlarmState::ARMED:
            return "ARMED";
        case AlarmState::TRIGGERED:
            return "TRIGGERED";
        case AlarmState::TAMPER:
            return "TAMPER";
        case AlarmState::FAULT:
            return "FAULT";
        case AlarmState::CONFIGURATION:
            return "CONFIGURATION";
        default:
            return "UNK";
    }
}

void transitionToState(AlarmState newState) {
    recordf("Transitioning from %s to %s...\n", stateString(currentState), stateString(newState));
    currentState = newState;
}