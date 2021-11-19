/* This contains script for:
    - Incrementally move A2 an amount (mm)
    - Move A2 to absolutle position
    - A2 Homing sequence
*/


void A2IncMove(float Val) {
  /* Moves A2 incrementally a distance - 'Val'(mm)

  */

  float CurrentPos; // Current position (mm)
  float inPos;      // How far you have to increment (mm)

  // Current position (counts)
  CurrentPos = A2Enc2Pos (A2_encoderPos);

  // Amount required to increment
  inPos = CurrentPos + Val;

  // Move!
  A2AbsMove(inPos);
}


void A2AbsMove(float Val) {
  /* Moves A2 to absolute position - 'Val'(mm).
   *  - This essentially does some checks, before 
   *    then calling A2Move to move axis
      - checks if homed
      - checks if in range
  */

  // Varibles
  int encVal;  // What is the desired encoder value

  //Check if homed
  if (A2_Homed) {
    delay(200);

    // Abs pos in encoder vals
    encVal = A2Pos2Enc(Val);

    // Check if in range
    if (encVal > A2_MinEnc && encVal < A2_MaxEnc) {
      A2Move(encVal);
    }
    else {
      // Out of range
    }
  }
  else {
    // Not homed!
  }
}


void A2Move(int Val) {
  // PID movement of A2
  //  Move to the absolute pos of 'Val'

  unsigned long previousMillis = 0;
  const long interval = 20;
  unsigned long currentMillis = millis();

  while (abs(Val - A2_encoderPos) >= A2_tol) {
    if (currentMillis - previousMillis >= interval) {  // start timed event
      // Update PID
      A2_Input = A2_encoderPos;
      A2_Setpoint = Val;
      A2PID.Compute();

      DriveToSpeed(A2_Output, 2);
    }
    // end of timed event
  }
  DriveToSpeed(0, 2);
}


void homeA2()
{
  /* This homes Axis 2 */

  // Define some vars
  int homespeed = 100;    // The speed at which homing occurs
  bool homed;             // Is home switch triggered?
  int delaytime = 1000;   // How long to keep moving tool up if initially homed

  // First, read if homed
  homed = digitalRead(A2_Home);

  // If it is already homed upon setup, we need to move it up
  // and beyond its home position, then enter the homing sequence.
  if (homed)
  {
    // Serial.println("Home on, going to move up");
    delay(500);
    while (homed == true)
    {
      DriveToSpeed( -1 * homespeed , 2 );
      homed = digitalRead(A2_Home);
    }

    delay(delaytime);
    DriveToSpeed( 0 , 2 );

    // Serial.println("No longer homed");
    delay(500);
  }


  // Now begin the actual homing sequence
  // Serial.println("Now Starting homing sequence");
  delay(1000);
  while (!homed)
  {
    DriveToSpeed( homespeed , 2 );
    homed = digitalRead(A2_Home);
  }
  // Serial.println("Homing Triggered");
  DriveToSpeed( 0 , 2 );
  delay(500);


  // Update
  slaveStatus.A2Home = true;
  slaveStatus.A2Pos  = A2_encoderPos;


  A2_encoderPos = A2_HomeOffset;
  A2_Homed = true;

  // Reset
  resetModeCommand(&machineStatus);

}


int A2Pos2Enc (float Pos) {
  /* Returns position in encoder clicks from x2 = 0.

  */
  int Enc = A2_cpmm * Pos;
  return Enc;
}

float A2Enc2Pos (int Enc) {
  /* Returns position in encoder clicks from x2 = 0.

  */
  float Pos = Enc / A2_cpmm;
  return Pos;
}
