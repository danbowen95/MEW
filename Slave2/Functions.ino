/* This contains functions to do stuff

   Actuation:
    - A1MoveInc()   - Move A1 an incremental amount (mm).
    - A3MoveAbs()   - Move A3 to absolute position.
    - A4MoveAbs()   - Move A4 to absolute position.
    - A6MoveAbs()   - Move A6 to absolute position.
    - A7MoveAbs()   - Move A7 to absolute position.
    - A3A1MoveAbs() - Move A3 to absolute position whilst moving A1 in sync.
    - A7A1MoveAbs() - Move A6 to absolute position whilst moving A1 in sync.
    - A3A4Arc()     - Move A3 & A4 in arc
    - A6A7Arc()     - Move A6 & A7 in arc
    
    Calibration & Home
     - calMotor()       - Calibrates axis n.
     - A3HomeSeq()      - Home A3.
     - A4HomeSeq()      - Home A4.
     - A6HomeSeq()      - Home A6.
     - A7HomeSeq()      - Home A7.
     - axisHomeSwitch() - reads if lim switch is on

   Checking Functions:
      axisHome()   - is the axis homed (bool)
      axisCal()    - is the axis calibrated (bool)
      withinLims() - is abs position within lims (bool)

    Miscellaneous:
     - roundNum()   - Rounds a float
     - blinkLED()   - blinks LED
     - mintorad()   - minutes to radians conversion
     
    Messages:
     - sendUpdateMsg()    - Sends message over serial to master to say command excecuted
     - resetModeCommand() - This resets the machine struct


*/






// - - - - - - - - - - - Actuation - - - - - - - - - - - - - - \\



void A1MoveInc( float PosFinal , float moveSpd ) {
  /* Moves Axis 1 an incremental amount
      - PosFinal: Absolute position we are moving to
      - moveSpd:  Speed (mm/s) we are moving at
  */
  // Some vars
  int Tmove;         // How long are we running for (given dist & spd)
  float RevInc;      // How much is the motor incrementing?
  float RevFinal;    // Final motor position (Revs)
  float RevPos;      // Temp pos of motor as it moves (Revs)
  bool Done = false; // For breaking the while loop
  bool Good2Move;    // Can we move?

  // Time
  unsigned long runTime;    // Current running time
  unsigned long startTime;  // The time we start moving
  unsigned long endTime;    // The time we expect to end

  /// Do checks to see if we're good to go...
  Good2Move = axisCal( 1 );

  // Are we good to move it move it?
  if (Good2Move) {
    // I like to move it, move it
    //Serial.println("Moving!");

    // How long will the move take, for the given speed
    // Note:  - linespeed should ALWAYS be a factor of 100 AND <= 100.
    //          (1,2,4,5,10,20,25,50,100 mm/s)
    //        - resolution of dist = XXXXX.X (mm)
    //        -> therefor, Tmove ALWAYS an int (millisec)
    PosFinal = roundNum( PosFinal , 1 );
    Tmove = (abs(PosFinal) * 1000) / moveSpd;

    // Calc total (final) motor increment (Rev)
    RevInc = A1Dir * ((PosFinal) * A1Rpmm);

    // Work off time:
    startTime = millis();        // The time we start moving
    endTime = startTime + Tmove;

    while ( !Done ) {
      // calc runtime:
      runTime = millis() - startTime;

      // calc motor Pos:
      RevPos = A1MotorPos + ( runTime * (RevInc / Tmove));

      // Send to pos:
      odrive1.SetPosition(1, RevPos);

      // If within dt of Tmax, send to final pos:
      if ( runTime >= Tmove - dt) {
        // Send to final position
        odrive1.SetPosition(1, (A1MotorPos + RevInc));

        // Update some values:
        A1MotorPos = A1MotorPos + RevInc;
        Done = true;
        break;
      }
      else {
        // Wait for motor to catch up and serial to clear
        delay(dt);
      }
    }
  }
  else {
    //Serial.println("Good2Move is false");
  }
}


