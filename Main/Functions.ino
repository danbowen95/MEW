/*   - - - - - - - - - - - - - - - - -
    | FUNCTIONS THAT DO USEFUL THINGS |
     - - - - - - - - - - - - - - - - -

  Here, you will find the following:
   Move Functions:
      A1Move - Moves A1 an incremental amount
      A2Move - Moves A2 to absolute position
      A3Move - Moves A3 to absolute position
      A4Move - Moves A4 to absolute position
      A5Move - Moves A5 to absolute position
      A6Move - Moves A6 to absolute position
      A7Move - Moves A7 to absolute position
      A8Move - Moves A8 to absolute position
      A1A3Move - Moves A3 to absolute position whilst moving A1 synchronously tracks
      A1A4Move - Moves A4 to absolute position whilst moving A1 synchronously tracks
      A3A4Arc  - Moves A3 and A4 in arc
      A6A7Arc  - Moves A6 and A7 in arc

   Other Functions:
      cmd2Slave       - sends message to slave teensy & waits for responce
      vacuumControl   - Turns on/off A5/A8 vacuum
   Checking Functions:
      axisHome   - is the axis homed (bool)
      axisCal    - is the axis calibrated (bool)
      withinLims - is abs position within lims (bool)
   Print Functions:
      printMachineCodes - Prints what machine codes do what
      printMachineStatus - Prints the status of each axis
   Other:
      roundNum      - rounds numbers to specified dp
      printCommand  - Prints NewCommand Struct
      resetCommand  - Resets NewCommand
      notRecognised - When command i snot recognised
      min2rad       - convert minute to radian
      updateArc     - updates positions folling arc move

*/


// - - - - - - Move Functions - - - - - - //

void A1Move( float Pos ) {
  /* Move A1 an incremental amount */

  // Send command
  cmd2Slave( 2 , 'b' , Pos);
}

void A2Move( float Pos ) {
  /* Move A2 to absolute position */

  // Send command
  cmd2Slave( 1 , 'e' , Pos);

  // Update position
  A2Pos = Pos;
}

void A3Move( float Pos ) {
  /* Move A3 to absolute position */

  // Send command
  cmd2Slave( 2 , 'c' , Pos);

  // Update position
  A3Pos = Pos;
}

void A4Move( float Pos ) {
  /* Move A3 to absolute position */

  // Send command
  cmd2Slave( 2 , 'd' , Pos);

  // Update position
  A4Pos = Pos;
}

void A5Move( float Pos ) {
  /* Move A2 to absolute position */

  // Send command
  cmd2Slave( 1 , 'f' , Pos);

  // Update position
  A5Pos = Pos;
}

void A6Move( float Pos ) {
  /* Move A3 to absolute position */

  // Send command
  cmd2Slave( 2 , 'e' , Pos);

  // Update position
  A6Pos = Pos;
}

void A7Move( float Pos ) {
  /* Move A3 to absolute position */

  // Send command
  cmd2Slave( 2 , 'f' , Pos);

  // Update position
  A7Pos = Pos;
}

void A8Move( float Pos ) {
  /* Move A2 to absolute position */

  // Send command
  cmd2Slave( 1 , 'g' , Pos);

  // Update position
  A5Pos = Pos;
}

void A1A3Move( float Pos ) {
  /* Move A3 to absolute position */

  // Send command
  cmd2Slave( 2 , 'g' , Pos);

  // Update position
  A3Pos = Pos;
}

void A1A6Move( float Pos ) {
  /* Move A3 to absolute position */

  // Send command
  cmd2Slave( 2 , 'h' , Pos);

  // Update position
  A6Pos = Pos;
}

void A3A4Arc( float angle ) {
  /* Arc move A3 and A4 */

  // Send command
  cmd2Slave( 2 , 'i' , angle);

  // Update position
  updateArc( angle , 1 );
}

