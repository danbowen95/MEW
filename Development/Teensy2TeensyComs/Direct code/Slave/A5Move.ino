/* This contains script for:
    - Incrementally move A2 an amount (mm)
    - Move A2 to absolutle position
    - A2 Homing sequence
*/


void homeA5() {
  /* Homes A5 */

  int A5HomeSpeed = 40;
  int StaticVal;
  int readVal;
  int readTol = 20;
  bool Home = false;

  // Serial.println("Homing A5");
  delay(500);
  // Serial.println("Here we go...");

  StaticVal = A5AvgSensorVal( 100 );
  // Serial.print("Static Val = ");
  // Serial.println(StaticVal);

  DriveToSpeed( A5HomeSpeed , 5 );

  while (Home == false) {
    readVal = A5AvgSensorVal(10);
    readVal = readVal - StaticVal;
    // Serial.print("Read val = ");
    // Serial.println(readVal);

    if (abs(readVal) > readTol) {
      DriveToSpeed( 0 , 5 );
      Home = true;
    }
  }

  delay(500);



  // Update
  A5_Pos = A5_HomeOffset;
  A5_encoderPos = A5Pos2Enc (A5_Pos);
  A5_Homed = true;

  // Movin 'awn up naw
  A5Move(50);
}

void A5ActiveControl() {
  int ValRead;
  int ValMapped;
  bool AtTop = false; // Do some checks to see if you're at the lims
  bool AtBot = false;

  // Update pos at start
  A5_Pos = A5Enc2Pos (A5_encoderPos);

  // Serial.print("Current Pos: ");
  // Serial.println(A5_Pos);

  if (A5_Pos <= 0) {
    AtTop = true;
  }
  else if (A5_Pos >= 100) {
    AtBot = true;
  }
  else {
    bool AtTop = false;
    bool AtBot = false;
  }

  // Collect static val (datum)
  if (!A5Cal) {
    A5staticVal = A5AvgSensorVal(100);
    A5Cal = true;
  }

  // Read in value
  ValRead = A5AvgSensorVal(10);

  // Map it
  ValMapped = ValRead - A5staticVal;
  if (abs(ValMapped) < A5SensorValDeadband) {
    ValMapped = 0;
  }
  else {
    ValMapped = ValMapped * 0.8; // Dampen velo to 80%
    if (abs(ValMapped) > 254) {
      ValMapped = 254 * (abs(ValMapped) / ValMapped);
    }

    // Stop if at lims
    if (AtTop && ValMapped < 0) {
      ValMapped = 0;
    }
    else if (AtBot && ValMapped > 0) {
      ValMapped = 0;
    }
  }

  // Drive to mapped speed
  DriveToSpeed( ValMapped , 5 );

  // Update pos
  //A5_Pos = A5_HomeOffset;
  //A5_encoderPos = A5Pos2Enc (A5_Pos);

}


void A5Move(float posVal) {
  // PID movement of A2
  //  Move to the absolute pos of 'Val'

  // Do some checks first,
  if (!A5_Homed) {
    // Serial.println("Axis 5 Not Homed!");
  }
  else if (posVal < 0 || posVal > 100) {
    // Serial.print(posVal);
    // Serial.println(" is out of range");
  }
  else { // Go to go!

    // Where are we moving to (counts)
    int Val = round(A5Pos2Enc (posVal));

    unsigned long previousMillis = 0;
    const long interval = 20;
    unsigned long currentMillis = millis();

    while (abs(Val - A5_encoderPos) >= A5_tol) {
      if (currentMillis - previousMillis >= interval) {  // start timed event
        previousMillis = currentMillis;

        // Update PID
        A5_Input = A5_encoderPos;
        A5_Setpoint = Val;
        A5PID.Compute();

        DriveToSpeed(A5_Output, 5);
      }
      currentMillis = millis();
      // end of timed event
    }
    DriveToSpeed(0, 5);
    delay(10);

    A5_Pos = A5Enc2Pos (A5_encoderPos);
  }
}

int A5AvgSensorVal( int nVals ) {
  /* This returns the pot value, averaged over 'nVals' readings */
  // nVals           // The number of values we are looking at
  int   valRead;     // This is the final value
  float Value = 0;   // This is the temporary value

  // Loop thru for average
  for (int i = 0; i < nVals; i++) {
    valRead = analogRead(A5_SensorPin);
    Value   = Value + valRead;
    delay(5);
  }
  Value = round(Value / nVals);
  return Value;
}

void A5IncMove(float Val) {
  /* Moves A2 incrementally a distance - 'Val'(mm)

  */

  float CurrentPos; // Current position (mm)
  float inPos;      // How far you have to increment (mm)

  // Current position (counts)
  CurrentPos = A5Enc2Pos (A5_encoderPos);

  // Amount required to increment
  inPos = CurrentPos + Val;

  //  Serial.print("CurrentPos = ");
  //  Serial.print(CurrentPos);
  //  Serial.print(" , A5_Pos = ");
  //  Serial.print(A5_Pos);
  //  Serial.print(" , inPos = ");
  //  Serial.println(inPos);

  // Move!
  A5Move(inPos);
}

int A5Pos2Enc (float Pos) {
  /* Returns position in encoder clicks from x2 = 0.

  */
  int Enc = A5_cpmm * Pos;
  return Enc;
}

float A5Enc2Pos (int Enc) {
  /* Returns position in encoder clicks from x2 = 0.

  */
  float Pos = Enc / A5_cpmm;
  return Pos;
}
