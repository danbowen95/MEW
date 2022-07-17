/* This contains functions to do stuff

    Miscellaneous:
     - Reset()         - Idle state
     - DriveToSpeed()  - Drive an axis to a defined speed
     - Moveup()        - Move axis 5 or 8 up by around 50mm
     - Movedown()      - Move axis 5 or 8 down by around 50mm
     - blinker()       - Blink integrated LED
     - shutdownSeq()   - Sends axis to shutdown posns

    A2:
     - A2Move()         - Moves A2 under PID control
     - A2IncMove()      - Moves A2 incrementally from current pos
     - A2AbsMove()      - Moves A2 to absolute position
     - A2Home()         - Homes A2
     - A2Pos2Enc()      - Returns position in encoder counts (given pos in mm)
     - A2Enc2Pos()      - Returns position in mm (given pos in encoder counts)

    A5:
     - A5Move()         - Moves A5 under PID control
     - A5IncMove()      - Moves A5 incrementally from current pos
     - A5AbsMove()      - Moves A5 to absolute position
     - A5Home()         - Homes A5
     - A5Pos2Enc()      - Returns position in encoder counts (given pos in mm)
     - A5Enc2Pos()      - Returns position in mm (given pos in encoder counts)
     - A5AvgSensorVal() - returns (filtered) sensor value
     - A5ActiveControl()- Turns on Active control
     - A5Down()         - Moves A5 down a set value or to bottom
    A8:
     - A8Move()         - Moves A8 under PID control
     - A8IncMove()      - Moves A8 incrementally from current pos
     - A8AbsMove()      - Moves A8 to absolute position
     - A8Home()         - Homes A8
     - A8Pos2Enc()      - Returns position in encoder counts (given pos in mm)
     - A8Enc2Pos()      - Returns position in mm (given pos in encoder counts)
     - A8AvgSensorVal() - returns (filtered) sensor value
     - A8ActiveControl()- Turns on Active control
     - A8Down()         - Moves A8 down a set value or to bottom

    Encoders:
      These are interupt functions to increment encoder values
     - A2_DoEncoderA()
     - A2_DoEncoderB()
     - A5_DoEncoderA()
     - A5_DoEncoderB()
     - A8_DoEncoderA()
     - A8_DoEncoderB()

    Messages:
     - sendUpdateMsg()    - Sends message over serial to master to say command excecuted
     - resetModeCommand() - This resets the machine struct


*/

// - - - - - - - - - Miscellaneous - - - - - - - - - - - - \\

void Reset() {

  // Reset cal of Active sensor
  A5Cal = false;
  A8Cal = false;

  // Stop movement
  DriveToSpeed( 0 , 5 );
  DriveToSpeed( 0 , 8 );

}



void DriveToSpeed(int Val , int AxisTemp) {
  /* Setting 'AxisTemp' to speed 'Val'.
      - 'AxisTemp' must be 2,5 or 8
      - 'Val' must be between -255 and 255
      - ^^ Has no catch
      - The code can (and probably should) be written MUCH more efficiently
  */

  switch (AxisTemp) {
    case 2:
      {
        if (Val <= 0)
        {
          // reverse rotation
          analogWrite(A2_LPWM, 0);
          analogWrite(A2_RPWM, abs(Val));
        }
        else
        {
          // forward rotation
          analogWrite(A2_RPWM, 0);
          analogWrite(A2_LPWM, Val);
        }
      }
      break;
    case 5:
      {
        if (Val <= 0)
        {
          // reverse rotation
          analogWrite(A5_LPWM, 0);
          analogWrite(A5_RPWM, abs(Val));
        }
        else
        {
          // forward rotation
          analogWrite(A5_RPWM, 0);
          analogWrite(A5_LPWM, Val);
        }
      }
      break;
    case 8:
      {
        if (Val <= 0)
        {
          // reverse rotation
          analogWrite(A8_LPWM, 0);
          analogWrite(A8_RPWM, abs(Val));
        }
        else
        {
          // forward rotation
          analogWrite(A8_RPWM, 0);
          analogWrite(A8_LPWM, Val);
        }
      }
      break;
  }
}


