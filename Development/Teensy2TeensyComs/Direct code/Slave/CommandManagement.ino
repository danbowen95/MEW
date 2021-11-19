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

//

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
      // idle state
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
      //A8IncMove(value);
      resetModeCommand(&machineStatus);
      break;
    case 'e':
      // Move A2 to abs position
      A2AbsMove(value);
      resetModeCommand(&machineStatus);
      break;
    case 'f':
      // Move A5 to abs position
      A5Move(value);
      resetModeCommand(&machineStatus);
      break;
    case 'g':
      // Move A8 to abs position
      //A8Move(value);
      resetModeCommand(&machineStatus);
      break;
    case 'h':
      // Home A2
      homeA2();
      resetModeCommand(&machineStatus);
      break;
    case 'i':
      // Home A5
      homeA5();
      resetModeCommand(&machineStatus);
      break;
    case 'j':
      // Home A8
      //homeA8();
      resetModeCommand(&machineStatus);
      break;
    case 'k':
      // A5 active control ON
      A5ActiveControl();
      break;
    case 'l':
      // A8 active control ON
      // A8ActiveControl();
      break;
    case 'm':
      A2AbsMove(11.3);
      resetModeCommand(&machineStatus);
      break;
    case 'x':
      moveup();
      resetModeCommand(&machineStatus);
      break;
    case 'y':
      movedown();
      resetModeCommand(&machineStatus);
      break;
    case 'z':
      //shutdownSeq();
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
  //
  A5Cal = false;
  DriveToSpeed( 0 , 5 );

}

void initializeSlaveMachineStatus(slaveStatusStruct * slaveStatus)
{
  // initialize Slavestatus
  slaveStatus->A2Home = false;   // Has A2 been homed
  slaveStatus->A5Home = false;   // Has A5 been homed
  slaveStatus->A8Home = false;   // Has A8 been homed
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



void printPos(slaveStatusStruct * slaveStatus)
{
  Serial.println("Positions of Axes");
  Serial.println("");
  if (slaveStatus->A2Home) {
    Serial.print("A2 encoder position: ");
    Serial.print(slaveStatus->A2Pos);
    Serial.print(" (counts), A2 actual position: ");
    Serial.print(slaveStatus->A2Pos * A2_cpmm);
    Serial.println(" (mm)");
  }
  else {
    Serial.println("A2 not homed!");
  }
  if (slaveStatus->A5Home) {
    Serial.print("A5 encoder position: ");
    Serial.print(slaveStatus->A5Pos);
    Serial.print(" (counts), A2 actual position: ");
    Serial.print(slaveStatus->A5Pos * A2_cpmm);
    Serial.println(" (mm)");
  }
  else {
    Serial.println("A5 not homed!");
  }
  if (slaveStatus->A8Home) {
    Serial.print("A8 encoder position: ");
    Serial.print(slaveStatus->A8Pos);
    Serial.print(" (counts), A8 actual position: ");
    Serial.print(slaveStatus->A8Pos * A2_cpmm);
    Serial.println(" (mm)");
  }
  else {
    Serial.println("A8 not homed!");
  }
}
