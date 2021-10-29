/* M commands */

void M11() {

  int motornum = 1;
  int requested_state;

  Serial.println("Doing M11!!!!!!!");

//  requested_state = ODriveArduino::AXIS_STATE_MOTOR_CALIBRATION;
//  Serial2 << "Axis" << 1 << ": Requesting state " << requested_state << '\n';
//  if (!odrive1.run_state(motornum, requested_state, true)) return;
//
//  //      requested_state = ODriveArduino::AXIS_STATE_ENCODER_OFFSET_CALIBRATION;
//  //      Serial << "Axis" << c << ": Requesting state " << requested_state << '\n';
//  //      if(!odrive.run_state(motornum, requested_state, true, 25.0f)) return;
//
//  requested_state = ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL;
//  Serial2 << "Axis" << 1 << ": Requesting state " << requested_state << '\n';
//  if (!odrive1.run_state(motornum, requested_state, false /*don't wait*/)) return;
}