void moveup(int AxisTemp)
{
  DriveToSpeed( -200 , AxisTemp );
  delay(500);
  DriveToSpeed( 0 , AxisTemp );
}

void movedown(int AxisTemp)
{
  DriveToSpeed( 200 , AxisTemp );
  delay(500);
  DriveToSpeed( 0 , AxisTemp );
}

void blinkLED( int n , float dt ) {
  for (int i = 1; i <= n; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(dt);
    digitalWrite(LED_BUILTIN, LOW);
    delay(dt);
  }
}

void shutdownSeq() {
  A2AbsMove(7);
  A5Move(50);
  A8Move(50);
}


// - - - - - - - - - A2 - - - - - - - - - - - - \\

void A2Move(int Val) {
  //  PID movement of A2
  //  Move to the absolute pos of 'Val'
  // **Warning** - No position checks done here!

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
    currentMillis = millis();
    // end of timed event
  }
  DriveToSpeed(0, 2);
}

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
      - This essentially does some checks, before
        then calling A2Move to move axis
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





void A2Home()
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
  //  slaveStatus.A2Home = true;
  //  slaveStatus.A2Pos  = A2_encoderPos;


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


































// - - - - - - - - - A5 - - - - - - - - - - - - \\

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

void A5IncMove(float Val) {
  /* Moves A2 incrementally a distance - 'Val'(mm)

  */

  float CurrentPos; // Current position (mm)
  float inPos;      // How far you have to increment (mm)

  // Current position (mm)
  CurrentPos = A5Enc2Pos (A5_encoderPos);

  // Amount required to increment
  inPos = CurrentPos + Val;

  // Move!
  A5Move(inPos);
}


void A5AbsMove(float Val) {
  // Pretty useless function, but added for completeness
  A5Move(Val);

}