void A3MoveAbs( float PosFinal , float moveSpd , bool homing ) {
  /* Move Axis 3 to abs pos
      - PosFinal: Absolute position we are moving to
      - moveSpd:  Speed (mm/s) we are moving at
      - homing:   0 not homing, do lim checks
                  1 homing, ignore lim checks
  */
  // Some vars
  int Tmove;         // How long are we running for (given dist & spd)
  float RevInc;      // How much is the motor incrementing?
  float RevFinal;    // Final motor position (Revs)
  float RevPos;      // Temp pos of motor as it moves (Revs)
  bool Done = false; // For breaking the while loop
  bool Good2Move;    // Can we move?

  // Time
  unsigned long runTime;    // Current running time
  unsigned long startTime;  // The time we start moving
  unsigned long endTime;    // The time we expect to end

  /// Do a bunch of checks to see if we're good to go
  if (homing) {
    Good2Move = axisCal( 3 );
  }
  else {
    Good2Move = axisCal( 3 ) * axisHome( 3 ) * withinLims( 3 , PosFinal );
  }

  // Are we good to move it move it?
  if (Good2Move) {
    // I like to move it, move it
    //Serial.println("Moving!");


    // How long will the move take, for the given speed
    // Note:  - linespeed should ALWAYS be a factor of 100 AND <= 100.
    //          (1,2,4,5,10,20,25,50,100 mm/s)
    //        - resolution of dist = XXX.X (mm)
    //        -> therefor, Tmove ALWAYS an int (millisec)
    PosFinal = roundNum( PosFinal , 1 );
    Tmove = (abs(A3Pos - PosFinal) * 1000) / moveSpd;

    // Calc total (final) motor increment (Rev)
    RevInc = A3Dir * ((PosFinal - A3Pos) * A3Rpmm);

    // Work off time:
    startTime = millis();        // The time we start moving
    endTime = startTime + Tmove;

    while ( !Done ) {
      // calc runtime:
      runTime = millis() - startTime;

      // calc motor Pos:
      RevPos = A3MotorPos + ( runTime * (RevInc / Tmove));

      // Send to pos:
      odrive2.SetPosition(0, RevPos);

      // If within dt of Tmax, send to final pos:
      if ( runTime >= Tmove - dt) {
        // Send to final position
        odrive2.SetPosition(0, (A3MotorPos + RevInc));

        // Update some values:
        A3MotorPos = A3MotorPos + RevInc;
        A3Pos = PosFinal;
        Done = true;
        break;
      }
      else {
        // Wait for motor to catch up and serial to clear
        delay(dt);
      }
    }
  }
  else {
    //Serial.println("Good2Move is false");
  }
}

void A4MoveAbs( float PosFinal , float moveSpd , bool homing ) {
  /* Move Axis 4 to abs pos
      - PosFinal: Absolute position we are moving to
      - moveSpd:  Speed (mm/s) we are moving at
      - homing:   0 not homing, do lim checks
                  1 homing, ignore lim checks
  */
  // Some vars
  int Tmove;         // How long are we running for (given dist & spd)
  float RevInc;      // How much is the motor incrementing?
  float RevFinal;    // Final motor position (Revs)
  float RevPos;      // Temp pos of motor as it moves (Revs)
  bool Done = false; // For breaking the while loop
  bool Good2Move;    // Can we move?

  // Time
  unsigned long runTime;    // Current running time
  unsigned long startTime;  // The time we start moving
  unsigned long endTime;    // The time we expect to end

  /// Do a bunch of checks to see if we're good to go
  if (homing) {
    Good2Move = axisCal( 4 );
  }
  else {
    Good2Move = axisCal( 4 ) * axisHome( 4 ) * withinLims( 4 , PosFinal );
  }

  // Are we good to move it move it?
  if (Good2Move) {
    // I like to move it, move it
    //Serial.println("Moving!");

    // How long will the move take, for the given speed
    // Note:  - linespeed should ALWAYS be a factor of 100 AND <= 100.
    //          (1,2,4,5,10,20,25,50,100 mm/s)
    //        - resolution of dist = XXX.X (mm)
    //        -> therefor, Tmove ALWAYS an int (millisec)
    PosFinal = roundNum( PosFinal , 1 );
    Tmove = (abs(A4Pos - PosFinal) * 1000) / moveSpd;

    // Calc total (final) motor increment (Rev)
    RevInc = A4Dir * ((PosFinal - A4Pos) * A4Rpmm);

    // Work off time:
    startTime = millis();        // The time we start moving
    endTime = startTime + Tmove;

    while ( !Done ) {
      // calc runtime:
      runTime = millis() - startTime;

      // calc motor Pos:
      RevPos = A4MotorPos + ( runTime * (RevInc / Tmove));

      // Send to pos:
      odrive2.SetPosition(1, RevPos);

      // If within dt of Tmax, send to final pos:
      if ( runTime >= Tmove - dt) {
        // Send to final position
        odrive2.SetPosition(1, (A4MotorPos + RevInc));

        // Update some values:
        A4MotorPos = A4MotorPos + RevInc;
        A4Pos = PosFinal;
        Done = true;
        break;
      }
      else {
        // Wait for motor to catch up and serial to clear
        delay(dt);
      }
    }
  }
  else {
    ////Serial.println("Good2Move is false");
  }
}

