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
        resetModeCommand(&machineStatus);
        break;
      case 'b':
        // Move A1 an incremental amount.
        A1MoveInc( value , linespeed );
        resetModeCommand(&machineStatus);
        break;
      case 'c':
        // Move A3 to absolute position
        A3MoveAbs( value , linespeed , false );
        resetModeCommand(&machineStatus);
        break;
      case 'd':
        // Move A4 to absolute position
        A4MoveAbs( value , linespeed , false );
        resetModeCommand(&machineStatus);
        break;
      case 'e':
        // Move A6 to absolute position
        A6MoveAbs( value , linespeed , false );
        resetModeCommand(&machineStatus);
        break;
      case 'f':
        // Move A7 to absolute position
        A7MoveAbs( value , linespeed , false );
        resetModeCommand(&machineStatus);
        break;
      case 'g':
        // Move A1 and A3 together, to absolute position of A3
        A3A1MoveAbs( value , linespeed );
        resetModeCommand(&machineStatus);
        break;
      case 'h':
        // Move A1 and A6 together, to absolute position of A6
        A6A1MoveAbs( value , linespeed );
        resetModeCommand(&machineStatus);
        break;
      case 'i':
        // Move A3 and A4 in tangental arc at radius of A3 position
        A3A4Arc( value );
        resetModeCommand(&machineStatus);
        break;
      case 'j':
        // Move A6 and A7 in tangental arc at radius of A7 position
        A6A7Arc( value );
        resetModeCommand(&machineStatus);
        break;
      case 'k':
        // Calibrate A1
        calMotor(1);
        resetModeCommand(&machineStatus);
        break;
      case 'l':
        // Calibrate A3
        calMotor(3);
        resetModeCommand(&machineStatus);
        break;
      case 'm':
        // Calibrate A4
        calMotor(4);
        resetModeCommand(&machineStatus);
        break;
      case 'n':
        // Calibrate A6
        calMotor(6);
        resetModeCommand(&machineStatus);
        break;
      case 'o':
        // Calibrate A7
        calMotor(7);
        resetModeCommand(&machineStatus);
        break;
      case 'p':
        // Home A3
        A3HomeSeq();
        resetModeCommand(&machineStatus);
        break;
      case 'q':
        // Home A4
        A4HomeSeq();
        resetModeCommand(&machineStatus);
        break;
      case 'r':
        // Home A6
        A6HomeSeq();
        resetModeCommand(&machineStatus);
        break;
      case 's':
        // Home A7
        A7HomeSeq();
        resetModeCommand(&machineStatus);
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
