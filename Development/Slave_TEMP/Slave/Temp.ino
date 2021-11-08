void moveup()
{
  DriveToSpeed( -200 , 5 );
  delay(500);
  DriveToSpeed( 0 , 5 );
  Serial.print("Now at: ");
  Serial.println(A5_encoderPos);
}

void movedown()
{
  DriveToSpeed( 200 , 5 );
  delay(500);
  DriveToSpeed( 0 , 5 );
  Serial.print("Now at: ");
  Serial.println(A5_encoderPos);
}

void A1Cal() {
  int requested_state; // Temp var

  Serial.println("Axis 1 Cal started");
  requested_state = ODriveArduino::AXIS_STATE_MOTOR_CALIBRATION;
  Serial << "Axis" << 1 << ": Requesting state " << requested_state << '\n';
  odrive1.run_state(1, requested_state, true);
  requested_state = ODriveArduino::AXIS_STATE_ENCODER_OFFSET_CALIBRATION;
  Serial << "Axis" << 1 << ": Requesting state " << requested_state << '\n';
  odrive1.run_state(1, requested_state, true);
  requested_state = ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL;
  Serial << "Axis" << 1 << ": Requesting state " << requested_state << '\n';
  odrive1.run_state(1, requested_state, false); // don't wait
  Serial.println("Axis 1 Cal Done!");
  delay(100);
}

void A1Move( float Val ) {

  int dir = abs(Val)/Val;
  
  float rev2mm = 9.73058;

  float nRev = abs(Val) / rev2mm;

  float incRev = 0.01;

  int inTot = nRev / incRev;

  float Pos;


  for (int i = 1; i <= inTot; i++) {
    Pos = A1Pos + dir * (i * incRev);
    Serial.print(" Now going to: ");
    Serial.println(Pos);
    odrive1.SetPosition(1, Pos); 
    delay(10);
  }

  A1Pos = Pos;
}