void A6MoveAbs( float PosFinal , float moveSpd , bool homing ) {
  /* Move Axis 6 to abs pos
      - PosFinal: Absolute position we are moving to
      - moveSpd:  Speed (mm/s) we are moving at
      - homing:   0 not homing, do lim checks
                  1 homing, ignore lim checks
  */
  // Some vars
  int Tmove;         // How long are we running for (given dist & spd)
  float RevInc;      // How much is the motor incrementing?
  float RevFinal;    // Final motor position (Revs)
  float RevPos;      // Temp pos of motor as it moves (Revs)
  bool Done = false; // For breaking the while loop
  bool Good2Move;    // Can we move?

  // Time
  unsigned long runTime;    // Current running time
  unsigned long startTime;  // The time we start moving
  unsigned long endTime;    // The time we expect to end

  /// Do a bunch of checks to see if we're good to go
  if (homing) {
    Good2Move = axisCal( 6 );
  }
  else {
    Good2Move = axisCal( 6 ) * axisHome( 6 ) * withinLims( 6 , PosFinal );
  }

  // Are we good to move it move it?
  if (Good2Move) {
    // I like to move it, move it
    //Serial.println("Moving!");

    // How long will the move take, for the given speed
    // Note:  - linespeed should ALWAYS be a factor of 100 AND <= 100.
    //          (1,2,4,5,10,20,25,50,100 mm/s)
    //        - resolution of dist = XXX.X (mm)
    //        -> therefor, Tmove ALWAYS an int (millisec)
    PosFinal = roundNum( PosFinal , 1 );
    Tmove = (abs(A6Pos - PosFinal) * 1000) / moveSpd;

    // Calc total (final) motor increment (Rev)
    RevInc = A6Dir * ((PosFinal - A6Pos) * A6Rpmm);

    // Work off time:
    startTime = millis();        // The time we start moving
    endTime = startTime + Tmove;

    while ( !Done ) {
      // calc runtime:
      runTime = millis() - startTime;

      // calc motor Pos:
      RevPos = A6MotorPos + ( runTime * (RevInc / Tmove));

      // Send to pos:
      odrive3.SetPosition(0, RevPos);

      // If within dt of Tmax, send to final pos:
      if ( runTime >= Tmove - dt) {
        // Send to final position
        odrive3.SetPosition(0, (A6MotorPos + RevInc));

        // Update some values:
        A6MotorPos = A6MotorPos + RevInc;
        A6Pos = PosFinal;
        Done = true;
        break;
      }
      else {
        // Wait for motor to catch up and serial to clear
        delay(dt);
      }
    }
  }
  else {
    //Serial.println("Good2Move is false");
  }
}

void A7MoveAbs( float PosFinal , float moveSpd , bool homing ) {
  /* Move Axis 7 to abs pos
      - PosFinal: Absolute position we are moving to
      - moveSpd:  Speed (mm/s) we are moving at
      - homing:   0 not homing, do lim checks
                  1 homing, ignore lim checks
  */
  // Some vars
  int Tmove;         // How long are we running for (given dist & spd)
  float RevInc;      // How much is the motor incrementing?
  float RevFinal;    // Final motor position (Revs)
  float RevPos;      // Temp pos of motor as it moves (Revs)
  bool Done = false; // For breaking the while loop
  bool Good2Move;    // Can we move?

  // Time
  unsigned long runTime;    // Current running time
  unsigned long startTime;  // The time we start moving
  unsigned long endTime;    // The time we expect to end

  /// Do a bunch of checks to see if we're good to go
  if (homing) {
    Good2Move = axisCal( 7 );
  }
  else {
    Good2Move = axisCal( 7 ) * axisHome( 7 ) * withinLims( 7 , PosFinal );
  }

  // Are we good to move it move it?
  if (Good2Move) {
    // I like to move it, move it
    //Serial.println("Moving!");

    // How long will the move take, for the given speed
    // Note:  - linespeed should ALWAYS be a factor of 100 AND <= 100.
    //          (1,2,4,5,10,20,25,50,100 mm/s)
    //        - resolution of dist = XXX.X (mm)
    //        -> therefor, Tmove ALWAYS an int (millisec)
    PosFinal = roundNum( PosFinal , 1 );
    Tmove = (abs(A7Pos - PosFinal) * 1000) / moveSpd;

    // Calc total (final) motor increment (Rev)
    RevInc = A7Dir * ((PosFinal - A7Pos) * A7Rpmm);

    // Work off time:
    startTime = millis();        // The time we start moving
    endTime = startTime + Tmove;

    while ( !Done ) {
      // calc runtime:
      runTime = millis() - startTime;

      // calc motor Pos:
      RevPos = A7MotorPos + ( runTime * (RevInc / Tmove));

      // Send to pos:
      odrive3.SetPosition(1, RevPos);

      // If within dt of Tmax, send to final pos:
      if ( runTime >= Tmove - dt) {
        // Send to final position
        odrive3.SetPosition(1, (A7MotorPos + RevInc));

        // Update some values:
        A7MotorPos = A7MotorPos + RevInc;
        A7Pos = PosFinal;
        Done = true;
        break;
      }
      else {
        // Wait for motor to catch up and serial to clear
        delay(dt);
      }
    }
  }
  else {
    //Serial.println("Good2Move is false");
  }
}


