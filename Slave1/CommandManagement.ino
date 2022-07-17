/* In here, we read, process and execute commands from the master controller/PC

       readSerial()
       - When serial is available
         - Read serial msg
         - Record mode in 'newModeSerial'
         - Record value in 'newValueSerial'
         - Flip 'newCommandSerialBool' = true

       processSerial()
       - When 'newCommandSerialBool' = true
         - Populate 'NewCommand' struct
           - 'machineStatus.mode'   = mode
           - 'machineStatus.value'  = newValueSerial * 0.01;
           - 'machineStatus.active' = true
           - 'NewCommand.active'    = true;
         - Flip 'newCommandSerialBool' = false

      enactCommand()
      - When 'NewCommand.active' = true
        - Enact desired command
        - Reset NewCommand
*/




void readSerial() {
  /* Reads in data from serial in
      See - https://www.programmingelectronics.com/serial-read/
  */
  //Check to see if anything is available in the serial receive buffer

  if (Serial1.available()) {
    blinkLED( 5 , 100 ); // Wait for all data to be sent
    while (Serial1.available() > 0)
    {
      //Create a place to hold the incoming newCommandSerial, newModeSerial
      static unsigned int newValueSerial_pos = 0;

      //Read the next available byte in the serial receive buffer
      char inByte = Serial1.read();

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
  /* This processes the newCommandSerial into machineStatus */

  if (newCommandSerialBool == true) {

    // Put into new structure
    machineStatus.mode   = newModeSerial[0];
    machineStatus.value  = atof(newValueSerial) * 0.01;
    machineStatus.active = true;

    // Now done with new Serial command so switch off
    newCommandSerialBool = false;
  }
}

void enactCommand() {
  /* This enacts a command

  */

  if (machineStatus.active == true) {
    char mode = machineStatus.mode;
    float value = machineStatus.value;

//    Serial.print("The mode is: ");
//    Serial.println(mode);
//    Serial.print("The value is: ");
//    Serial.println(value);


    switch (mode) {
      case 'a':
        // idle state - do nothing
        Reset();
        resetModeCommand(&machineStatus);
        break;
      case 'b':
        // Move A2 an incremental amount.
        A2IncMove(value);
        resetModeCommand(&machineStatus);
        break;
      case 'c':
        // Move A5 an incremental amount.
        A5IncMove(value);
        resetModeCommand(&machineStatus);
        break;
      case 'd':
        // Move A8 an incremental amount.
        A8IncMove(value);
        resetModeCommand(&machineStatus);
        break;
      case 'e':
        // Move A2 to abs position
        A2AbsMove(value);
        resetModeCommand(&machineStatus);
        break;
      case 'f':
        // Move A5 to abs position
        A5AbsMove(value);
        resetModeCommand(&machineStatus);
        break;
      case 'g':
        // Move A8 to abs position
        A8AbsMove(value);
        resetModeCommand(&machineStatus);
        break;
      case 'h':
        // Home A2
        A2Home();
        resetModeCommand(&machineStatus);
        break;
      case 'i':
        // Home A5
        A5Home();
        resetModeCommand(&machineStatus);
        break;
      case 'j':
        // Home A8
        A8Home();
        resetModeCommand(&machineStatus);
        break;
      case 'k':
        // A5 active control ON
        A5ActiveControl(false);
        break;
      case 'l':
        // A8 active control ON
        A8ActiveControl(false);
        break;
      case 'm':
        // A5 active control OFF
        // This is its own code as you might want to update at a later point
        Reset();
        active = false;
        resetModeCommand(&machineStatus);
        break;
      case 'n':
        // A8 active control OFF
        // This is its own code as you might want to update at a later point
        Reset();
        active = false;
        resetModeCommand(&machineStatus);
        break;
      case 'o':
        // A5 Down
        A5Down();
        resetModeCommand(&machineStatus);
        break;
      case 'p':
        // A8 Down
        A8Down();
        resetModeCommand(&machineStatus);
        break;
      case 'q':
        // A5 lift
        A5ActiveControl(true);
        break;
      case 'r':
        // A8 lift
        A8ActiveControl(true);
        break;
      case 'y':
        // Debug
        //TempFun();
        resetModeCommand(&machineStatus);
        break;
      case 'z':
        // Blink
        blinkLED( value , 500 );
        resetModeCommand(&machineStatus);
        break;
      default:
        // unrecognised mode
        resetModeCommand(&machineStatus);
        break;
    }
  }
}
