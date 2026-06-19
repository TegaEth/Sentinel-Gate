#include "StateMachine.h"
#include "Arduino.h"

static SystemState currentState;
static CommsMode commsMode;

SystemState getCurrentState(){
  return currentState;
};



void transitionTo(SystemState newState){
  if (getCurrentState() == newState){
    // checking if the new state is same as the current state
    // should do something but not sure yet 
  }else{
    if(getCurrentState() != newState){
        currentState = newState;
    }
  }
}