void A3A1MoveAbs( float PosFinal , float moveSpd ) {
  /* Move Axis 3 to abs pos, with Axis 1 moving simultaneously
      - PosFinal: Absolute position we are moving to
      - moveSpd:  Speed (mm/s) we are moving at
  */
  // Some vars
  int Tmove;         // How long are we running for (given dist & spd)
  float A1RevInc;    // How much is A1 motor incrementing?
  float A1RevFinal;  // Final A1 motor position (Revs)
  float A1RevPos;    // Temp pos of A1 motor as it moves (Revs)
  float A3RevInc;    // How much is A3 motor incrementing?
  float A3RevFinal;  // Final A3 motor position (Revs)
  float A3RevPos;    // Temp pos of A3 motor as it moves (Revs)
  bool Done = false; // For breaking the while loop
  bool Good2Move;    // Can we move?

  // Time
  unsigned long runTime;    // Current running time
  unsigned long startTime;  // The time we start moving
  unsigned long endTime;    // The time we expect to end

  /// Do a bunch of checks to see if we're good to go
  Good2Move = axisCal( 3 ) * axisHome( 3 ) * withinLims( 3 , PosFinal ) * axisCal( 1 );


  // Are we good to move it move it?
  if (Good2Move) {
    // I like to move it, move it
    //Serial.println("Moving!");


    // How long will the move take, for the given speed
    // Note:  - linespeed should ALWAYS be a factor of 100 AND <= 100.
    //          (1,2,4,5,10,20,25,50,100 mm/s)
    //        - resolution of dist = XXX.X (mm)
    //        -> therefor, Tmove ALWAYS an int (millisec)
    PosFinal = roundNum( PosFinal , 1 );
    Tmove = (abs(A3Pos - PosFinal) * 1000) / moveSpd;

    // Calc total (final) motor increment (Rev)
    A1RevInc = A1Dir * ((PosFinal - A3Pos) * A1Rpmm);
    A3RevInc = A3Dir * ((PosFinal - A3Pos) * A3Rpmm);

    // Work off time:
    startTime = millis();        // The time we start moving
    endTime = startTime + Tmove;

    while ( !Done ) {
      // calc runtime:
      runTime = millis() - startTime;

      // calc motor Pos:
      A1RevPos = A1MotorPos + ( runTime * (A1RevInc / Tmove));
      A3RevPos = A3MotorPos + ( runTime * (A3RevInc / Tmove));

      // Send to pos:
      odrive1.SetPosition(1, A1RevPos);
      odrive2.SetPosition(0, A3RevPos);

      // If within dt of Tmax, send to final pos:
      if ( runTime >= Tmove - dt) {
        // Send to final position
        odrive1.SetPosition(1, (A1MotorPos + A1RevInc));
        odrive2.SetPosition(0, (A3MotorPos + A3RevInc));

        // Update some values:
        A1MotorPos = A1MotorPos + A1RevInc;
        A3MotorPos = A3MotorPos + A3RevInc;

        A3Pos = PosFinal;

        Done = true;
        break;
      }
      else {
        // Wait for motor to catch up and serial to clear
        delay(dt);
      }
    }
  }
  else {
    //Serial.println("Good2Move is false");
  }
}

void A6A1MoveAbs( float PosFinal , float moveSpd ) {
  /* Move Axis 6 to abs pos, with Axis 1 moving simultaneously
      - PosFinal: Absolute position we are moving to
      - moveSpd:  Speed (mm/s) we are moving at
  */
  // Some vars
  int Tmove;         // How long are we running for (given dist & spd)
  float A1RevInc;    // How much is A1 motor incrementing?
  float A1RevFinal;  // Final A1 motor position (Revs)
  float A1RevPos;    // Temp pos of A1 motor as it moves (Revs)
  float A6RevInc;    // How much is A3 motor incrementing?
  float A6RevFinal;  // Final A3 motor position (Revs)
  float A6RevPos;    // Temp pos of A3 motor as it moves (Revs)
  bool Done = false; // For breaking the while loop
  bool Good2Move;    // Can we move?

  // Time
  unsigned long runTime;    // Current running time
  unsigned long startTime;  // The time we start moving
  unsigned long endTime;    // The time we expect to end

  /// Do a bunch of checks to see if we're good to go
  Good2Move = axisCal( 6 ) * axisHome( 6 ) * withinLims( 6 , PosFinal ) * axisCal( 1 );


  // Are we good to move it move it?
  if (Good2Move) {
    // I like to move it, move it
    //Serial.println("Moving!");


    // How long will the move take, for the given speed
    // Note:  - linespeed should ALWAYS be a factor of 100 AND <= 100.
    //          (1,2,4,5,10,20,25,50,100 mm/s)
    //        - resolution of dist = XXX.X (mm)
    //        -> therefor, Tmove ALWAYS an int (millisec)
    PosFinal = roundNum( PosFinal , 1 );
    Tmove = (abs(A6Pos - PosFinal) * 1000) / moveSpd;

    // Calc total (final) motor increment (Rev)
    A1RevInc = -1 * A1Dir * ((PosFinal - A6Pos) * A1Rpmm);
    A6RevInc = A6Dir * ((PosFinal - A6Pos) * A6Rpmm);

    // Work off time:
    startTime = millis();        // The time we start moving
    endTime = startTime + Tmove;

    while ( !Done ) {
      // calc runtime:
      runTime = millis() - startTime;

      // calc motor Pos:
      A1RevPos = A1MotorPos + ( runTime * (A1RevInc / Tmove));
      A6RevPos = A6MotorPos + ( runTime * (A6RevInc / Tmove));

      // Send to pos:
      odrive1.SetPosition(1, A1RevPos);
      odrive3.SetPosition(0, A6RevPos);

      // If within dt of Tmax, send to final pos:
      if ( runTime >= Tmove - dt) {
        // Send to final position
        odrive1.SetPosition(1, (A1MotorPos + A1RevInc));
        odrive3.SetPosition(0, (A6MotorPos + A6RevInc));

        // Update some values:
        A1MotorPos = A1MotorPos + A1RevInc;
        A6MotorPos = A6MotorPos + A6RevInc;

        A6Pos = PosFinal;

        Done = true;
        break;
      }
      else {
        // Wait for motor to catch up and serial to clear
        delay(dt);
      }
    }
  }
  else {
    //Serial.println("Good2Move is false");
  }
}



