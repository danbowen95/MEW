// This runs the motor for 10 seconds in one direction at incrementing velocities

// Preamble
// includes
#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include <ODriveArduino.h>
// Printing with stream operator helper functions
template<class T> inline Print& operator <<(Print &obj,     T arg) {
  obj.print(arg);
  return obj;
}
template<>        inline Print& operator <<(Print &obj, float arg) {
  obj.print(arg, 4);
  return obj;
}


////////////////////////////////
// Set up serial pins to the ODrive
////////////////////////////////

// Teensy 3
// pin 0: RX - connect to ODrive TX (GPIO 1)
// pin 1: TX - connect to ODrive RX (GPIO 1)
// See https://www.pjrc.com/teensy/td_uart.html for other options on Teensy
HardwareSerial& odrive_serial = Serial1;

// ODrive object
ODriveArduino odrive(odrive_serial);


void setup() {
  // ODrive uses 115200 baud
  odrive_serial.begin(115200);

  // Serial to PC
  Serial.begin(115200);

  while (!Serial) ; // wait for Arduino Serial Monitor to open
  Serial.println("Hello!");

  // Read the voltage
  odrive_serial << "r vbus_voltage\n";
  Serial.print("Bus voltage: ");
  Serial.println(odrive.readFloat());

  // Run calibration sequence
  Serial.println("Running Calibration Sequence");

  int motornum = 0;
  int requested_state;

  requested_state = ODriveArduino::AXIS_STATE_MOTOR_CALIBRATION;
  if (!odrive.run_state(motornum, requested_state, true)) return; // These are the lines that wait for calibration to complete

  requested_state = ODriveArduino::AXIS_STATE_ENCODER_OFFSET_CALIBRATION;
  if (!odrive.run_state(motornum, requested_state, true, 25.0f)) return;
  Serial.println("Calibration Sequence DONE");

  Serial.println("Putting Motor into 'Closed Loop Control'");
  requested_state = ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL;
  if (!odrive.run_state(motornum, requested_state, false /*don't wait*/)) return;
  Serial.println("Motor in 'Closed Loop Control'");

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    int c = Serial.parseInt();
    Serial.println(c);

    if (c == 1) {
      Serial.println("I am here");
      }
    else {
      Serial.println("I am there");
    }
    delay(1000);
  if (c == 0) return;
  }
}





// Set axis.controller.config.control_mode = CONTROL_MODE_VELOCITY_CONTROL.
// You can now control the velocity with axis.controller.input_vel = 1 [turn/s].
