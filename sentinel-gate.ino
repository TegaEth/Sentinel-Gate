#include "StateMachine.h"

// Helper function to print the CommsMode as text
const char* commsModeToString(CommsMode mode) {
  switch(mode) {
    case CommsMode::CONNECTED: return "CONNECTED";
    case CommsMode::AUTONOMOUS: return "AUTONOMOUS";
    default: return "UNKNOWN";
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) { ; }

  Serial.println("\n========================================================");
  Serial.println("SENTINEL-GATE: EXHAUSTIVE STATE MACHINE TEST ENVIRONMENT");
  Serial.println("========================================================");
  
  Serial.println("\n--- HARDWARE & SENSOR EVENTS ---");
  Serial.println(" [b] BOOT_COMPLETE");
  Serial.println(" [e] SELF_CHECK_FAILED");
  Serial.println(" [p] PIR_CONFIRMED");
  Serial.println(" [c] PIR_CLEARED");
  Serial.println(" [r] RFID_CARD_PRESENTED");
  
  Serial.println("\n--- CLASSIFICATION EVENTS ---");
  Serial.println(" [y] RFID_AUTHORIZED (Yes)");
  Serial.println(" [n] RFID_UNAUTHORIZED (No)");
  Serial.println(" [u] RFID_UNKNOWN");

  Serial.println("\n--- TIMER EVENTS ---");
  Serial.println(" [w] IDENTIFICATION_WINDOW_EXPIRED");
  Serial.println(" [i] IDENTIFICATION_NOT_ATTEMPTED");
  Serial.println(" [t] CHALLENGE_TIMEOUT");
  Serial.println(" [o] LOCKOUT_EXPIRED");

  Serial.println("\n--- OPERATOR COMMANDS ---");
  Serial.println(" [a] CMD_ARM");
  Serial.println(" [x] CMD_DISARM");
  Serial.println(" [l] CMD_LOCKDOWN");
  Serial.println(" [k] CMD_RELEASE_LOCKDOWN");
  Serial.println(" [z] CMD_RESET");
  Serial.println(" [g] CMD_ACKNOWLEDGE");

  Serial.println("\n--- COMMS & HEARTBEAT EVENTS ---");
  Serial.println(" [f] HEARTBEAT_FAILED");
  Serial.println(" [s] HEARTBEAT_RESTORED");

  Serial.println("\n--- SYSTEM UTILITY ---");
  Serial.println(" [?] PRINT CURRENT STATUS (State & CommsMode)");
  Serial.println("========================================================\n");

  // Kick off the system
  handleEvent(Event::BOOT_COMPLETE);
}

void loop() {
  if (Serial.available() > 0) {
    char incomingChar = Serial.read();

    // Ignore whitespace/newlines
    if (incomingChar == '\n' || incomingChar == '\r' || incomingChar == ' ') {
      return;
    }

    Serial.print("\n>>> Input received: [");
    Serial.print(incomingChar);
    Serial.println("]");

    switch (incomingChar) {
      // Hardware
      case 'b': handleEvent(Event::BOOT_COMPLETE); break;
      case 'e': handleEvent(Event::SELF_CHECK_FAILED); break;
      case 'p': handleEvent(Event::PIR_CONFIRMED); break;
      case 'c': handleEvent(Event::PIR_CLEARED); break;
      case 'r': handleEvent(Event::RFID_CARD_PRESENTED); break;
      
      // Classification
      case 'y': handleEvent(Event::RFID_AUTHORIZED); break;
      case 'n': handleEvent(Event::RFID_UNAUTHORIZED); break;
      case 'u': handleEvent(Event::RFID_UNKNOWN); break;
      
      // Timers
      case 'w': handleEvent(Event::IDENTIFICATION_WINDOW_EXPIRED); break;
      case 'i': handleEvent(Event::IDENTIFICATION_NOT_ATTEMPTED); break;
      case 't': handleEvent(Event::CHALLENGE_TIMEOUT); break;
      case 'o': handleEvent(Event::LOCKOUT_EXPIRED); break;
      
      // Commands
      case 'a': handleEvent(Event::CMD_ARM); break;
      case 'x': handleEvent(Event::CMD_DISARM); break;
      case 'l': handleEvent(Event::CMD_LOCKDOWN); break;
      case 'k': handleEvent(Event::CMD_RELEASE_LOCKDOWN); break;
      case 'z': handleEvent(Event::CMD_RESET); break;
      case 'g': handleEvent(Event::CMD_ACKNOWLEDGE); break;
      
      // Comms
      case 'f': handleEvent(Event::HEARTBEAT_FAILED); break;
      case 's': handleEvent(Event::HEARTBEAT_RESTORED); break;

      // Utility / Status Check
      case '?': 
        Serial.println("--- CURRENT SYSTEM STATUS ---");
        // Note: systemStateToString needs to be accessible, or we just rely on the transition logs.
        // Assuming you have a way to print the current state, otherwise the comms mode is key here:
        Serial.print("CommsMode: ");
        Serial.println(commsModeToString(getCommsMode()));
        Serial.println("-----------------------------");
        break;

      default:
        Serial.println("[ERR] Unknown command character. Press '?' for status.");
        break;
    }
  }
}