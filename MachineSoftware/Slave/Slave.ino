/*   = = = = = = = = = = = = = =
    | FIRMWARE FOR SLAVE TEENSY |
     = = = = = = = = = = = = = =

    This sketch is to be uploaded to the Slave Teensy - Teensy 4.1.
    See full documentation in 'Chapter 6 - Mechanising the English Wheel'
    for more info on opperating the machine.

*  ** WARNINGS **
    ! ! When uploading code to this controller, ensure no ground loops are formed ! !

    Documentation Contents:
     - Intro
     - Warnings!
     - Documentation Contents
     - Code Structure
     - Pin Assignments
     - Recognised Commands
     - Other Notes

    Code Structure:
    - Preamble
    - setup()
    - loop()
    - DrivetoSpeed()
    - Encoders()


    Pin Assignments:
    GND - GND of 5v rail
     0  - RX1 to TX1 of master Teensy
     1  - Tx1 to RX1 of master Teensy
     2  - Not used
     3  - LED (Grn) for serial connection
     4  - Not used
     5  - Not Used
     6  - Not Used
     7  - Not Used
     8  - Dig (Blu) to A5 vacuum relay
     9  - LED (Grn)
     10 - Not Used
     11 - Dig (Blu) to A8 vacuum relay
     12 - LED (Grn)
     3v - Not Used
     24 - PWM (Gry) to A5 driver
     25 - PWM (Yel) to A5 driver
     26 - Ang (Blu) to A5 sensor
     27 - Dig (Yel) to A5 home sensor
     28 - Dig (Grn) to A5 encoder (A)
     29 - Dig (Yel) to A5 encoder (B)
     30 - Not Used
     31 - LED (Grn)
     32 - Not Used
    - - - - - - - - - - - -
    Vin - 5v rail
    GND - Not used
    3v  - Not used (?)
     23 - PWM (Grn) to A2 driver
     22 - PWM (Yel) to A2 driver
     21 - Dig (Grn) to A2 encoder (A)
     20 - Dig (Yel) to A2 encoder (B)
     19 - Dig (Yel) to A2 home sensor
     18 - Not Used
     17 - Not Used
     16 - Not Used
     15 - Not Used
     14 - Not Used
     13 - Not Used
    GND -
     41 - PWM (Gry) to A8 driver
     40 - PWM (Yel) to A8 driver
     39 - Ang (Blu) to A8 sensor
     38 - Dig (Yel) to A8 home sensor
     37 - Dig (Grn) to A8 encoder (A)
     36 - Dig (Yel) to A8 encoder (B)
     35 - Not Used
     34 - LED (Grn)
     33 - Not Used

    Recognised Commands from Serial in:

      a - Do nothing
   bxxx - Move A2 incrementally (mm)
   cxxx - Move A5 incrementally (mm)
   dxxx - Move A8 incrementally (mm)
   exxx - Move A2 to absolute position (mm)
   fxxx - Move A5 to absolute position (mm)
   gxxx - Move A8 to absolute position (mm)
      h - Home A2
      i - Home A5
      j - Home A8
      k - A5 active control ON
      l - A5 active control OFF
      m - A8 active control ON
      n - A8 active control OFF
      o - A2 Tool change


    Some Notes:
    - Interupts on teensy work slightly differently to on Arduino
    - Once left active control (A5 or A8), you'll need to put
      it back into active control
    - DEFG E C D
*/

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \\
// * Preamble * \\
//
#include <PID_v1.h>
#include "structsSlave.h"

// Function forward declaration
void printCommand(ModeStatus* machineStatus);
void resetModeCommand(ModeStatus * machineStatus);

/// Varibles & Pin assignments
// Structures
ModeStatus machineStatus;      // What mode are we running?
slaveStatusStruct slaveStatus; // What is the status of A2,3,4

// For serial coms with master Teensy
const unsigned int MAX_MESSAGE_LENGTH = 12; // Max length of serial msg that can be sent
char newValueSerial[12];                    // What the latest serial value
char newModeSerial[3];                      // What the latest serial mode
bool newCommandSerialBool = false;          // Has there been a new command

// Axis 2
int A2_Home = 19;                 // Home Pin
int A2_RPWM = 23;                 // RPWM Pin
int A2_LPWM = 22;                 // LPWM Pin
int A2_encoderPinA = 20;          // encoder A Pin
int A2_encoderPinB = 21;          // encoder B Pin
volatile long A2_encoderPos = 0;  // encoder Pos Var
double A2_cpmm = 275808 / 385;    // encoder counts per mm of tool = 716.38441...
// A2_cpr = 16 (counts/rev_enc) * 11492/77(rev_enc/rev_drv) * 24/20 (rev_drv/rev_srw) * 4 (mm/rev_srw)
int A2_tol = 20;               // tol of position accuracy (counts)
int A2_P = 0.5;                // Proportional gain
int A2_I = 0;                  // Integrator gain
int A2_D = 0.08;               // Derivitive gain
// PID Vars:
double A2_Setpoint, A2_Input, A2_Output, A2_OutputA; // Varibles wheel
PID A2_PID(&A2_Input, &A2_Output, &A2_Setpoint, A2_P, A2_I, A2_D, DIRECT);

