// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \\
// * Preamble * \\
//
#include <PID_v1.h>

// For collecting data from serial
int read1;
char ident;

// Axis 2
int A2_Home = 19;                 // Home Pin
int A2_RPWM = 23;                 // RPWM Pin
int A2_LPWM = 22;                 // LPWM Pin
int A2_encoderPinA = 20;          // encoder A Pin
int A2_encoderPinB = 21;          // encoder B Pin
volatile long A2_encoderPos = 0;  // encoder Pos Var
int A2_tol = 50;                  // tol of position accuracy (counts)
double A2_cpmm = 275808 / 385;    // encoder counts per mm of tool = 716.38441...
double A2_P = 0.5;                   // Proportional gain
double A2_I = 0;                     // Integrator gain
double A2_D = 0.08;                  // Derivitive gain
// PID Vars:
double A2_Setpoint, A2_Input, A2_Output, A2_OutputA; // Varibles wheel
PID A2PID(&A2_Input, &A2_Output, &A2_Setpoint, A2_P, A2_I, A2_D, DIRECT);



// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \\
// Setup
void setup() {
  // start serial
  Serial.begin(9600);

  // Assignments
  // Axis 2
  pinMode(A2_RPWM, OUTPUT); // Driver
  pinMode(A2_LPWM, OUTPUT);
  pinMode(A2_encoderPinA, INPUT_PULLUP); // Encoder
  pinMode(A2_encoderPinB, INPUT_PULLUP);
  attachInterrupt(A2_encoderPinA, A2_doEncoderA, CHANGE);
  attachInterrupt(A2_encoderPinB, A2_doEncoderB, CHANGE);
  pinMode(A2_Home, INPUT);  // Home sensor
  A2PID.SetMode(AUTOMATIC);
  A2PID.SetOutputLimits(-250, 250);
  A2PID.SetSampleTime(20);

}


void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    read1 = Serial.parseInt();
    ident = Serial.read();
    // say what you got:
    //    Serial.print("Input Value: ");
    //    Serial.println(read1);

    if ((read1 >= -255) && (read1 <= 255)) {
      DriveToSpeed(read1,2);
    }
    else {
      //      Serial.println("Value outside of -255 and 255 you muppet");
    }
    Serial.println(A2_encoderPos);
  }
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
  }
}











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