void A6A7Arc( float angle ) {
  /* Arc move A6 and A7 */

  // Send command
  cmd2Slave( 2 , 'j' , angle );

  // Update position
  updateArc( angle , 2 );
}


void cmd2Slave ( int serialLine , char mode , float val ) {
  /* This sends a string (msg) to the slave controller and waits for the okay to continue
      - Slave is on Serial1.
      - msg is "mXYYY.YYY\0"
        - m is the mode
        - X is either '0' or '-'
        - YYY.YYY is the float value
      - bool 'Done' says if the command has been carried out
  */

  char  bufferSlave[11];    // Buffer big enough for 7-character float
  char tempBuf;
  bool Done = false;   // Has command been enacted
  float sendval;

  // Clear buffer - not sure if this is necessary
  //memset(0, buffer, sizeof(buffer));
  for (int i = 0; i < 11; i++) {
    bufferSlave[i] = '0';
  };

  //  For some reason, the serial line to the teensy's no not like
  //  floating point numbers, unlike the serial line between the PC
  //  and the master teensy. So what we do is multiply by 100 on
  //  the master end, then divide by 100 on the slave end. HACK.
  sendval = val * 100;

  if (debug) {
    Serial.print("val is: ");
    Serial.print(val);
    Serial.print(" , sendval is: ");
    Serial.println(sendval);
  }

  // float to char array: buffer
  dtostrf(sendval, -8, 0, bufferSlave);

  String str1 = String(mode);
  String str2 = bufferSlave;
  String str3 = "\0";

  String msg = str1 + str2 + str3;

  if (debug) {
    Serial.print("The message is: ");
    Serial.println(msg);
  }
  
  switch ( serialLine ) {
    case 1:
      // Send to slave 1
      Serial1.println(msg);  // send data
      Serial1.flush();       // make sure data is sent
      delay(500);

      if (debug) {
        Serial.print("Data sent!");
      }

      // Wait to recieve 'completion'
      while (!Done) {
        if (debug) {
          Serial.println("Waiting...");
        }
        delay(500);
        if (Serial1.available()) {
          while (Serial1.available()) {
            // Hack to empty the serial
            tempBuf = Serial1.read();
          }
          delay(100);
          Done = true; // exit loop
        }
      }
      break;
    case 2:
      // Send to slave 2
      Serial2.println(msg);  // send data
      Serial2.flush();       // make sure data is sent
      delay(500);

      if (debug) {
        Serial.print("Data sent to Serial2");
      }

      // Wait to recieve 'completion'
      while (!Done) {
        if (debug) {
          Serial.println("Waiting...");
        }
        delay(500);
        if (Serial2.available()) {
          while (Serial2.available()) {
            // Hack to empty the serial
            tempBuf = Serial2.read();
          }
          delay(100);
          Done = true; // exit loop
        }
      }
      break;
  }

  if (debug) {
    Serial.println("DONE!");
    Serial.print("Last char is: ");
    Serial.println(tempBuf);
  }
}

void VacControl( int axis , bool mode ) {

  switch (axis) {
    case 5:
      if (mode) {
        A5Vac = true;
        digitalWrite(A5VacPin, LOW);
      }
      else {
        A5Vac = false;
        digitalWrite(A5VacPin, HIGH);
      }
      break;
    case 8:
      if (mode) {
        A8Vac = true;
        digitalWrite(A8VacPin, LOW);
      }
      else {
        A8Vac = false;
        digitalWrite(A8VacPin, HIGH);
      }
      break;
  }
}



// - - - - - - Checking Functions - - - - - - //

