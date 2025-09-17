#ifndef STATE_H
#define STATE_H

// Define the different states for the alarm system
enum class AlarmState {
    IDLE,
    ARMING,         
    ARMED,        
    TRIGGERED,    
    TAMPER,       
    FAULT,        
    CONFIGURATION 
};

extern AlarmState currentState;

// Function prototypes
void handleState();
void handleArm();
void handleIdleState();
void handleArmedState();
void handleTriggeredState();
void handleTamperState();
void handleFaultState();
void handleConfigurationState();
void transitionToState(AlarmState newState);

#endif // STATE_H