void A3A4Arc( float thetaEnd ) {
  /* Move Axis 3 and 4 at an arc
  */


  // Some vars
  int Tmove;             // How long are we running for (given dist & spd)
  float A3PosFinal;      // Final position of A3
  float A3MotorPosFinal; // Final motor position of A3
  float dA3;             // Increment position of A3
  float A3RevFinal;      // Final A3 motor position (Revs)
  float A3RevPos;        // Temp pos of A3 motor as it moves (Revs)
  float A4PosFinal;      // Final position of A4
  float A4MotorPosFinal; // Final motor position of A4
  float dA4;             // Increment position of A3
  float A4RevFinal;      // Final A4 motor position (Revs)
  float A4RevPos;        // Temp pos of A4 motor as it moves (Revs)
  bool Done = false;     // For breaking the while loop
  bool Good2Move;        // Can we move?

  // Time
  unsigned long runTime;    // Current running time
  unsigned long startTime;  // The time we start moving
  unsigned long endTime;    // The time we expect to end

  // For geometry
  float R;            // Arc radius
  float theta;        // floating varible
  float thetaStart;   // Starting value of theta
  float thetaInc;     // Total theta increment
  float thetaRate;    // Angular velo

  /// Do some initial calcs
  R = sqrt((A3Pos * A3Pos) + (A4Pos * A4Pos));

  // theta calcs
  thetaEnd   = min2Rad(thetaEnd); 
  thetaRate  = arcspeed / R;             // v = wR
  thetaStart = tan( A4Pos / A3Pos );
  thetaInc   = thetaEnd - thetaStart;
  
  // End positions
  A3PosFinal      = R * cos( thetaEnd );
  A4PosFinal      = R * sin( thetaEnd );
  A3MotorPosFinal = A3MotorPos + (A3Dir * (A3PosFinal - A3Pos) * A3Rpmm);
  A4MotorPosFinal = A4MotorPos + (A4Dir * (A4PosFinal - A4Pos) * A4Rpmm);


  /// Do a bunch of checks to see if we're good to go
  Good2Move = axisCal( 3 ) * axisHome( 3 ) * axisCal( 4 ) * axisHome( 4 ) * withinLims( 3 , R ) * withinLims( 4 , A4PosFinal ) * withinLims( 0 , thetaEnd );

  // Are we good to move it move it?
  if (Good2Move) {
    // I like to move it, move it

    // How long will the move take, for the given speed
    // Note:  - linespeed should ALWAYS be a factor of 100 AND <= 100.
    //          (1,2,4,5,10,20,25,50,100 mm/s)
    //        - resolution of dist = XXX.X (mm)
    //        -> therefor, Tmove ALWAYS an int (millisec)
    A3PosFinal = roundNum( A3PosFinal , 1 );
    A4PosFinal = roundNum( A4PosFinal , 1 );
    Tmove =  1000 * abs(thetaInc)  / thetaRate;

    // Work off time:
    startTime = millis();        // The time we start moving
    endTime   = startTime + Tmove;

    while ( !Done ) {
      // calc runtime:
      runTime = millis() - startTime;

      // Calc change in position
      theta = thetaStart + (thetaInc * (float(runTime) / float(Tmove)));
      dA3   = R * ( cos(theta) - cos(thetaStart));
      dA4   = R * ( sin(theta) - sin(thetaStart));

      // Calc changes in motor position
      A3RevPos = A3MotorPos + (A3Dir * dA3 * A3Rpmm);
      A4RevPos = A4MotorPos + (A4Dir * dA4 * A4Rpmm);

      // Send to position
      odrive2.SetPosition(0, A3RevPos);
      delay(dt);
      odrive2.SetPosition(1, A4RevPos);

      // If within dt of Tmax, send to final pos:
      if ( runTime >= Tmove - dt) {
        // Send to final position
        odrive2.SetPosition(0, A3RevPos);
        odrive2.SetPosition(1, A4RevPos);

        // Update some values
        A3MotorPos = A3MotorPosFinal;
        A4MotorPos = A4MotorPosFinal;
        A3Pos = A3PosFinal;
        A4Pos = A4PosFinal;

        // End
        Done = true;
        break;
      }
      else {
        // Wait for motor to catch up and serial to clear
        delay(dt);
      }
    }
  }
}


