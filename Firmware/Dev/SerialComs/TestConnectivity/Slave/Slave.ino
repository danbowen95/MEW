/* Test code to ensure coms work 
 *  Network: 
 *              
 *  PC 
 *    \ (USB)
 *     Master (mega) 
 *                  \ (RX,TX,GND)
 *                   Slave 1 (UNO)
 *              
 *  The PC sends a msg: <mode><number>
 *    <mode> is a char   - 'a' = blink once
 *                       - 'b' = blink twice
 *                       - 'c' = blink long
 *    <number> is an int - the number of times the mode is executed 
 *    
 *    This is Relayed to the slave and the command excecuted
 */

// THIS CODE IS FOR SLAVE (UNO) \\

#include "structsSlave.h"

// Function forward declaration
void printCommand(ModeStatus* machineStatus);
void resetModeCommand(ModeStatus * machineStatus);

/// Varibles & Pin assignments
// Structures
ModeStatus machineStatus;      // What mode are we running?

/// Some handy vars:
int LEDPin = 13; // Which pin is the LED attached to

// For serial coms with master Teensy
const unsigned int MAX_MESSAGE_LENGTH = 12; // Max length of serial msg that can be sent
char newValueSerial[12];                    // What the latest serial value
char newModeSerial[3];                      // What the latest serial mode
bool newCommandSerialBool = false;          // Has there been a new command



// Setup
void setup() {
  // initialize LED as an output.
  pinMode(LEDPin, OUTPUT);
  
  // Initialise structures
  resetModeCommand(&machineStatus);

  // Begin serial with Mega
  Serial.begin(9600);
}

// Run
void loop() {
  readSerial();
  processSerial();
  enactCommand();
}