//// Axis 5
//int A5_RPWM = 6;                  // RPWM Pin
//int A5_LPWM = 7;                  // LPWM Pin
//int A5_encoderPinA = 8;           // encoder A Pin
//int A5_encoderPinB = 9;           // encoder B Pin
//volatile long A5_encoderPos = 0;  // encoder Pos Var
//int A5_cpr = 40;                  // Counts per rev
//int A5_ptc = 3;                   // screw pitch
//double A5_tol = 20;               // tol of position accuracy
//double A5_P = 0.5;                // Proportional gain
//double A5_I = 0;                  // Integrator gain
//double A5_D = 0.08;               // Derivitive gain
//// PID Vars:
//double A5_Setpoint, A5_Input, A5_Output, A5_OutputA;
//PID A5_PID(&A5_Input, &A5_Output, &A5_Setpoint, A5_P, A5_I, A5_D, DIRECT);
//
//// Axis 8
//int A8_RPWM = 10;                 // RPWM Pin
//int A8_LPWM = 11;                 // LPWM Pin
//int A8_encoderPinA = 14;          // encoder A Pin
//int A8_encoderPinB = 15;          // encoder B Pin
//volatile long A8_encoderPos = 0;  // encoder Pos Var
//int A8_cpr = 40;                  // Counts per rev
//int A8_ptc = 3;                   // screw pitch
//double A8_tol = 20;               // tol of position accuracy
//double A8_P = 0.5;                // Proportional gain
//double A8_I = 0;                  // Integrator gain
//double A8_D = 0.08;               // Derivitive gain
//// PID Vars:
//double A8_Setpoint, A8_Input, A8_Output, A8_OutputA; // Varibles wheel
//PID A8_PID(&A8_Input, &A8_Output, &A8_Setpoint, A8_P, A8_I, A8_D, DIRECT);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \\
// Setup
void setup() {
  // start serial
  Serial.begin(9600);

  // Initialise structures
  resetModeCommand(&machineStatus);
  initializeSlaveMachineStatus(&slaveStatus);

  // Assignments
  // Axis 2
  pinMode(A2_RPWM, OUTPUT); // Driver
  pinMode(A2_LPWM, OUTPUT);
  pinMode(A2_encoderPinA, INPUT_PULLUP); // Encoder
  pinMode(A2_encoderPinB, INPUT_PULLUP);
  attachInterrupt(A2_encoderPinA, A2_doEncoderA, CHANGE);
  attachInterrupt(A2_encoderPinA, A2_doEncoderB, CHANGE);
  pinMode(A2_Home, INPUT);  // Home sensor
  A2_PID.SetMode(AUTOMATIC);
  A2_PID.SetOutputLimits(-250, 250);
  A2_PID.SetSampleTime(20);

  //  // Axis 5
  //  pinMode(A5_RPWM, OUTPUT); // Driver
  //  pinMode(A5_LPWM, OUTPUT);
  //  pinMode(A5_encoderPinA, INPUT_PULLUP); // Encoder
  //  pinMode(A5_encoderPinB, INPUT_PULLUP);
  //  attachInterrupt(A5_encoderPinA, A5_doEncoderA, CHANGE);
  //  attachInterrupt(A5_encoderPinA, A5_doEncoderB, CHANGE);
  //  A5_PID.SetMode(AUTOMATIC);
  //  A5_PID.SetOutputLimits(-255, 255);
  //  A5_PID.SetSampleTime(20);
  //
  //  // Axis 8
  //  pinMode(A8_RPWM, OUTPUT); // Driver
  //  pinMode(A8_LPWM, OUTPUT);
  //  pinMode(A8_encoderPinA, INPUT_PULLUP); // Encoder
  //  pinMode(A8_encoderPinB, INPUT_PULLUP);
  //  attachInterrupt(A8_encoderPinA, A8_doEncoderA, CHANGE);
  //  attachInterrupt(A8_encoderPinA, A8_doEncoderB, CHANGE);
  //  A8_PID.SetMode(AUTOMATIC);
  //  A8_PID.SetOutputLimits(-255, 255);
  //  A8_PID.SetSampleTime(20);
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \\
// loop
void loop() {
  readSerial();

  processSerial();

  enactCommand();
}




//
//}
//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \\

//
//
//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \\
//// DriveToPos
//void DriveToPos(int Val , int AxisTemp) {
//  /* Drives 'AxsTemp' to pos Val
//
//  */
//  switch (AxisTemp) {
//    case 2:
//      {
//        while (abs(Val - A2_encoderPos) >= A2_tol) {
//          // Update PID
//          A2_Input = A2_encoderPos;
//          A2_Setpoint = Val;
//          A2_PID.Compute();
//          DriveToSpeed(A2_Output, 2);
//        }
//        DriveToSpeed(0, 2);
//      }
//      break;
//    case 5:
//      {
//        while (abs(Val - A5_encoderPos) >= A5_tol) {
//          // Update PID
//          A5_Input = A5_encoderPos;
//          A5_Setpoint = Val;
//          A5_PID.Compute();
//          DriveToSpeed(A5_Output, 5);
//        }
//        DriveToSpeed(0, 5);
//      }
//      break;
//    case 8:
//      {
//        while (abs(Val - A8_encoderPos) >= A8_tol) {
//          // Update PID
//          A8_Input = A8_encoderPos;
//          A8_Setpoint = Val;
//          A8_PID.Compute();
//          DriveToSpeed(A8_Output, 8);
//        }
//        DriveToSpeed(0, 8);
//      }
//      break;
//    default:
//      // statements
//      break;
//  }
//}
//
