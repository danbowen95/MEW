/* This sketch is to be uploaded to the Slave Teensy - Teensy 4.0.
    See full documentation for more info

    Code Structure
    - Preamble
    - setup()
    - loop()
    - DrivetoSpeed
    - Encoders

 *  ** WARNING **
    When uploading code to this controller..

    Pin Assignments:
    0  - RX1 to TXxxx of master Teensy
    1  - Tx1 to RXxxx of master Teensy
    2  - PWM (Gry) to A2 driver
    3  - PWM (Yel) to A2 driver
    4  - Dig (Grn) to A2 encoder (A)
    5  - Dig (Yel) to A2 encoder (B)
    6  - PWM (Gry) to A5 driver
    7  - PWM (Yel) to A5 driver
    8  - Dig (Grn) to A5 encoder (A)
    9  - Dig (Yel) to A5 encoder (B)
    10 - PWM (Gry) to A8 driver
    11 - PWM (Yel) to A8 driver
    12 - Not Used
    - - - - - - - - - - - -
 *  *Note upward direction of pins on board*
    13 - Not Used
    14 - Dig (Yel) to A2 encoder (B)
    15 - Dig (Grn) to A2 encoder (A)
    16 - Dig (Blu) to A5 vacuum relay
    17 - Dig (Blu) to A5 vacuum relay
    18 - LED (Grn)
    19 - Dig (Yel?) to A2 home sensor
    20 - Dig (Yel?) to A5 home sensor
    21 - Dig (Yel?) to A6 home sensor
    22 - Ang (Blu) to A5 sensor
    23 - Ang (Blu) to A8 sensor

    Serial in:
    h2,h5,h8 - home axis 2, axis 5, axis 8
    m2xxx    - move axis 2 (requires homing)
    t5, t8   - move axis 5, axis 8 to top
    v5x, v8x - vacuume axis 5, axis 8 on(x=1), off(x=0)
    a5, a8 - active axis
    

    Some Notes:
    - Interupts on teensy work slightly differently to on Arduino
    - DEFG E C D
*/

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \\
// * Preamble * \\
//
#include <PID_v1.h>

// Serial Connection to Master Controller

// Axis 2
bool A2_Cal = false;              // Axis calibrated 
int A2_RPWM = 2;                  // RPWM Pin
int A2_LPWM = 3;                  // LPWM Pin
int A2_encoderPinA = 4;           // encoder A Pin
int A2_encoderPinB = 5;           // encoder B Pin
volatile long A2_encoderPos = 0;  // encoder Pos Var
int A2_cpr = 40;                  // Counts per rev
int A2_ptc = 3;                   // screw pitch
double A2_tol = 20;               // tol of position accuracy
double A2_P = 0.5;                // Proportional gain
double A2_I = 0;                  // Integrator gain
double A2_D = 0.08;               // Derivitive gain
// PID Vars:
double A2_Setpoint, A2_Input, A2_Output, A2_OutputA; // Varibles wheel
PID A2_PID(&A2_Input, &A2_Output, &A2_Setpoint, A2_P, A2_I, A2_D, DIRECT);



// Axis 5
bool A5_Cal = false;              // Axis calibrated
int A5_RPWM = 6;                  // RPWM Pin
int A5_LPWM = 7;                  // LPWM Pin
int A5_encoderPinA = 8;           // encoder A Pin
int A5_encoderPinB = 9;           // encoder B Pin
volatile long A5_encoderPos = 0;  // encoder Pos Var
int A5_cpr = 40;                  // Counts per rev
int A5_ptc = 3;                   // screw pitch
double A5_tol = 20;               // tol of position accuracy
double A5_P = 0.5;                // Proportional gain
double A5_I = 0;                  // Integrator gain
double A5_D = 0.08;               // Derivitive gain
// PID Vars:
double A5_Setpoint, A5_Input, A5_Output, A5_OutputA;
PID A5_PID(&A5_Input, &A5_Output, &A5_Setpoint, A5_P, A5_I, A5_D, DIRECT);

