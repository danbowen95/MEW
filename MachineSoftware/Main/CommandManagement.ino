void readSerial() {
  /* Reads in data from serial in
      - Updates 'newCommandSerial' to what the serial in is
      - Flips 'newCommandSerialBool' to True
      See - https://www.programmingelectronics.com/serial-read/
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
    //Full newCommandSerial received...
    else
    {
      //Add null character to string
      newCommandSerial[newCommandSerial_pos] = '\0';

      //Print the newCommandSerial (or do other things)
      Serial.println("New command recieved: ");
      Serial.println(newCommandSerial);
      newCommandSerialBool = true;

      //Reset for the next newCommandSerial
      newCommandSerial_pos = 0;
    }
  }
}

void processSerial() {
  /* This processes the newCommandSerial into NewCommand

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

  // reset arrays
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
      else if (ch == '-') // space is our separator
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
    // Quite a hack to index correctly.
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

    Serial.print("Doing ");
    Serial.print(GMtype);
    if (n<10) {Serial.print("0");};
    Serial.print(n);

    switch (GMtype) {
      case 'G':
        switch (n) {
          case 0:
            G00();
            break;
          case 1:
            Serial.println("Doing G01");
            G01();
            break;
          case 3:
            Serial.println("Doing G03");
            G03();
            break;
          case 13:
            Serial.println("Doing G13");
            //G13();
            break;
        }
        break;
      case 'M':
        Serial.println("Recognised M");
        switch (n) {
          case 0:
            Serial.println("Doing M00");
            break;
          case 10:
            Serial.println("Doing M10");
            //M10();
            Serial.println("Done M10");
            break;
          case 11:
            Serial.println("Doing M11");
            M11();
            Serial.println("Done M11");
            break;
          case 13:
            Serial.println("Doing M13");
            M13();
            Serial.println("Done M13");
            break;

        }
        break;
    }



    // Reset NewCommand ready for the next
    resetCommand(&NewCommand);

    // Print to check
    printCommand(&NewCommand);
  }
}




















void resetCommand(CommandStruct * NewCommand)
{
  // Reset NewCommand ready for the next
  NewCommand->CommandID = "Mxx";
  NewCommand->X         = 0;
  NewCommand->Y         = 0;
  NewCommand->Z         = 0;
  NewCommand->A         = 0;
  NewCommand->B         = 0;
  NewCommand->active    = false;
}

void printCommand(CommandStruct * NewCommand)
{
  Serial.print("The Command ID: ");
  Serial.println(NewCommand->CommandID);
  Serial.print("X Value: ");
  Serial.println(NewCommand->X);
  Serial.print("Y Value: ");
  Serial.println(NewCommand->Y);
  Serial.print("Z Value: ");
  Serial.println(NewCommand->Z);
  Serial.print("A Value: ");
  Serial.println(NewCommand->A);
  Serial.print("B Value: ");
  Serial.println(NewCommand->B);
  Serial.print("Status: ");
  Serial.println(NewCommand->active);
  Serial.println(" - - - - - - ");
}
