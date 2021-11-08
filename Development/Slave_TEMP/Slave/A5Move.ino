/* This contains script for:
    - Incrementally move A2 an amount (mm)
    - Move A2 to absolutle position
    - A2 Homing sequence
*/


//void A2IncMove(float Val) {
//  /* Moves A2 incrementally a distance - 'Val'(mm)
//
//  */
//
//  float CurrentPos; // Current position (mm)
//  float inPos;      // How far you have to increment (mm)
//
//  // Current position (counts)
//  CurrentPos = A2Enc2Pos (A2_encoderPos);
//
//  // Amount required to increment
//  inPos = CurrentPos + Val;
//
//  // Move!
//  A2AbsMove(inPos);
//}
//
//
//void A2AbsMove(float Val) {
//  /* Moves A2 to absolute position - 'Val'(mm).
//   *  - This essentially does some checks, before
//   *    then calling A2Move to move axis
//      - checks if homed
//      - checks if in range
//  */
//
//  // Varibles
//  int encVal;  // What is the desired encoder value
//
//  //Check if homed
//  if (A2_Homed) {
//    delay(200);
//
//    // Abs pos in encoder vals
//    encVal = A2Pos2Enc(Val);
//
//    // Check if in range
//    if (encVal > A2_MinEnc && encVal < A2_MaxEnc) {
//      A2Move(encVal);
//    }
//    else {
//      Serial.println("Out of range!");
//    }
//  }
//  else {
//    Serial.println("A2 not homed!");
//  }
//}
//
//


void homeA5() {
  /* Homes A5 */

  int A5HomeSpeed = 40;
  int StaticVal;
  int readVal;
  int readTol = 20;
  bool Home = false;

  Serial.println("Homing A5");
  delay(500);
  Serial.println("Here we go...");

  StaticVal = A5AvgSensorVal( 100 );
  Serial.print("Static Val = ");
  Serial.println(StaticVal);

  DriveToSpeed( A5HomeSpeed , 5 );

  while (Home == false) {
    readVal = A5AvgSensorVal(10);
    readVal = readVal - StaticVal;
    Serial.print("Read val = ");
    Serial.println(readVal);

    if (abs(readVal) > readTol) {
      DriveToSpeed( 0 , 5 );
      Home = true;
    }
  }
}








//DriveToSpeed( A5HomeSpeed , 5 );






void A5ActiveControl() {
  int ValRead;
  int ValMapped;

  if (A5Cal == false) {
    Serial.println("Collecting static val");
    A5staticVal = A5AvgSensorVal(100);
    A5Cal = true;
    Serial.print("Static Val = ");
    Serial.println(A5staticVal);
  }

  // Read in value
  ValRead = A5AvgSensorVal(10);

  // Map it
  ValMapped = ValRead - A5staticVal;
  if (abs(ValMapped) < A5SensorValDeadband) {
    ValMapped = 0;
  }
  else {
    ValMapped = ValMapped * 0.8;
    if (abs(ValMapped) > 254) {
      ValMapped = 254 * (abs(ValMapped) / ValMapped);
    }
  }

  DriveToSpeed( ValMapped , 5 );


  Serial.print("ValRead = ");
  Serial.print(ValRead);
  Serial.print(" , ValMapped = ");
  Serial.println(ValMapped);
}


void A5Move(int Val) {
  // PID movement of A2
  //  Move to the absolute pos of 'Val'

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
  }
  Value = round(Value / nVals);
  return Value;
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