// Axis 8
bool A8_Cal = false;              // Axis calibrated
int A8_RPWM = 10;                 // RPWM Pin
int A8_LPWM = 11;                 // LPWM Pin
int A8_encoderPinA = 14;          // encoder A Pin
int A8_encoderPinB = 15;          // encoder B Pin
volatile long A8_encoderPos = 0;  // encoder Pos Var
int A8_cpr = 40;                  // Counts per rev
int A8_ptc = 3;                   // screw pitch
double A8_tol = 20;               // tol of position accuracy
double A8_P = 0.5;                // Proportional gain
double A8_I = 0;                  // Integrator gain
double A8_D = 0.08;               // Derivitive gain
// PID Vars:
double A8_Setpoint, A8_Input, A8_Output, A8_OutputA; // Varibles wheel
PID A8_PID(&A8_Input, &A8_Output, &A8_Setpoint, A8_P, A8_I, A8_D, DIRECT);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \\
// Setup
void setup() {
  // Pin Assignments
  // Axis 2
  pinMode(A2_RPWM, OUTPUT); // Driver
  pinMode(A2_LPWM, OUTPUT);
  pinMode(A2_encoderPinA, INPUT_PULLUP); // Encoder
  pinMode(A2_encoderPinB, INPUT_PULLUP);
  attachInterrupt(A2_encoderPinA, A2_doEncoderA, CHANGE);
  attachInterrupt(A2_encoderPinA, A2_doEncoderB, CHANGE);
  A2_PID.SetMode(AUTOMATIC);
  A2_PID.SetOutputLimits(-255, 255);
  A2_PID.SetSampleTime(20);

  // Axis 5
  pinMode(A5_RPWM, OUTPUT); // Driver
  pinMode(A5_LPWM, OUTPUT);
  pinMode(A5_encoderPinA, INPUT_PULLUP); // Encoder
  pinMode(A5_encoderPinB, INPUT_PULLUP);
  attachInterrupt(A5_encoderPinA, A5_doEncoderA, CHANGE);
  attachInterrupt(A5_encoderPinA, A5_doEncoderB, CHANGE);
  A5_PID.SetMode(AUTOMATIC);
  A5_PID.SetOutputLimits(-255, 255);
  A5_PID.SetSampleTime(20);
  
  // Axis 8
  pinMode(A8_RPWM, OUTPUT); // Driver
  pinMode(A8_LPWM, OUTPUT);
  pinMode(A8_encoderPinA, INPUT_PULLUP); // Encoder
  pinMode(A8_encoderPinB, INPUT_PULLUP);
  attachInterrupt(A8_encoderPinA, A8_doEncoderA, CHANGE);
  attachInterrupt(A8_encoderPinA, A8_doEncoderB, CHANGE);
  A8_PID.SetMode(AUTOMATIC);
  A8_PID.SetOutputLimits(-255, 255);
  A8_PID.SetSampleTime(20);
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \\
// loop
void loop() {
  // put your main code here, to run repeatedly:

}

void HomeAxis( int AxisTemp ) {
  /* This function homes the 'AxisTemp' Axis.
   *  
   */

switch (AxisTemp) {
  case 2:
    // home axis 2
    break;
  case 5:
    // home axis 5
    break;
  case 8:
    // home axis 8
    break;
  default:
    // if nothing else matches, do the default
    // default is optional
    break;
}
  
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \\
// DriveToSpeed
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
          analogWrite(A2_RPWM, 0);
          analogWrite(A2_LPWM, Val);
        }
        else
        {
          // forward rotation
          analogWrite(A2_LPWM, 0);
          analogWrite(A2_RPWM, Val);
        }
      }
      break;
    case 5:
      {
        if (Val <= 0)
        {
          // reverse rotation
          analogWrite(A5_RPWM, 0);
          analogWrite(A5_LPWM, Val);
        }
        else
        {
          // forward rotation
          analogWrite(A5_LPWM, 0);
          analogWrite(A5_RPWM, Val);
        }
      }
      break;
    case 8:
      {
        if (Val <= 0)
        {
          // reverse rotation
          analogWrite(A8_RPWM, 0);
          analogWrite(A8_LPWM, Val);
        }
        else
        {
          // forward rotation
          analogWrite(A8_LPWM, 0);
          analogWrite(A8_RPWM, Val);
        }
      }
      break;
    default:
      // statements
      break;
  }
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \\
// DriveToPos
void DriveToPos(int Val , int AxisTemp) {
/* Drives 'AxsTemp' to pos Val
 *  
 */
  switch (AxisTemp) {
    case 2:
      {
        while (abs(Val - A2_encoderPos) >= A2_tol) {
          // Update PID
          A2_Input = A2_encoderPos;
          A2_Setpoint = Val;
          A2_PID.Compute();
          DriveToSpeed(A2_Output, 2);
        }
        DriveToSpeed(0, 2);
      }
      break;
    case 5:
      {
        while (abs(Val - A5_encoderPos) >= A5_tol) {
          // Update PID
          A5_Input = A5_encoderPos;
          A5_Setpoint = Val;
          A5_PID.Compute();
          DriveToSpeed(A5_Output, 5);
        }
        DriveToSpeed(0, 5);
      }
      break;
    case 8:
      {
        while (abs(Val - A8_encoderPos) >= A8_tol) {
          // Update PID
          A8_Input = A8_encoderPos;
          A8_Setpoint = Val;
          A8_PID.Compute();
          DriveToSpeed(A8_Output, 8);
        }
        DriveToSpeed(0, 8);
      }
      break;
    default:
      // statements
      break;
  }
}















// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \\
// Encoders
// includes - A2a, A2b, A5a, A5b, A8a, A8b
//
// A2a
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
}
//
// A5a
void A5_doEncoderA() {
  if (digitalRead(A5_encoderPinA) == HIGH) {
    if (digitalRead(A5_encoderPinB) == LOW) {
      A5_encoderPos = A5_encoderPos + 1;         // CW
    }
    else {
      A5_encoderPos = A5_encoderPos - 1;         // CCW
    }
  }
  else
  {
    if (digitalRead(A5_encoderPinB) == HIGH) {
      A5_encoderPos = A5_encoderPos + 1;          // CW
    }
    else {
      A5_encoderPos = A5_encoderPos - 1;          // CCW
    }
  }
}
//
// A5b
void A5_doEncoderB() {
  if (digitalRead(A5_encoderPinB) == HIGH) {
    if (digitalRead(A5_encoderPinA) == HIGH) {
      A5_encoderPos = A5_encoderPos + 1;         // CW
    }
    else {
      A5_encoderPos = A5_encoderPos - 1;         // CCW
    }
  }
  else {
    if (digitalRead(A5_encoderPinA) == LOW) {
      A5_encoderPos = A5_encoderPos + 1;          // CW
    }
    else {
      A5_encoderPos = A5_encoderPos - 1;          // CCW
    }
  }
}

//
// A8a
void A8_doEncoderA() {
  if (digitalRead(A8_encoderPinA) == HIGH) {
    if (digitalRead(A8_encoderPinB) == LOW) {
      A8_encoderPos = A8_encoderPos + 1;         // CW
    }
    else {
      A8_encoderPos = A8_encoderPos - 1;         // CCW
    }
  }
  else
  {
    if (digitalRead(A8_encoderPinB) == HIGH) {
      A8_encoderPos = A8_encoderPos + 1;          // CW
    }
    else {
      A8_encoderPos = A8_encoderPos - 1;          // CCW
    }
  }
}
//
// A5b
void A8_doEncoderB() {
  if (digitalRead(A8_encoderPinB) == HIGH) {
    if (digitalRead(A8_encoderPinA) == HIGH) {
      A8_encoderPos = A8_encoderPos + 1;         // CW
    }
    else {
      A8_encoderPos = A8_encoderPos - 1;         // CCW
    }
  }
  else {
    if (digitalRead(A8_encoderPinA) == LOW) {
      A8_encoderPos = A8_encoderPos + 1;          // CW
    }
    else {
      A8_encoderPos = A8_encoderPos - 1;          // CCW
    }
  }
}


void errorCodes( int n ){
  /* These display error codes for debugging.
   *  When running fine, red LED on pin XX does not flash
   *  When error, LED flashes. 
   *  Count the flashes between 2 sec long flash for error number
   *  
   *  Error codes
   *  1 - Not used
   *  2 - Home axis number not recognised
   *  3 - 
   */
}
