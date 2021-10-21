void homeA2()
{
  /* This homes Axis 2 */

  // Define some vars
  int homespeed = 100; // The speed at which homing occurs
  bool homed;          // Is home switch triggered?
  int delaytime = 1000; // How long to keep moving tool up if initially homed

  // First, read if homed
  homed = digitalRead(A2_Home);

  // If it is already homed upon setup, we need to move it up
  // and beyond its home position, then enter the homing sequence.
  if (homed)
  {
    Serial.println("Home on, going to move up");
    delay(500);
    while (homed == true)
    {
      DriveToSpeed( -1 * homespeed , 2 );
      homed = digitalRead(A2_Home);
    }

    delay(delaytime);
    DriveToSpeed( 0 , 2 );

    Serial.println("No longer homed");
    delay(500);
  }


  // Now begin the actual homing sequence
  Serial.println("Now Starting homing sequence");
  delay(1000);
  while (!homed)
  {
    DriveToSpeed( homespeed , 2 );
    homed = digitalRead(A2_Home);
  }
  Serial.println("Homing Triggered");
  DriveToSpeed( 0 , 2 );

  // Now update and reset
  slaveStatus.A2Home = true;
  slaveStatus.A2Pos  = 0;
  resetModeCommand(&machineStatus);

}