void A6A7Arc( float thetaEnd ) {
  /* Move Axis 3 and 4 at an arc
  */


  // Some vars
  int Tmove;             // How long are we running for (given dist & spd)
  float A6PosFinal;      // Final position of A3
  float A6MotorPosFinal; // Final motor position of A3
  float dA6;             // Increment position of A3
  float A6RevFinal;      // Final A3 motor position (Revs)
  float A6RevPos;        // Temp pos of A3 motor as it moves (Revs)
  float A7PosFinal;      // Final position of A4
  float A7MotorPosFinal; // Final motor position of A4
  float dA7;             // Increment position of A3
  float A7RevFinal;      // Final A4 motor position (Revs)
  float A7RevPos;        // Temp pos of A4 motor as it moves (Revs)
  bool Done = false;     // For breaking the while loop
  bool Good2Move;        // Can we move?

  // Time
  unsigned long runTime;    // Current running time
  unsigned long startTime;  // The time we start moving
  unsigned long endTime;    // The time we expect to end

  // For geometry
  float R;            // Arc radius
  float theta;        // floating varible
  float thetaStart;   // Starting value of theta
  float thetaInc;     // Total theta increment
  float thetaRate;    // Angular velo

  /// Do some initial calcs
  R = sqrt((A6Pos * A6Pos) + (A7Pos * A7Pos));

  // theta calcs
  thetaEnd   = min2Rad(thetaEnd); 
  thetaRate  = arcspeed / R;             // v = wR
  thetaStart = tan( A7Pos / A6Pos );
  thetaInc   = thetaEnd - thetaStart;
  
  // End positions
  A6PosFinal      = R * cos( thetaEnd );
  A7PosFinal      = R * sin( thetaEnd );
  A6MotorPosFinal = A6MotorPos + (A6Dir * (A6PosFinal - A6Pos) * A6Rpmm);
  A7MotorPosFinal = A7MotorPos + (A7Dir * (A7PosFinal - A7Pos) * A7Rpmm);

  /// Do a bunch of checks to see if we're good to go
  Good2Move = axisCal( 6 ) * axisHome( 6 ) * axisCal( 7 ) * axisHome( 7 ) * withinLims( 6 , R ) * withinLims( 7 , A7PosFinal ) * withinLims( 0 , thetaEnd );

  // Are we good to move it move it?
  if (Good2Move) {
    // I like to move it, move it

    // How long will the move take, for the given speed
    // Note:  - linespeed should ALWAYS be a factor of 100 AND <= 100.
    //          (1,2,4,5,10,20,25,50,100 mm/s)
    //        - resolution of dist = XXX.X (mm)
    //        -> therefor, Tmove ALWAYS an int (millisec)
    A6PosFinal = roundNum( A6PosFinal , 1 );
    A7PosFinal = roundNum( A7PosFinal , 1 );
    Tmove =  1000 * abs(thetaInc)  / thetaRate;

    // Work off time:
    startTime = millis();        // The time we start moving
    endTime   = startTime + Tmove;

    while ( !Done ) {
      // calc runtime:
      runTime = millis() - startTime;

      // Calc change in position
      theta = thetaStart + (thetaInc * (float(runTime) / float(Tmove)));
      dA6   = R * ( cos(theta) - cos(thetaStart));
      dA7   = R * ( sin(theta) - sin(thetaStart));

      // Calc changes in motor position
      A6RevPos = A6MotorPos + (A6Dir * dA6 * A6Rpmm);
      A7RevPos = A7MotorPos + (A7Dir * dA7 * A7Rpmm);

      // Send to position
      odrive3.SetPosition(0, A6RevPos);
      odrive3.SetPosition(1, A7RevPos);

      // If within dt of Tmax, send to final pos:
      if ( runTime >= Tmove - dt) {
        // Send to final position
        odrive3.SetPosition(0, A6RevPos);
        delay(dt);
        odrive3.SetPosition(1, A7RevPos);

        // Update some values
        A6MotorPos = A6MotorPosFinal;
        A7MotorPos = A7MotorPosFinal;
        A6Pos = A6PosFinal;
        A7Pos = A7PosFinal;

        // End
        Done = true;
        break;
      }
      else {
        // Wait for motor to catch up and serial to clear
        delay(dt);
      }
    }
  }
}

// - - - - - - - - - - - Calibration & Home - - - - - - - - - - - - - - \\

void calMotor ( int axis ) {
  /* Calibrate axes */

  // Temp var
  int requested_state;

  switch (axis) {
    case 1:
      // Calibrate Axis 1
      requested_state = ODriveArduino::AXIS_STATE_MOTOR_CALIBRATION;
      odrive1.run_state(1, requested_state, true);
      requested_state = ODriveArduino::AXIS_STATE_ENCODER_OFFSET_CALIBRATION;
      odrive1.run_state(1, requested_state, true);
      requested_state = ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL;
      odrive1.run_state(1, requested_state, false); // don't wait

      delay(100);

      A1MotorPos = 0;
      A1Cal      = true;
      break;

    case 3:
      // Calibrate Axis 3
      requested_state = ODriveArduino::AXIS_STATE_MOTOR_CALIBRATION;
      odrive2.run_state(0, requested_state, true);
      requested_state = ODriveArduino::AXIS_STATE_ENCODER_OFFSET_CALIBRATION;
      odrive2.run_state(0, requested_state, true);
      requested_state = ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL;
      odrive2.run_state(0, requested_state, false); // don't wait

      delay(100);

      A3MotorPos = 0;
      A3Cal      = true;
      A3Home     = false;
      break;

    case 4:
      // Calibrate Axis 4
      requested_state = ODriveArduino::AXIS_STATE_MOTOR_CALIBRATION;
      odrive2.run_state(1, requested_state, true);
      requested_state = ODriveArduino::AXIS_STATE_ENCODER_OFFSET_CALIBRATION;
      odrive2.run_state(1, requested_state, true);
      requested_state = ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL;
      odrive2.run_state(1, requested_state, false); // don't wait

      delay(100);

      A4MotorPos = 0;
      A4Cal      = true;
      A4Home     = false;
      break;

    case 6:
      // Calibrate Axis 6
      requested_state = ODriveArduino::AXIS_STATE_MOTOR_CALIBRATION;
      odrive3.run_state(0, requested_state, true);
      requested_state = ODriveArduino::AXIS_STATE_ENCODER_OFFSET_CALIBRATION;
      odrive3.run_state(0, requested_state, true);
      requested_state = ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL;
      odrive3.run_state(0, requested_state, false); // don't wait

      delay(100);

      A6MotorPos = 0;
      A6Cal      = true;
      A4Home     = false;
      break;

    case 7:
      // Calibrate Axis 7
      requested_state = ODriveArduino::AXIS_STATE_MOTOR_CALIBRATION;
      odrive3.run_state(1, requested_state, true);
      requested_state = ODriveArduino::AXIS_STATE_ENCODER_OFFSET_CALIBRATION;
      odrive3.run_state(1, requested_state, true);
      requested_state = ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL;
      odrive3.run_state(1, requested_state, false); // don't wait

      delay(100);

      A7MotorPos = 0;
      A7Cal      = true;
      A4Home     = false;
      break;
  }
}