void A5Home() {
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





void A5ActiveControl(bool shiftUp) {
  int ValRead;
  int ValMapped;
  bool AtTop = false; // Do some checks to see if you're at the lims
  bool AtBot = false;

  if (A5_Homed) {
    // Update pos at start
    A5_Pos = A5Enc2Pos (A5_encoderPos);

    // Have we reached Lims
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

    // If we are shifting up:
    if (shiftUp) {

      // Start moving up
      DriveToSpeed( (abs(shiftSpeed) * -1), 5 );

      // Read in value
      ValRead = A5AvgSensorVal(10);

      // Map it
      ValMapped = ValRead - A5staticVal;

      // Keep moving up until sensor triggered
      while (abs(ValMapped) < A5SensorValDeadband) {
        // Read in value
        ValRead = A5AvgSensorVal(10);

        // Map it
        ValMapped = ValRead - A5staticVal;

        // Are we at the top    
        // Update pos at start
        A5_Pos = A5Enc2Pos (A5_encoderPos);
        if (A5_Pos <= 0) {
          break;
        }
      }

      // Now put into regular active control
      machineStatus.mode = 'k';

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
  }

  sendUpdateMsg();

  if (A5_Homed) {
    active = true;
  }

}



void A5Down() {


  float CurrentPos; // Current position (mm)
  float incPos;      // How far you have to increment (mm)

  // Current position (mm)
  CurrentPos = A5Enc2Pos (A5_encoderPos);

  // Amount required to increment
  incPos = CurrentPos + m_down;

  if (incPos < 0 || incPos > 100) {
    A5Move(100);
  }
  else {
    A5Move(incPos);
  }
}



// - - - - - - - - - A8 - - - - - - - - - - - - \\

void A8Move(float posVal) {
  // PID movement of A8
  //  Move to the absolute pos of 'Val'

  // Do some checks first,
  if (!A8_Homed) {
    // Serial.println("Axis 8 Not Homed!");
  }
  else if (posVal < 0 || posVal > 100) {
    // Serial.print(posVal);
    // Serial.println(" is out of range");
  }
  else { // Go to go!

    // Where are we moving to (counts)
    int Val = round(A8Pos2Enc (posVal));

    unsigned long previousMillis = 0;
    const long interval = 20;
    unsigned long currentMillis = millis();

    while (abs(Val - A8_encoderPos) >= A8_tol) {
      if (currentMillis - previousMillis >= interval) {  // start timed event
        previousMillis = currentMillis;

        // Update PID
        A8_Input = A8_encoderPos;
        A8_Setpoint = Val;
        A8PID.Compute();

        DriveToSpeed(A8_Output, 8);
      }
      currentMillis = millis();
      // end of timed event
    }
    DriveToSpeed(0, 8);
    delay(10);

    A8_Pos = A8Enc2Pos (A8_encoderPos);
  }
}

void A8IncMove(float Val) {
  /* Moves A2 incrementally a distance - 'Val'(mm)

  */

  float CurrentPos; // Current position (mm)
  float inPos;      // How far you have to increment (mm)

  // Current position (counts)
  CurrentPos = A8Enc2Pos (A8_encoderPos);

  // Amount required to increment
  inPos = CurrentPos + Val;

  // Move!
  A8Move(inPos);
}


void A8AbsMove(float Val) {
  A8Move(Val);
}


void A8Home() {
  /* Homes A8 */

  int A8HomeSpeed = 40;
  int StaticVal;
  int readVal;
  int readTol = 20;
  bool Home = false;

  // Serial.println("Homing A8");
  delay(500);
  // Serial.println("Here we go...");

  StaticVal = A8AvgSensorVal( 100 );
  // Serial.print("Static Val = ");
  // Serial.println(StaticVal);

  DriveToSpeed( A8HomeSpeed , 8 );

  while (Home == false) {
    readVal = A8AvgSensorVal(10);
    readVal = readVal - StaticVal;
    // Serial.print("Read val = ");
    // Serial.println(readVal);

    if (abs(readVal) > readTol) {
      DriveToSpeed( 0 , 8 );
      Home = true;
    }
  }

  delay(500);

  // Update
  A8_Pos = A8_HomeOffset;
  A8_encoderPos = A8Pos2Enc (A8_Pos);
  A8_Homed = true;

  // Movin 'awn up naw
  A8Move(50);
}

int A8Pos2Enc (float Pos) {
  /* Returns position in encoder clicks from x2 = 0.

  */
  int Enc = A8_cpmm * Pos;
  return Enc;
}

float A8Enc2Pos (int Enc) {
  /* Returns position in encoder clicks from x2 = 0.

  */
  float Pos = Enc / A8_cpmm;
  return Pos;
}

int A8AvgSensorVal( int nVals ) {
  /* This returns the pot value, averaged over 'nVals' readings */
  // nVals           // The number of values we are looking at
  int   valRead;     // This is the final value
  float Value = 0;   // This is the temporary value

  // Loop thru for average
  for (int i = 0; i < nVals; i++) {
    valRead = analogRead(A8_SensorPin);
    Value   = Value + valRead;
    delay(5);
  }
  Value = round(Value / nVals);
  return Value;
}





void A8ActiveControl(bool shiftUp) {
  int ValRead;
  int ValMapped;
  bool AtTop = false; // Do some checks to see if you're at the lims
  bool AtBot = false;

  if (A8_Homed) {
    // Update pos at start
    A8_Pos = A8Enc2Pos (A8_encoderPos);

    // Have we reached Lims
    if (A8_Pos <= 0) {
      AtTop = true;
    }
    else if (A8_Pos >= 100) {
      AtBot = true;
    }
    else {
      bool AtTop = false;
      bool AtBot = false;
    }

    // Collect static val (datum)
    if (!A8Cal) {
      A8staticVal = A8AvgSensorVal(100);
      A8Cal = true;
    }

    // If we are shifting up:
    if (shiftUp) {

      // Start moving up
      DriveToSpeed( (abs(shiftSpeed) * -1), 8 );

      // Read in value
      ValRead = A8AvgSensorVal(10);

      // Map it
      ValMapped = ValRead - A8staticVal;

      // Keep moving up until sensor triggered
      while (abs(ValMapped) < A8SensorValDeadband) {
        // Read in value
        ValRead = A8AvgSensorVal(10);

        // Map it
        ValMapped = ValRead - A8staticVal;

        // Are we at the top
        // Update pos at start
        A8_Pos = A8Enc2Pos (A8_encoderPos);
        if (A8_Pos <= 0) {
          break;
        }
      }

      // Now put into regular active control
      machineStatus.mode = 'l';

    }

    // Read in value
    ValRead = A8AvgSensorVal(10);

    // Map it
    ValMapped = ValRead - A8staticVal;
    if (abs(ValMapped) < A8SensorValDeadband) {
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
    DriveToSpeed( ValMapped , 8 );

    // Update pos
    //A8_Pos = A8_HomeOffset;
    //A8_encoderPos = A8Pos2Enc (A8_Pos);
  }

  sendUpdateMsg();

  if (A5_Homed) {
    active = true;
  }

}


void A8Down() {


  float CurrentPos; // Current position (mm)
  float incPos;      // How far you have to increment (mm)

  // Current position (mm)
  CurrentPos = A8Enc2Pos (A8_encoderPos);

  // Amount required to increment
  incPos = CurrentPos + m_down;

  if (incPos < 0 || incPos > 100) {
    A8Move(100);
  }
  else {
    A8Move(incPos);
  }
}


// - - - - - - - - - Encoders - - - - - - - - - - - - \\

void A2_doEncoderA() {
  // look for a low-to-high on channel A
  if (digitalRead(A2_encoderPinA) == HIGH) {

    // check channel B to see which way encoder is turning
    if (digitalRead(A2_encoderPinB) == LOW) {
      A2_encoderPos = A2_encoderPos + 1;         // CW
    }
    else {
      A2_encoderPos = A2_encoderPos - 1;         // CCW
    }
  }

  else   // must be a high-to-low edge on channel A
  {
    // check channel B to see which way encoder is turning
    if (digitalRead(A2_encoderPinB) == HIGH) {
      A2_encoderPos = A2_encoderPos + 1;          // CW
    }
    else {
      A2_encoderPos = A2_encoderPos - 1;          // CCW
    }
  }
  // use for debugging - remember to comment out
  //Serial.println(A2_encoderPos);
}
//
// A2b
void A2_doEncoderB() {
  // look for a low-to-high on channel B
  if (digitalRead(A2_encoderPinB) == HIGH) {

    // check channel A to see which way encoder is turning
    if (digitalRead(A2_encoderPinA) == HIGH) {
      A2_encoderPos = A2_encoderPos + 1;         // CW
    }
    else {
      A2_encoderPos = A2_encoderPos - 1;         // CCW
    }
  }

  // Look for a high-to-low on channel B
  else {
    // check channel B to see which way encoder is turning
    if (digitalRead(A2_encoderPinA) == LOW) {
      A2_encoderPos = A2_encoderPos + 1;          // CW
    }
    else {
      A2_encoderPos = A2_encoderPos - 1;          // CCW
    }
  }
  //Serial.println(A2_encoderPos);
}

// Axis 5
// A5a
void A5_doEncoderA() {
  // look for a low-to-high on channel A
  if (digitalRead(A5_encoderPinA) == HIGH) {

    // check channel B to see which way encoder is turning
    if (digitalRead(A5_encoderPinB) == LOW) {
      A5_encoderPos = A5_encoderPos + 1;         // CW
    }
    else {
      A5_encoderPos = A5_encoderPos - 1;         // CCW
    }
  }

  else   // must be a high-to-low edge on channel A
  {
    // check channel B to see which way encoder is turning
    if (digitalRead(A5_encoderPinB) == HIGH) {
      A5_encoderPos = A5_encoderPos + 1;          // CW
    }
    else {
      A5_encoderPos = A5_encoderPos - 1;          // CCW
    }
  }
  // use for debugging - remember to comment out
}
//
// A5b
void A5_doEncoderB() {
  // look for a low-to-high on channel B
  if (digitalRead(A5_encoderPinB) == HIGH) {

    // check channel A to see which way encoder is turning
    if (digitalRead(A5_encoderPinA) == HIGH) {
      A5_encoderPos = A5_encoderPos + 1;         // CW
    }
    else {
      A5_encoderPos = A5_encoderPos - 1;         // CCW
    }
  }

  // Look for a high-to-low on channel B
  else {
    // check channel B to see which way encoder is turning
    if (digitalRead(A5_encoderPinA) == LOW) {
      A5_encoderPos = A5_encoderPos + 1;          // CW
    }
    else {
      A5_encoderPos = A5_encoderPos - 1;          // CCW
    }
  }
}

// Axis 8
// A8a
void A8_doEncoderA() {
  // look for a low-to-high on channel A
  if (digitalRead(A8_encoderPinA) == HIGH) {

    // check channel B to see which way encoder is turning
    if (digitalRead(A8_encoderPinB) == LOW) {
      A8_encoderPos = A8_encoderPos + 1;         // CW
    }
    else {
      A8_encoderPos = A8_encoderPos - 1;         // CCW
    }
  }

  else   // must be a high-to-low edge on channel A
  {
    // check channel B to see which way encoder is turning
    if (digitalRead(A8_encoderPinB) == HIGH) {
      A8_encoderPos = A8_encoderPos + 1;          // CW
    }
    else {
      A8_encoderPos = A8_encoderPos - 1;          // CCW
    }
  }
  // use for debugging - remember to comment out
}
//
// A5b
void A8_doEncoderB() {
  // look for a low-to-high on channel B
  if (digitalRead(A8_encoderPinB) == HIGH) {

    // check channel A to see which way encoder is turning
    if (digitalRead(A8_encoderPinA) == HIGH) {
      A8_encoderPos = A8_encoderPos + 1;         // CW
    }
    else {
      A8_encoderPos = A8_encoderPos - 1;         // CCW
    }
  }

  // Look for a high-to-low on channel B
  else {
    // check channel B to see which way encoder is turning
    if (digitalRead(A8_encoderPinA) == LOW) {
      A8_encoderPos = A8_encoderPos + 1;          // CW
    }
    else {
      A8_encoderPos = A8_encoderPos - 1;          // CCW
    }
  }
}


// - - - - - - - - - - - Messages - - - - - - - - - - - - - - \\


void sendUpdateMsg() {
  if (active == false) {
    Serial1.write("d");
  }
}




void resetModeCommand(ModeStatus * machineStatus)
{
  // Reset NewCommand ready for the next
  machineStatus->mode   = 'a';
  machineStatus->value  = 0;
  machineStatus->active = false;

  sendUpdateMsg();
  active = false;
}


void TempFun() {
  Serial.println("move in one direction, then another");
  Serial.println("Current vals of:");
  Serial.print("A8_encoderPos - ");
  Serial.println(A8_encoderPos);
  Serial.print("A8_Pos - ");
  Serial.println(A8_Pos);

  delay(1000);
  Serial.println("now moving...");

  DriveToSpeed(100, 8);
  delay(1000);
  DriveToSpeed(0, 8);
  delay(1000);

  Serial.println("");
  Serial.println("Moved!");
  Serial.print("A8_encoderPos - ");
  Serial.println(A8_encoderPos);
  Serial.print("A8_Pos - ");
  Serial.println(A8_Pos);

  delay(5000);
  Serial.println("now moving...");

  DriveToSpeed(-100, 8);
  delay(1000);
  DriveToSpeed(0, 8);
  delay(1000);
}
