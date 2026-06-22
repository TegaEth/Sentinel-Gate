#include "StateMachine.h"

void setup() {
  // Initialize serial communication at a fast baud rate
  Serial.begin(115200);
  
  // Wait for serial port to connect (necessary for some Arduino boards)
  while (!Serial) {
    ; 
  }

  Serial.println("\n=============================================");
  Serial.println("SENTINEL-GATE: STATE MACHINE TEST ENVIRONMENT");
  Serial.println("=============================================");
  Serial.println("Send characters via Serial Monitor to trigger events:");
  Serial.println(" [b] -> BOOT_COMPLETE");
  Serial.println(" [p] -> PIR_CONFIRMED");
  Serial.println(" [c] -> PIR_CLEARED");
  Serial.println(" [r] -> RFID_CARD_PRESENTED");
  Serial.println(" [y] -> RFID_AUTHORIZED (Yes)");
  Serial.println(" [n] -> RFID_UNAUTHORIZED (No)");
  Serial.println(" [u] -> RFID_UNKNOWN");
  Serial.println(" [w] -> IDENTIFICATION_WINDOW_EXPIRED");
  Serial.println(" [l] -> CMD_LOCKDOWN");
  Serial.println(" [k] -> CMD_RELEASE_LOCKDOWN");
  Serial.println(" [x] -> CMD_DISARM");
  Serial.println(" [a] -> CMD_ARM");
  Serial.println(" [f] -> HEARTBEAT_FAILED");
  Serial.println(" [s] -> HEARTBEAT_RESTORED");
  Serial.println("=============================================\n");

  // Kick off the system
  handleEvent(Event::BOOT_COMPLETE);
}

void loop() {
  // Check if you have typed anything into the Serial Monitor
  if (Serial.available() > 0) {
    char incomingChar = Serial.read();

    // Ignore newline and carriage return characters
    if (incomingChar == '\n' || incomingChar == '\r') {
      return;
    }

    Serial.print("\n--- Input received: ");
    Serial.println(incomingChar);

    // Map keyboard inputs to your system events
    switch (incomingChar) {
      case 'b': handleEvent(Event::BOOT_COMPLETE); break;
      case 'p': handleEvent(Event::PIR_CONFIRMED); break;
      case 'c': handleEvent(Event::PIR_CLEARED); break;
      case 'r': handleEvent(Event::RFID_CARD_PRESENTED); break;
      case 'y': handleEvent(Event::RFID_AUTHORIZED); break;
      case 'n': handleEvent(Event::RFID_UNAUTHORIZED); break;
      case 'u': handleEvent(Event::RFID_UNKNOWN); break;
      case 'w': handleEvent(Event::IDENTIFICATION_WINDOW_EXPIRED); break;
      case 'l': handleEvent(Event::CMD_LOCKDOWN); break;
      case 'k': handleEvent(Event::CMD_RELEASE_LOCKDOWN); break;
      case 'x': handleEvent(Event::CMD_DISARM); break;
      case 'a': handleEvent(Event::CMD_ARM); break;
      case 'f': handleEvent(Event::HEARTBEAT_FAILED); break;
      case 's': handleEvent(Event::HEARTBEAT_RESTORED); break;
      
      default:
        Serial.println("[ERR] Unknown command character.");
        break;
    }
  }
}