void A3HomeSeq() {
  /* This homes Axis 3
     - All values updated within this function
  */

  // Define some vars
  bool homed;             // Is home switch triggered?
  int extraMove = 15;     // How far to keep moving tool up if initially homed
  float TempPos;          // Temporary position to send motor to (mm)
  bool Good2Move;

  // Check if calibrated
  Good2Move = axisCal( 3 );

  if (Good2Move) {
    //Serial.println("Now starting homing sequence");

    // First, read if homed
    homed = axisHomeSwitch( 3 );

    // If it is already homed upon setup, we need to move it up
    // and beyond its home position, then enter the homing sequence.
    if (homed)
    {
      //Serial.println("We're already home, we need to move up");
      delay(500);
      while (homed)
      {
        TempPos = A3Pos + 1;
        A3MoveAbs( TempPos , homespeed , true );
        homed = axisHomeSwitch( 3 );
      }

      // Move that extra distance
      TempPos = A3Pos + extraMove;
      A3MoveAbs( TempPos , linespeed , true );

      //Serial.println("No longer homed");
      delay(500);
    }

    // Now begin the actual homing sequence
    //Serial.println("Now Starting homing sequence");
    delay(1000);

    while (!homed)
    {
      TempPos = A3Pos - 0.5;
      A3MoveAbs( TempPos , homespeed , true );
      homed = axisHomeSwitch( 3 );
    }

    //Serial.println("Homing Triggered");
    delay(500);

    // Update
    A3Home = true;
    A3Pos  = A3HomePos;
  }
  else {
    //Serial.println("Good2Move is false");
  }
}

void A4HomeSeq() {
  /* This homes Axis 4
     - All values updated within this function
  */

  // Define some vars
  bool homed;             // Is home switch triggered?
  int extraMove = 15;     // How far to keep moving tool up if initially homed
  float TempPos;          // Temporary position to send motor to (mm)
  bool Good2Move;

  // Check if calibrated
  Good2Move = axisCal( 4 );

  if (Good2Move) {
    //Serial.println("Now starting homing sequence");

    // First, read if homed
    homed = axisHomeSwitch( 4 );

    // If it is already homed upon setup, we need to move it up
    // and beyond its home position, then enter the homing sequence.
    if (homed)
    {
      //Serial.println("We're already home, we need to move up");
      delay(500);
      while (homed)
      {
        TempPos = A4Pos - 1;
        A4MoveAbs( TempPos , homespeed , true );
        homed = axisHomeSwitch( 4 );
      }

      // Move that extra distance
      TempPos = A4Pos - extraMove;
      A4MoveAbs( TempPos , linespeed , true );

      //Serial.println("No longer homed");
      delay(500);
    }

    // Now begin the actual homing sequence
    //Serial.println("Now Starting homing sequence");
    delay(1000);

    while (!homed)
    {
      TempPos = A4Pos + 0.5;
      A4MoveAbs( TempPos , homespeed , true );
      homed = axisHomeSwitch( 4 );
    }

    //Serial.println("Homing Triggered");
    delay(500);

    // Update
    A4Home = true;
    A4Pos  = A4HomePos;
  }
  else {
    //Serial.println("Good2Move is false");
  }
}

