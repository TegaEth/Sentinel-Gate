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


void handleEvent(Event event){
  Serial.print(">>> [INFO] Event Handler Active...");

  switch(currentState){
    case SystemState::IDLE:
      switch(event){
        case Event::CMD_ARM:
          Serial.println("Received CMD_ARM, transitioning to ARMED state...");
          transitionTo(SystemState::ARMED); 
          break;

        case Event::HEARTBEAT_FAILED:
          Serial.println("[COMMS] Link lost, entering autonomous mode...");
          commsMode = CommsMode::AUTONOMOUS;
          break;

        case Event::HEARTBEAT_RESTORED:
          Serial.println("[COMMS] Link restored, resuming connected mode...");
          commsMode = CommsMode::CONNECTED;
          // in later phases this also triggers EEPROM flush
          break;
        
      }
      break;

    case SystemState::ARMED:
      switch(event){
        case Event::BOOT_COMPLETE:
          Serial.println(">>> [BOOT] Self-check passed. SENTINEL-GATE is now operational. transitioning to ARMED state...");
          break;
        case Event::PIR_CONFIRMED:
          Serial.println(">>> [INFO] PIR confirmed, transitioning to TRIGGERED state...");
          transitionTo(SystemState::TRIGGERED); 
          break;

        case Event::CMD_LOCKDOWN:
          Serial.println(">>> [CMD] Received CMD_LOCKDOWN, transitioning to LOCKED_OUT state...");
          transitionTo(SystemState::LOCKED_OUT); 
          break;
        
        case Event::CMD_DISARM:
          Serial.println(">>> [CMD] Received CMD_DISARM, transitioning to IDLE state...");
          transitionTo(SystemState::IDLE); 
          break;
        
        case Event::HEARTBEAT_FAILED:
          Serial.println(">>> [COMMS] Link lost, entering autonomous mode...");
          commsMode = CommsMode::AUTONOMOUS;
          break;

        case Event::HEARTBEAT_RESTORED:
          Serial.println(">>> [COMMS] Link restored, resuming connected mode...");
          commsMode = CommsMode::CONNECTED;
          // in later phases this also triggers EEPROM flush
          break;

        
      }
      break;

    case SystemState::TRIGGERED:
      switch(event){
        case Event::PIR_CLEARED:
          Serial.println(">>> [INFO] PIR cleared, transitioning to ARMED state...");
          transitionTo(SystemState::ARMED); 
          break;
        case Event::RFID_CARD_PRESENTED:
          Serial.println(">>> [INFO] RFID card presented, transitioning to CHALLENGE state...");
          transitionTo(SystemState::CHALLENGE); 
          break;
        case Event::IDENTIFICATION_WINDOW_EXPIRED:
          Serial.println(">>> [INFO] Identification window expired, transitioning to LOCKED_OUT state...");
          transitionTo(SystemState::LOCKED_OUT); 
          break;
        case Event::HEARTBEAT_FAILED:
          Serial.println(">>> [COMMS] Link lost, entering autonomous mode...");
          commsMode = CommsMode::AUTONOMOUS;
          break;

        case Event::HEARTBEAT_RESTORED:
          Serial.println(">>> [COMMS] Link restored, resuming connected mode...");
          commsMode = CommsMode::CONNECTED;
          // in later phases this also triggers EEPROM flush
          break;
        case Event::CMD_RESET:
          Serial.println(">>> [CMD] Received CMD_RESET, transitioning to ARMED state...");
          transitionTo(SystemState::ARMED); 
          break;

        case Event::CMD_LOCKDOWN:
          Serial.println(">>> [CMD] Received CMD_LOCKDOWN, transitioning to LOCKED_OUT state...");
          transitionTo(SystemState::LOCKED_OUT); 
          break;
      }
      break;

    case SystemState::CHALLENGE:
      switch(event){
        case Event::CHALLENGE_TIMEOUT:
          Serial.println(">>> [INFO] Challenge timeout, transitioning to LOCKED_OUT state...");
          transitionTo(SystemState::LOCKED_OUT); 
          break;
        case Event::RFID_AUTHORIZED:
          Serial.println(">>> [INFO] RFID authorized, transitioning to ARMED state...");
          transitionTo(SystemState::ARMED); 
          break;
        case Event::RFID_UNAUTHORIZED:
          Serial.println(">>> [INFO] RFID unauthorized, transitioning to LOCKED_OUT state...");
          transitionTo(SystemState::LOCKED_OUT); 
          break;
        case Event::RFID_UNKNOWN:
          Serial.println(">>> [INFO] RFID unknown, transitioning to LOCKED_OUT state...");
          transitionTo(SystemState::LOCKED_OUT); 
          break;
        case Event::HEARTBEAT_FAILED:
          Serial.println(">>> [COMMS] Link lost, entering autonomous mode...");
          commsMode = CommsMode::AUTONOMOUS;
          break;

        case Event::HEARTBEAT_RESTORED:
          Serial.println(">>> [COMMS] Link restored, resuming connected mode...");
          commsMode = CommsMode::CONNECTED;
          // in later phases this also triggers EEPROM flush
          break;
        
        case Event::CMD_RESET:
          Serial.println(">>> [CMD] Received CMD_RESET, transitioning to ARMED state...");
          transitionTo(SystemState::ARMED); 
          break;

        case Event::CMD_LOCKDOWN:
          Serial.println(">>> [CMD] Received CMD_LOCKDOWN, transitioning to LOCKED_OUT state...");
          transitionTo(SystemState::LOCKED_OUT); 
          break;
      }
      break;

    case SystemState::LOCKED_OUT:
      switch(event){
        case Event::LOCKOUT_EXPIRED:
          Serial.println(">>> [INFO] Lockout expired, transitioning to ARMED state...");
          transitionTo(SystemState::ARMED); 
          break;
        case Event::CMD_RELEASE_LOCKDOWN:
          Serial.println(">>> [CMD] Received CMD_RELEASE_LOCKDOWN, transitioning to ARMED state...");
          transitionTo(SystemState::ARMED); 
          break;
        case Event::HEARTBEAT_FAILED:
          Serial.println(">>> [COMMS] Link lost, entering autonomous mode...");
          commsMode = CommsMode::AUTONOMOUS;
          break;

        case Event::HEARTBEAT_RESTORED:
          Serial.println(">>> [COMMS] Link restored, resuming connected mode...");
          commsMode = CommsMode::CONNECTED;
          // in later phases this also triggers EEPROM flush
          break;
        
        case Event::CMD_RESET:
          Serial.println(">>> [CMD] Received CMD_RESET, transitioning to ARMED state...");
          transitionTo(SystemState::ARMED); 
          break;
        

      }
      break;
      
    default:
      break;
  }
}

