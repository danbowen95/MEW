/* This code recieves a signal from the master Teensy
 *  *  'Xyyyy'
      - X: 'a' = flash once
           'b' = flash twice
           'c' = flash three times
      - Y: <int>, number of loops

*/

// For serial coms with master Teensy
const unsigned int MAX_MESSAGE_LENGTH = 12; // Max length of serial msg that can be sent
char newValueSerial[12];                    // What the latest serial value
char newModeSerial[3];                      // What the latest serial mode
bool newCommandSerialBool = false;          // Has there been a new command


void setup() {
  // Set up coms with Master (Transmitter) Teensy:
  Serial1.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

}




void readSerial() {
  /* Reads in data from serial in
      See - https://www.programmingelectronics.com/serial-read/
  */
  //Check to see if anything is available in the serial receive buffer
  if (Serial1.available()) {
    delay(100); //allows all serial sent to be received together

    while (Serial1.available() > 0)
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
}



void processSerial() {
  /* This processes the newCommandSerial into machineStatus
    //
    //  */

  if (newCommandSerialBool == true) {

    // Put into new structure
    machineStatus.mode   = newModeSerial[0];
    machineStatus.value  = atof(newValueSerial);
    machineStatus.active = true;

    // Print to check
    // printCommand(&machineStatus);

    // Now done with new Serial command so switch off
    newCommandSerialBool = false;
  }