bool axisHome( int axis ) {
  /* This checks if axis is homed
   *    * 0 - axis is not homed
   *    * 1 - axis is homed
  */

  switch (axis) {
    case 2:
      if (A2Home) {
        return true;
      }
      else {
        return false;
      }
      break;
    case 3:
      if (A3Home) {
        return true;
      }
      else {
        return false;
      }
      break;
    case 4:
      if (A4Home) {
        return true;
      }
      else {
        return false;
      }
      break;
    case 5:
      if (A5Home) {
        return true;
      }
      else {
        return false;
      }
      break;
    case 6:
      if (A6Home) {
        return true;
      }
      else {
        return false;
      }
      break;
    case 7:
      if (A7Home) {
        return true;
      }
      else {
        return false;
      }
      break;
    case 8:
      if (A8Home) {
        return true;
      }
      else {
        return false;
      }
      break;
    default:
      return false;
      if (debug) {
        Serial.println("Numpty");
      }
      break;
  }
}

bool axisCal( int axis ) {
  /* This checks if axis is calibrated (odrive)
   *    * 0 - axis is not calibrated
   *    * 1 - axis is calibrated
  */

  switch (axis) {
    case 1:
      if (A1Cal) {
        return true;
      }
      else {
        return false;
      }
      break;
    case 3:
      if (A3Cal) {
        return true;
      }
      else {
        return false;
      }
      break;
    case 4:
      if (A4Cal) {
        return true;
      }
      else {
        return false;
      }
      break;
    case 6:
      if (A6Cal) {
        return true;
      }
      else {
        return false;
      }
      break;
    case 7:
      if (A7Cal) {
        return true;
      }
      else {
        return false;
      }
      break;
    default:
      return false;
      if (debug) {
        Serial.println("Numpty");
      }
      break;
  }
}


bool withinLims( int axis , float Pos ) {
  /* This checks the lims of an axis
   *    * 0 - Pos is outside of lims
   *    * 1 - Pos is inside of lims
  */

  switch (axis) {
    case 2:
      if (Pos < A2MinPos || Pos >= A2MaxPos) {
        return false;
      }
      else {
        return true;
      }
      break;
    case 3:
      if (Pos < A3MinPos || Pos >= A3MaxPos) {
        return false;
      }
      else {
        return true;
      }
      break;
    case 4:
      if (Pos < A4MinPos || Pos >= A4MaxPos) {
        return false;
      }
      else {
        return true;
      }
      break;
    case 5:
      if (Pos < A5MinPos || Pos >= A5MaxPos) {
        return false;
      }
      else {
        return true;
      }
      break;
    case 6:
      if (Pos < A6MinPos || Pos >= A6MaxPos) {
        return false;
      }
      else {
        return true;
      }
      break;
    case 7:
      if (Pos < A7MinPos || Pos >= A7MaxPos) {
        return false;
      }
      else {
        return true;
      }
      break;
    case 8:
      if (Pos < A8MinPos || Pos >= A8MaxPos) {
        return false;
      }
      else {
        return true;
      }
      break;
    default:
      return false;
      if (debug) {
        Serial.println("Numpty");
      }
      break;
  }
}














