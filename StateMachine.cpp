#include "StateMachine.h"
#include "Arduino.h"

static SystemState currentState = SystemState::ARMED;
static CommsMode commsMode = CommsMode::CONNECTED;

const char* systemStateToString(SystemState state){
  switch(state){
    case SystemState::IDLE:
      return "IDLE";

    case SystemState::ARMED:
      return "ARMED";

    case SystemState::TRIGGERED:
      return "TRIGGERED";

    case SystemState::CHALLENGE:
      return "CHALLENGE";

    case SystemState::LOCKED_OUT:
      return "LOCKED_OUT";
    default:
      return "UNKNOWN STATE"; 
  }
}

SystemState getCurrentState(){
  return currentState;
}

CommsMode getCommsMode(){
  return commsMode;
}

void handleEvent(Event event){
  Serial.print(">>> [INFO] Event Handler Active...");

  switch(event){
    
  }
}

void transitionTo(SystemState newState){
  Serial.print("State Transition in progress...");

  if (currentState == newState){
    //state stays same 
    Serial.print("[WARN] transitionTo called with same state: ");
    Serial.println(systemStateToString(newState));
  }else{
    SystemState oldState = currentState;
    currentState = newState;

    Serial.print(">>> [STATE] ");
    Serial.print(systemStateToString(oldState));
    Serial.print(" -> ");
    Serial.println(systemStateToString(newState));
    }
}