/* In here, we read, process and execute commands from the PC

  Logic: PC --Serial--> Arduino

       readSerial()
       - When serial is available
         - Read serial msg and store in 'newCommandSerial'
         - Flip 'newCommandSerialBool' = true

       processSerial()
       - When 'newCommandSerialBool' = true
         - Populate 'NewCommand' struct
           - 'NewCommand.CommandID' = G or M
           - 'NewCommand.X'         = float
           - 'NewCommand.Y'         = float
           - ...
           - 'NewCommand.active'    = true;
         - Flip 'newCommandSerialBool' = false

      enactCommand()
      - When 'NewCommand.active' = true
        - Enact desired command
        - Reset NewCommand
*/


void readSerial() {
  /* Reads in data from PC serial
      - Updates 'newCommandSerial' to what the serial in is
      - Flips 'newCommandSerialBool' to True
      - See: https://www.programmingelectronics.com/serial-read/
  */
  //Check to see if anything is available in the serial receive buffer
  while (Serial.available() > 0)
  {
    //Create a place to hold the incoming newCommandSerial
    //    newCommandSerial[MAX_MESSAGE_LENGTH];
    static unsigned int newCommandSerial_pos = 0;

    //Read the next available byte in the serial receive buffer
    char inByte = Serial.read();

    //Message coming in (check not terminating character) and guard for over newCommandSerial size
    if ( inByte != '\n' && (newCommandSerial_pos < MAX_MESSAGE_LENGTH - 1) )
    {
      //Add the incoming byte to our newCommandSerial
      newCommandSerial[newCommandSerial_pos] = inByte;
      newCommandSerial_pos++;
    }
    else
    {
      //Full newCommandSerial received...
      //Add null character to string
      newCommandSerial[newCommandSerial_pos] = '\0';

      //Print the newCommandSerial if debug
      if (debug) {
        Serial.println("New command recieved: ");
        Serial.println(newCommandSerial);
      }
      newCommandSerialBool = true;

      //Reset for the next newCommandSerial
      newCommandSerial_pos = 0;
    }
  }
}


void processSerial() {
  /* This processes the newCommandSerial into NewCommand
      - This just works, it's 100% a hack, but it works, reliably so we're good!

  */

  // Define some vars just for this function:
  int n;
  int signVals[7];
  int cmdidx;
  float val = 0;
  float valTemp;
  float storeVals[7];
  char GMtype;
  bool ptReached = false;

  // reset arrays - Not sure is this is required?
  for (int i = 1; i <= 7; i++) {
    storeVals[i] = 0;
    signVals[i]  = 1;
  }


  if (newCommandSerialBool == true) {
    for (int i = 0; i <= MAX_MESSAGE_LENGTH; i++) {

      /// Extract char to analyse
      char ch = newCommandSerial[i];

      /// Allocate char to rightful place
      if (ch >= '0' && ch <= '9') // Digit between 0 and 9
      {
        valTemp = ch - '0'; // char to int

        // insert to store vals (this is a super neat bitta code...)
        if (ptReached == true) {
          // RHS of DP
          storeVals[cmdidx] = storeVals[cmdidx] + (valTemp * pow(10, n));
          --n;
        }
        else {
          // LHS of DP
          storeVals[cmdidx] = (storeVals[cmdidx] * 10) + valTemp;
        }
      }
      else if (ch == ' ') // space is our separator
      {
        // Do nothing
      }
      else if (ch == '-')
      {
        signVals[cmdidx] = -1;
      }
      else if (ch == '.') // decimal point
      {
        ptReached = true;
        n = -1;
      }
      else if (ch == 'G' || 'M' || 'X' || 'Y' || 'Z' || 'A' || 'B' )
      {
        val = 0;
        n   = -1;
        ptReached = false;
        switch (ch) {
          case 'G':
            cmdidx = 1;
            GMtype = 'G';
            break;
          case 'M':
            cmdidx = 1;
            GMtype = 'M';
            break;
          case 'X':
            cmdidx = 2;
            break;
          case 'Y':
            cmdidx = 3;
            break;
          case 'Z':
            cmdidx = 4;
            break;
          case 'A':
            cmdidx = 5;
            break;
          case 'B':
            cmdidx = 6;
            break;
          default:
            // Imposible to get here
            break;
        }
      }

      // Empty the array ready for next cmd, as it is a global var
      newCommandSerial[i] = ' ';
    }

    // Now go thru and update command structure
    NewCommand.CommandID = GMtype + String(int((storeVals[1])));
    NewCommand.X         = storeVals[2] * signVals[2];
    NewCommand.Y         = storeVals[3] * signVals[3];
    NewCommand.Z         = storeVals[4] * signVals[4];
    NewCommand.A         = storeVals[5] * signVals[5];
    NewCommand.B         = storeVals[6] * signVals[6];
    NewCommand.active    = true;

    // Print to check
    printCommand(&NewCommand);

    // Now done with new Serial command so switch off
    newCommandSerialBool = false;
  }
}