/// Print Functions
void printMachineCodes() {
  if (debug) {
    Serial.println("Help Codes: ");
    Serial.println("G00: Print help messages(s)");
    Serial.println("G01: Move A1 incrementally");
    Serial.println("  X<float>: increment distance");
    Serial.println("G02: Move A2 incrementally");
    Serial.println("  X<float>: increment distance");
    Serial.println("G03: Move A3 incrementally");
    Serial.println("  X<float>: increment distance");
    Serial.println("G04: Move A4 incrementally");
    Serial.println("  X<float>: increment distance");
    Serial.println("G05: Move A5 incrementally");
    Serial.println("  X<float>: increment distance");
    Serial.println("G06: Move A6 incrementally");
    Serial.println("  X<float>: increment distance");
    Serial.println("G07: Move A7 incrementally");
    Serial.println("  X<float>: increment distance");
    Serial.println("G08: Move A8 incrementally");
    Serial.println("  X<float>: increment distance");

    Serial.println("G12: Move A2 to absolute position");
    Serial.println("  X<float>: position");
    Serial.println("G13: Move A3 to absolute position");
    Serial.println("  X<float>: position");
    Serial.println("G14: Move A4 to absolute position");
    Serial.println("  X<float>: position");
    Serial.println("G15: Move A5 to absolute position");
    Serial.println("  X<float>: position");
    Serial.println("G16: Move A6 to absolute position");
    Serial.println("  X<float>: position");
    Serial.println("G17: Move A7 to absolute position");
    Serial.println("  X<float>: position");
    Serial.println("G18: Move A8 to absolute position");
    Serial.println("  X<float>: position");

    Serial.println("G21: Move A3 an incremental amount, moving A1 synchronously");
    Serial.println("  X<float>: increment distance");
    Serial.println("G22: Move A3 to absolute position, moving A1 synchronously");
    Serial.println("  X<float>: position");
    Serial.println("G23: Move A6 an incremental amount, moving A1 synchronously");
    Serial.println("  X<float>: increment distance");
    Serial.println("G24: Move A6 to absolute position, moving A1 synchronously");
    Serial.println("  X<float>: position");
    Serial.println("G25: Move A3 and A4 in arc, of radius A3 to abs theta position");
    Serial.println("  X<float>: theta");
    Serial.println("G26: Move A6 and A7 in arc, of radius A3 to abs theta position");
    Serial.println("  X<float>: theta");

    Serial.println("G31: Raise A5, then AC");
    Serial.println("G32: Raise A8, then AC");
    Serial.println("G33: A5 shift down 50, or to bottom");
    Serial.println("G34: A8 shift down 50, or to bottom");
    Serial.println("G35: A5 adaptive ON");
    Serial.println("G36: A5 adaptive OFF");
    Serial.println("G37: A8 adaptive ON");
    Serial.println("G38: A8 adaptive OFF");

    Serial.println("G42: Home A2");
    Serial.println("G43: Home A3");
    Serial.println("G44: Home A4");
    Serial.println("G45: Home A5");
    Serial.println("G46: Home A6");
    Serial.println("G47: Home A7");
    Serial.println("G48: Home A8");

    Serial.println("M00: Print machine status");
    Serial.println("M01: Print all seperate axis status");
    Serial.println("M02: Print individual axis status");
    Serial.println("  X<int>: axis no");
    Serial.println("M03: Print status of active control");
    Serial.println("M04: Print vacuum status");
    Serial.println("M05: Calibrate and home all axes");
    Serial.println("M06: A5 Vacuum ON");
    Serial.println("M07: A5 Vacuum OFF");
    Serial.println("M08: A8 Vacuum ON");
    Serial.println("M09: A5 Vacuum OFF");
    Serial.println("M10: Calibrate A1 A3 A4 A6 A7");
    Serial.println("M11: Calibrate A1");
    Serial.println("M13: Calibrate A3");
    Serial.println("M14: Calibrate A4");
    Serial.println("M16: Calibrate A6");
    Serial.println("M17: Calibrate A7");
  }
}

void printActiveStatus() {
  /* Prints status of Vacuum */
  if (debug) {

  Serial.println("Active control status:");
  Serial.print("  A5: ");
  //
  if (A5Active) {
    Serial.println("ON");
  }
  else {
    Serial.println("OFF");
  }
  //
  Serial.print("  A8: ");
  if (A8Active) {
    Serial.println("ON");
  }
  else {
    Serial.println("OFF");
  }
  }
}

void printVacStatus() {
  /* Prints status of Vacuum */
  if (debug) {
  Serial.println("Vacuum status:");
  Serial.print("  A5 Vacuum: ");
  //
  if (A5Vac) {
    Serial.println("ON");
  }
  else {
    Serial.println("OFF");
  }
  //
  Serial.print("  A8 Vacuum: ");
  if (A8Vac) {
    Serial.println("ON");
  }
  else {
    Serial.println("OFF");
  }
}
}

