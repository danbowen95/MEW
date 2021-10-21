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

    // Put into new structure
    machineStatus.mode   = newModeSerial[0];
    machineStatus.value  = atof(newValueSerial);
    machineStatus.active = true;

    // Print to check
    // printCommand(&machineStatus);

    // Now done with new Serial command so switch off
    newCommandSerialBool = false;
  }
}

void enactCommand() {
  /* This enacts a command

  */
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
      resetModeCommand(&machineStatus);
      break;
    case 'c':
      //to be run continuously
      Serial.println("Doing c");
      delay(500);
      resetModeCommand(&machineStatus);
      break;
    case 'd':
      delay(200);
      Serial.print("Value = ");
      Serial.println(value);
      break;
    case 'h':
      // Home A2
      homeA2();
      Serial.println("A2 homed");
      break;
    case 'x':
      moveup();
      resetModeCommand(&machineStatus);
      break;
    case 'y':
      movedown();
      resetModeCommand(&machineStatus);
      break;
  }
}

void resetModeCommand(ModeStatus * machineStatus)
{
  // Reset NewCommand ready for the next
  machineStatus->mode   = 'a';
  machineStatus->value  = 0;
  machineStatus->active = 1;
}

void initializeSlaveMachineStatus(slaveStatusStruct * slaveStatus)
{
  // initialize Slavestatus
  slaveStatus->A2Home = false;   // Has A2 been homed
  slaveStatus->A3Home = false;   // Has A2 been homed
  slaveStatus->A5Home = false;   // Has A2 been homed
  slaveStatus->A2Pos  = 0;
  slaveStatus->A5Pos  = 0;
  slaveStatus->A8Pos  = 0;
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
