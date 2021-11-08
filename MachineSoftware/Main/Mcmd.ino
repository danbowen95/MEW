/* M commands */


void M00() {
  /* Print All M Commands*/
  Serial.println("All Known 'M' commands");
}

void M03() {
  
}


void M10() {
  /* Calibrate ALL odrive axes */

}

void M11() {
  /* Calibrate A1 */
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

  // Now Update
  A1Pos = 0;       // Axis current actual position (mm)
  A1Cal = true;    // Axis calibration
}


void M13() {
  /* Calibrate A3 */
  int requested_state; // Temp var

  Serial.println("Axis 3 Cal started");
  requested_state = ODriveArduino::AXIS_STATE_MOTOR_CALIBRATION;
  Serial << "Axis" << 0 << ": Requesting state " << requested_state << '\n';
  odrive2.run_state(0, requested_state, true);
  requested_state = ODriveArduino::AXIS_STATE_ENCODER_OFFSET_CALIBRATION;
  Serial << "Axis" << 0 << ": Requesting state " << requested_state << '\n';
  odrive2.run_state(0, requested_state, true);
  requested_state = ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL;
  Serial << "Axis" << 0 << ": Requesting state " << requested_state << '\n';
  odrive2.run_state(0, requested_state, false); // don't wait
  Serial.println("Axis 1 Cal Done!");
  delay(100);

  // Now Update
  A3MotorPos = 0;  // Axis motor position (mm)
  A3Cal = true;    // Axis calibration
  A3Home = false;  // Axis is not homed
}


//void M14() {
//  /* Calibrate A4 */
//  int requested_state; // Temp var
//
//  Serial.println("Axis 4 Cal started");
//  requested_state = ODriveArduino::AXIS_STATE_MOTOR_CALIBRATION;
//  Serial << "Axis" << 1 << ": Requesting state " << requested_state << '\n';
//  odrive2.run_state(1, requested_state, true);
//  requested_state = ODriveArduino::AXIS_STATE_ENCODER_OFFSET_CALIBRATION;
//  Serial << "Axis" << 1 << ": Requesting state " << requested_state << '\n';
//  odrive2.run_state(1, requested_state, true);
//  requested_state = ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL;
//  Serial << "Axis" << 1 << ": Requesting state " << requested_state << '\n';
//  odrive2.run_state(1, requested_state, false); // don't wait
//  delay(100);
//
//  // Now Update
//  A4Pos = 0;       // Axis current actual position (mm)
//  A4Cal = true;    // Axis calibration
//}
//
//void M16() {
//  /* Calibrate A6 */
//  int requested_state; // Temp var
//
//  Serial.println("Axis 6 Cal started");
//  requested_state = ODriveArduino::AXIS_STATE_MOTOR_CALIBRATION;
//  Serial << "Axis" << 0 << ": Requesting state " << requested_state << '\n';
//  odrive3.run_state(0, requested_state, true);
//  requested_state = ODriveArduino::AXIS_STATE_ENCODER_OFFSET_CALIBRATION;
//  Serial << "Axis" << 0 << ": Requesting state " << requested_state << '\n';
//  odrive3.run_state(0, requested_state, true);
//  requested_state = ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL;
//  Serial << "Axis" << 0 << ": Requesting state " << requested_state << '\n';
//  odrive3.run_state(0, requested_state, false); // don't wait
//  Serial.println("Axis 1 Cal Done!");
//  delay(100);
//
//  // Now Update
//  A6Pos = 0;       // Axis current actual position (mm)
//  A6Cal = true;    // Axis calibration
//}
//
//void M17() {
//  /* Calibrate A7 */
//  int requested_state; // Temp var
//
//  Serial.println("Axis 4 Cal started");
//  requested_state = ODriveArduino::AXIS_STATE_MOTOR_CALIBRATION;
//  Serial << "Axis" << 1 << ": Requesting state " << requested_state << '\n';
//  odrive3.run_state(1, requested_state, true);
//  requested_state = ODriveArduino::AXIS_STATE_ENCODER_OFFSET_CALIBRATION;
//  Serial << "Axis" << 1 << ": Requesting state " << requested_state << '\n';
//  odrive3.run_state(1, requested_state, true);
//  requested_state = ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL;
//  Serial << "Axis" << 1 << ": Requesting state " << requested_state << '\n';
//  odrive3.run_state(1, requested_state, false); // don't wait
//  delay(100);
//
//  // Now Update
//  A7Pos = 0;       // Axis current actual position (mm)
//  A7Cal = true;    // Axis calibration
//}