void A6HomeSeq() {
  /* This homes Axis 6
     - All values updated within this function
  */

  // Define some vars
  bool homed;             // Is home switch triggered?
  int extraMove = 15;     // How far to keep moving tool up if initially homed
  float TempPos;          // Temporary position to send motor to (mm)
  bool Good2Move;

  // Check if calibrated
  Good2Move = axisCal( 6 );

  if (Good2Move) {
    //Serial.println("Now starting homing sequence");

    // First, read if homed
    homed = axisHomeSwitch( 6 );

    // If it is already homed upon setup, we need to move it up
    // and beyond its home position, then enter the homing sequence.
    if (homed)
    {
      //Serial.println("We're already home, we need to move up");
      delay(500);
      while (homed)
      {
        TempPos = A6Pos + 1;
        A6MoveAbs( TempPos , homespeed , true );
        homed = axisHomeSwitch( 6 );
      }

      // Move that extra distance
      TempPos = A6Pos + extraMove;
      A6MoveAbs( TempPos , linespeed , true );

      //Serial.println("No longer homed");
      delay(500);
    }

    // Now begin the actual homing sequence
    //Serial.println("Now Starting homing sequence");
    delay(1000);

    while (!homed)
    {
      TempPos = A6Pos - 0.5;
      A6MoveAbs( TempPos , homespeed , true );
      homed = axisHomeSwitch( 6 );
    }

    //Serial.println("Homing Triggered");
    delay(500);

    // Update
    A6Home = true;
    A6Pos  = A6HomePos;
  }
  else {
    //Serial.println("Good2Move is false");
  }
}

void A7HomeSeq() {
  /* This homes Axis 7
     - All values updated within this function
  */

  // Define some vars
  bool homed;             // Is home switch triggered?
  int extraMove = 15;     // How far to keep moving tool up if initially homed
  float TempPos;          // Temporary position to send motor to (mm)
  bool Good2Move;

  // Check if calibrated
  Good2Move = axisCal( 7 );

  if (Good2Move) {
    //Serial.println("Now starting homing sequence");

    // First, read if homed
    homed = axisHomeSwitch( 7 );

    // If it is already homed upon setup, we need to move it up
    // and beyond its home position, then enter the homing sequence.
    if (homed)
    {
      //Serial.println("We're already home, we need to move up");
      delay(500);
      while (homed)
      {
        TempPos = A7Pos - 1;
        A7MoveAbs( TempPos , homespeed , true );
        homed = axisHomeSwitch( 7 );
      }

      // Move that extra distance
      TempPos = A7Pos - extraMove;
      A7MoveAbs( TempPos , linespeed , true );

      //Serial.println("No longer homed");
      delay(500);
    }

    // Now begin the actual homing sequence
    //Serial.println("Now Starting homing sequence");
    delay(1000);

    while (!homed)
    {
      TempPos = A7Pos + 0.5;
      A7MoveAbs( TempPos , homespeed , true );
      homed = axisHomeSwitch( 7 );
    }

    //Serial.println("Homing Triggered");
    delay(500);

    // Update
    A7Home = true;
    A7Pos  = A7HomePos;
  }
  else {
    //Serial.println("Good2Move is false");
  }
}

bool axisHomeSwitch( int axis ) {
  // Due to the wriring set up, it is hard to determine if the switch is high or low
  // This function returns:  true - when switch is on
  //                         false - when switch is off
  // Ten readings are taken at set intervals, if all these are true, then return true val
  // This might also go some way to eliminate mechanical bounce

  int n  = 10;   // Number of readings
  int dt = 20;   // Reading interval

  int axisPin;
  bool homed = true;

  // Sort axis
  switch (axis) {
    case 3:
      axisPin = A3HomePin;
      break;
    case 4:
      axisPin = A4HomePin;
      break;
    case 6:
      axisPin = A6HomePin;
      break;
    case 7:
      axisPin = A7HomePin;
      break;
  }

  // Collect data
  for (int i = 1; i <= n; i++) {
    homed = homed * digitalRead(axisPin);
    delay(dt);
  }

  if (homed) {
    return false;
  }
  else       {
    return true;
  }

}

// - - - - - - - - - - - Checking Functions - - - - - - - - - - - - - - \\

bool axisHome( int axis ) {
  /* This checks if axis is homed
   *    * 0 - axis is not homed
   *    * 1 - axis is homed
  */

  switch (axis) {
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
    default:
      return false;
      //Serial.println("Numpty");
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
      //Serial.println("Numpty");
      break;
  }
}


bool withinLims( int axis , float Pos ) {
  /* This checks the lims of an axis
   *    * 0 - Pos is outside of lims
   *    * 1 - Pos is inside of lims
  */

  switch (axis) {
    case 0:
      if (Pos <= tan(A4MinPos / A3MinPos) || Pos >= tan(A4MaxPos / A3MinPos)) {
        return false;
      }
      else {
        return true;
      }
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
    default:
      return false;
      //Serial.println("Numpty");
      break;
  }
}



// - - - - - - - - - - - Miscellaneous - - - - - - - - - - - - - - \\


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


void blinkLED( int n , float dt ) { 
  for (int i = 1; i <= n; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(dt); 
    digitalWrite(LED_BUILTIN, LOW);
    delay(dt); 
  }
}


float min2Rad( float minIn ) {
  // Convert minute to radian
  float radOut;
  return radOut = (3.141 / (60*180)) * minIn;
}

// - - - - - - - - - - - Messages - - - - - - - - - - - - - - \\


void sendUpdateMsg() {
  Serial1.print("d");
}




void resetModeCommand(ModeStatus * machineStatus)
{
  // Reset NewCommand ready for the next
  machineStatus->mode   = 'a';
  machineStatus->value  = 0;
  machineStatus->active = false;

  sendUpdateMsg();
}