void enactCommand() {
  /*
    Here, we take the command and enact it
  */
  if (NewCommand.active == true) {

    // Hack to index correctly.
    String commandIDString = NewCommand.CommandID;
    char commandIDchar[4];
    commandIDString.toCharArray(commandIDchar, 4);
    int  n;
    char buffer[3];
    buffer[0] = commandIDchar[1];
    buffer[1] = commandIDchar[2];
    buffer[2] = commandIDchar[3];
    buffer[3] = '\0';
    n = atoi(buffer);
    char GMtype = commandIDString[0];

    if (debug) {
      Serial.print("Enacting ");
      Serial.print(GMtype);
      if (n < 10) {
        Serial.print("0");
      };
      Serial.println(n);
    }

    // Enact commands:
    switch (GMtype) {
      case 'G':
        switch (n) {
          case 0:
            G00();
            break;
          case 1:
            G01();
            break;
          case 2:
            G02();
            break;
          case 3:
            G03();
            break;
          case 4:
            G04();
            break;
          case 5:
            G05();
            break;
          case 6:
            G06();
            break;
          case 7:
            G07();
            break;
          case 8:
            G08();
            break;
          case 12:
            G12();
            break;
          case 13:
            G13();
            break;
          case 14:
            G14();
            break;
          case 15:
            G15();
            break;
          case 16:
            G16();
            break;
          case 17:
            G17();
            break;
          case 18:
            G18();
            break;
          case 21:
            G21();
            break;
          case 22:
            G22();
            break;
          case 23:
            G23();
            break;
          case 24:
            G24();
            break;
          case 25:
            G25();
            break;
          case 26:
            G26();
            break;
          case 28:
            G28();
            break;
          case 31:
            G31();
            break;
          case 32:
            G32();
            break;
          case 33:
            G33();
            break;
          case 34:
            G34();
            break;
          case 35:
            G35();
            break;
          case 36:
            G36();
            break;
          case 37:
            G37();
            break;
          case 38:
            G38();
            break;
          case 40:
            G40();
            break;
          case 42:
            G42();
            break;
          case 43:
            G43();
            break;
          case 44:
            G44();
            break;
          case 45:
            G45();
            break;
          case 46:
            G46();
            break;
          case 47:
            G47();
            break;
          case 48:
            G48();
            break;
          case 50:
            G50();
            break;
          default:
            notRecognised();
            break;
        }
        break;
      case 'M':
        switch (n) {
          case 0:
            M00();
            break;
          case 1:
            M01();
            break;
          case 2:
            M02();
            break;
          case 3:
            M03();
            break;
          case 4:
            M04();
            break;
          case 5:
            M05();
            break;
          case 6:
            M06();
            break;
          case 7:
            M07();
            break;
          case 8:
            M08();
            break;
          case 9:
            M09();
            break;
          case 10:
            M10();
            break;
          case 11:
            M11();
            break;
          case 13:
            M13();
            break;
          case 14:
            M14();
            break;
          case 16:
            M16();
            break;
          case 17:
            M17();
            break;
        }
        break;
    }

    if (debug) {
      Serial.print("Command complete");
    }
    else
    {
      Serial.print("1");
    }

    // Reset NewCommand ready for the next
    resetCommand(&NewCommand);

    // Print to check
    // printCommand(&NewCommand);
  }
}
