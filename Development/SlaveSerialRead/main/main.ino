/* To be uploaded to slave teensy

    Commands:
      - a,    Do nothing
      - bxxx, move A5 incrementally (mm)
      - cxxx, move A8 incrementally (mm)
      - dxxx, move A5 to absolute position (mm)
      - exxx, move A8 to absolute position (mm)
      - f, A5 active control on
      - g, A5 active control off
      - h, A8 active control on
      - i, A8 active control off
      - j, home A5
      - k, home A8
      - l, home A2
      - m, move A2 (see docs)

       These are to be run continuously
*/
#include "structsSlave.h"

// Function forward declaration
void printCommand(ModeStatus* machineStatus);
void resetModeCommand(ModeStatus * machineStatus);

/// Varibles

ModeStatus machineStatus; // New Structure

// For serial coms with PC
const unsigned int MAX_MESSAGE_LENGTH = 12; // Max length of serial
char newValueSerial[12];                    // What the latest serial value
char newModeSerial[3];                      // What the latest serial mode
bool newCommandSerialBool = false;

/// Set up
void setup() {
  Serial.begin(9600);

  // Initialise ModeStatus structure
  resetModeCommand(&machineStatus);

}

void loop() {
  readSerial();

  processSerial();

  enactCommand();
}




void readSerial() {
  /* Reads in data from serial in
      See - https://www.programmingelectronics.com/serial-read/
  */
  //Check to see if anything is available in the serial receive buffer
  while (Serial.available() > 0)
  {
    //Create a place to hold the incoming newCommandSerial, newModeSerial
    static unsigned int newValueSerial_pos = 0;

    //Read the next available byte in the serial receive buffer
    char inByte = Serial.read();

    if (newValueSerial_pos == 0)
    {
      newModeSerial[0] = inByte;
      newModeSerial[1] = '\0';

      newValueSerial_pos++;
    }
    //Message coming in (check not terminating character) and guard for over newCommandSerial size
    else if ( inByte != '\n' && (newValueSerial_pos < MAX_MESSAGE_LENGTH - 1) )
    {
      //Add the incoming byte to our newCommandSerial
      newValueSerial[newValueSerial_pos - 1] = inByte;
      newValueSerial_pos++;
    }
    //Full newCommandSerial received...
    else
    {
      //Add null character to string
      newValueSerial[newValueSerial_pos - 1] = '\0';

      newCommandSerialBool = true;

      //Reset for the next newCommandSerial
      newValueSerial_pos = 0;
    }
  }
}


void processSerial() {
  /* This processes the newCommandSerial into machineStatus
    //
    //  */

  if (newCommandSerialBool == true) {

    //resetModeCommand(&machineStatus);

    // Put into new structure
    machineStatus.mode   = newModeSerial[0];
    machineStatus.value  = atof(newValueSerial);
    machineStatus.active = true;

    // Print to check
    printCommand(&machineStatus);

    // Now done with new Serial command so switch off
    newCommandSerialBool = false;
  }
}




void enactCommand() {

  
  char mode = machineStatus.mode;
  float value = machineStatus.value;
  
  switch (mode) {
    case 'a':
      //do nothing
      break;
    case 'b':
      //to be run continuously
      Serial.println("Doing b");
      delay(20);
      break;
    case 'c':
      //to be run continuously
      Serial.println("Doing c");
      delay(500);
      break;
    case 'd':
      delay(200);
      Serial.print("Value = ");
      Serial.println(value);
      break;

  }
}

void printCommand(ModeStatus * machineStatus)
{
  Serial.print("The mode: ");
  Serial.println(machineStatus->mode);
  Serial.print("The value: ");
  Serial.println(machineStatus->value);
  Serial.print("active: ");
  Serial.println(machineStatus->active);
  Serial.println(" - - - - - - ");
}

void resetModeCommand(ModeStatus * machineStatus)
{
  // Reset NewCommand ready for the next
  machineStatus->mode   = 'a';
  machineStatus->value  = 0;
  machineStatus->active = 1;
}

void doneSignal() {
  /* Send signal to say we're done! /*

  */

  Serial.println(" - - - - - - ");
  Serial.println("|    DONE   |");
  Serial.println(" - - - - - - ");
}