void printAxisStatus( int axis ) {
  /* This prints the status of specified axis
       Here we demonstrate why arrays should be used
       and why I am not a programmer*/
       if (debug) {

  Serial.print("Axis ");
  Serial.println(axis);

  // Calibration
  Serial.print("  Calibration: ");
  switch (axis) {
    case 1:
      if (A1Cal) {
        Serial.println("True");
      }
      else {
        Serial.println("False");
      }
      break;
    case 2:
      Serial.println("NA");
      break;
    case 3:
      if (A3Cal) {
        Serial.println("True");
      }
      else {
        Serial.println("False");
      }
      break;
    case 4:
      if (A4Cal) {
        Serial.println("True");
      }
      else {
        Serial.println("False");
      }
      break;
    case 5:
      Serial.println("NA");
      break;
    case 6:
      if (A6Cal) {
        Serial.println("True");
      }
      else {
        Serial.println("False");
      }
      break;
    case 7:
      if (A7Cal) {
        Serial.println("True");
      }
      else {
        Serial.println("False");
      }
      break;
    case 8:
      Serial.println("NA");
      break;
    default:
      Serial.println(" Numpty");
      break;
  }

  // Home
  Serial.print("  Home: ");
  switch (axis) {
    case 1:
      Serial.println("NA");
      break;
    case 2:
      if (A2Home) {
        Serial.println("True");
      }
      else {
        Serial.println("False");
      }
      break;
    case 3:
      if (A3Home) {
        Serial.println("True");
      }
      else {
        Serial.println("False");
      }
      break;
    case 4:
      if (A4Home) {
        Serial.println("True");
      }
      else {
        Serial.println("False");
      }
      break;
    case 5:
      if (A5Home) {
        Serial.println("True");
      }
      else {
        Serial.println("False");
      }
      break;
    case 6:
      if (A6Home) {
        Serial.println("True");
      }
      else {
        Serial.println("False");
      }
      break;
    case 7:
      if (A7Home) {
        Serial.println("True");
      }
      else {
        Serial.println("False");
      }
      break;
    case 8:
      if (A8Home) {
        Serial.println("True");
      }
      else {
        Serial.println("False");
      }
      break;
    default:
      Serial.println(" Numpty");
      break;
  }


  // Position
  Serial.print("  Position (mm): ");
  switch (axis) {
    case 1:
      Serial.println("NA");
      break;
    case 2:
      Serial.println(A2Pos);
      break;
    case 3:
      Serial.println(A3Pos);
      break;
    case 4:
      Serial.println(A4Pos);
      break;
    case 5:
      Serial.println(A5Pos);
      break;
    case 6:
      Serial.println(A6Pos);
      break;
    case 7:
      Serial.println(A7Pos);
      break;
    case 8:
      Serial.println(A8Pos);
      break;
    default:
      Serial.println(" Numpty");
      break;
  }
}
}


void printCommand(CommandStruct * NewCommand)
{
  if (debug) {
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
}

/// Other
float roundNum( float floatIn , int nDp ) {
  /* Round number to nDp */

  float NumOut;
  int Temp;
  float Add;

  floatIn = 10 * (floatIn + (5 * pow(10, (-1 * (nDp + 1)))));
  Temp = (int)floatIn;
  NumOut = (float)Temp / (pow(10, nDp));

  return NumOut;
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





void notRecognised() {
  if (debug) {Serial.println("Command not recognised");}
}


float min2Rad( float minIn ) {
  // Convert minute to radian
  float radOut;
  return radOut = (3.141 / (60 * 180)) * minIn;
}


void updateArc( float theta , int axis ) {
  /* Updates position following arc move */

  // theta should be in min
  theta = min2Rad( theta );
  float R;

  switch (axis) {
    case 1:
      R = sqrt( (A3Pos * A3Pos) + (A4Pos * A4Pos) );
      A4Pos = R * sin(theta);
      A3Pos = R * cos(theta);
      break;
    case 2:
      R = sqrt( (A6Pos * A6Pos) + (A7Pos * A7Pos) );
      A7Pos = R * sin(theta);
      A6Pos = R * cos(theta);
      break;
    default:
      // statements
      break;
  }
}
