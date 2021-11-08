/* G Code commands */

void G00() {
  Serial.println("Help Codes: ");
  Serial.println("G00: Print help messages(s)");
  Serial.println("G01: Move A1 incrementally");
  Serial.println("  X<float>: increment distance");
  Serial.println("  Y<float>: linespeed");
  Serial.println("G02: Move A2 incrementally");
  Serial.println("  X<float>: increment distance");
  Serial.println("G03: Move A3 incrementally");
  Serial.println("  X<float>: increment distance");
  Serial.println("  Y<float>: linespeed");
}

void G01() {
  /* Incrementally move Axis 1
      - dist: distance (mm) we are incrementing
      - spd:  speed (mm/s) we are moving
              defaults to linespeed

  */

  float Tmax;          // How long are we running for
  int nInc;            // How many increments are we running
  float distMRev;      // Distance in motor revs
  float A1NewMotorPos; // New motor position

  // Retrieve Command
  float dist = NewCommand.X;
  float spd = NewCommand.Y;

  if (spd == 0) {
    spd = linespeed;
  }

  // Check if calibrated
  if (!A1Cal) {
    Serial.println("A1 is not calibrated");
    Serial.println("Command abandoned");
  }
  else {

    // Calc Tmax(millisec)
    Tmax = (abs(dist) * 1000) / spd;

    // Calc number of incs
    nInc = Tmax / dt;

    // Calc dist in motor revs
    distMRev = dist * A1Rpmm;

    // Loop through nInc
    for (int inc = 1; inc <= nInc; inc++) {
      A1NewMotorPos = A1MotorPos + ((float(inc) / nInc) * distMRev);
      Serial.println(A1NewMotorPos);
      odrive1.SetPosition(1, A1NewMotorPos);
      delay(dt);
    }

    // Update
    A1Pos = A1Pos + dist;
    A1MotorPos = A1NewMotorPos;
  }
}





void G03() {
  /* Incrementally move Axis 3
      - dist: distance (mm) we are incrementing
      - spd:  speed (mm/s) we are moving
              Defaults to linespeed

  */

  float Tmax;          // How long are we running for
  int nInc;            // How many increments are we running
  float distMRev;      // Distance in motor revs
  float A3NewMotorPos; // New motor position

  // Retrieve Command
  float dist = NewCommand.X;
  float spd = NewCommand.Y;

  if (spd == 0) {
    spd = linespeed;
  }

  // Check if calibrated
  if (!A3Cal) {
    Serial.println("A3 is not calibrated");
    Serial.println("Command abandoned");
  }
  else {

    // Calc Tmax(millisec)
    Tmax = (abs(dist) * 1000) / spd;

    // Calc number of incs
    nInc = Tmax / dt;

    // Calc dist in motor revs
    distMRev = dist * A3Rpmm;

    // Loop through nInc
    for (int inc = 1; inc <= nInc; inc++) {
      A3NewMotorPos = A3MotorPos + ((float(inc) / nInc) * distMRev);
      Serial.println(A3NewMotorPos);
      odrive2.SetPosition(0, A3NewMotorPos);
      delay(dt);
    }

    // Update
    A3Pos = A3Pos + dist;
    A3MotorPos = A3NewMotorPos;
  }
}






void G13( ) {
  /* Move A1 to absolute position 'Val' */

  float Val = NewCommand.X;

  Serial.print("Moving to: ");
  Serial.println(Val);

  odrive2.SetPosition(0, Val);
  odrive1.SetPosition(1, Val);
  delay(5);

  Serial.println("Moved!");

}

































void G43() {
  /* This homes Axis 2 */

  // Define some vars
  bool homed;             // Is home switch triggered?
  int delaytime = 1000;   // How long to keep moving tool up if initially homed
  float motorInc;         // Motor inc (at homespeed, given dt)
  float extraDist = 30;   // If homed, how much further out shall we go (mm)?

  // Check if calibrated
  if (!A3Cal) {
    Serial.println("A3 is not calibrated");
    Serial.println("Command abandoned");
  }
  else {
    Serial.println("Now starting homing sequence");

    // Calc motorInc
    motorInc = homespeed * dt * 0.001 * A3Rpmm;

    // First, read if homed
    homed = digitalRead(A3HomePin);

    // If it is already homed upon setup, we need to move it up
    // and beyond its home position, then enter the homing sequence.
    if (homed)
    {
      Serial.println("Home on, going to move up");

      delay(500);
      while (homed == true)
      {
        A3MotorPos = A3MotorPos + motorInc;
        odrive2.SetPosition(1, A3MotorPos);
        delay(dt);
        homed = digitalRead(A3HomePin);
      }

      // Extra dist in rev (with current pos)
      extraDist = A3MotorPos + (extraDist * A3Rpmm);

      while ( abs(A3MotorPos - extraDist) >= 0.05 )
      {
        A3MotorPos = A3MotorPos + motorInc;
        odrive2.SetPosition(1, A3MotorPos);
        delay(dt);
      }

      Serial.println("No longer homed");
      delay(500);
    }

    // Now begin the actual homing sequence
    Serial.println("Now Starting homing sequence");
    delay(1000);

    while (!homed)
    {
      A3MotorPos = A3MotorPos - motorInc;   // Other direction
      odrive2.SetPosition(1, A3MotorPos);
      homed = digitalRead(A3HomePin);
      delay(dt);
    }

    Serial.println("Homing Triggered");
    delay(500);

    // Update
    bool A3Home = true;           // Axis homed
    float A3Pos = A3HomingPos;    // Axis current position (mm)
  }
}
