/*   = = = = = = = = = = = = = = = = =
    | FIRMWARE FOR SLAVE 1 CONTROLLER |
     = = = = = = = = = = = = = = = = =


    This sketch is to be uploaded to the Teensy 4.1.
    See full documentation in 'Mechanising the English Wheel'
    for more info on opperating the machine.

    ** WARNING **
    ! ! When uploading code to this controller, ensure no ground loops are formed ! !
    
   Pin Assignments:
    GND - GND --> 5v/12v rail
      0 - RX1 --> TX2 of master controller
      1 - TX1 --> RX2 of master controller
      2 - Not used
      3 - Not used
      4 - Not used
      5 - Not used 
      6 - Not used
      7 - Not used
      8 - Not used
      9 - Not used
     10 - Not used
     11 - Not used
     12 - Not used 
   3.3v - Not used
     24 - PWM --> A5 RPWM
     25 - PWM --> A5 LPWM
     26 - A12 --> A5 Sensor
     27 - I/O --> A5 Encoder A
     28 - I/O --> A5 Encoder B
     29 - Not used
     30 - Not used
     31 - Not used 
     32 - Not used
   - - - - - - - - - - - - - -
    Vin - Vin --> +5v
    GND - Not Used
   3.3v - Not used
     23 - PWM --> A2 RPWM
     22 - PWM --> A2 LPWM
     21 - I/O --> A2 Encoder B
     20 - I/O --> A2 Encoder A
     19 - I/O --> A2 Home
     18 - Not used
     17 - Not used
     16 - Not used
     15 - Not used 
     14 - Not used
     13 - Not used
    GND - Not used
     41 - Not used
     40 - I/O --> A8 Encoder A
     39 - I/O --> A8 Encoder B     
     38 - A12 --> A8 Sensor
     37 - PWM --> A8 RPWM
     36 - PWM --> A8 LPWM
     35 - Not used
     34 - Not used
     33 - Not used
     
     
     
  Some Notes:
    - A5 drives up when reflashed (???)
    - Interupts on teensy work slightly differently to on Arduino
    - DEFG E C D
*/


/// Libraries
#include <PID_v1.h>
#include "structsSlave.h"

// Function forward declaration
void printCommand(ModeStatus* machineStatus);
void resetModeCommand(ModeStatus * machineStatus);

// Structures
ModeStatus machineStatus;      // What mode are we running?
//slaveStatusStruct slaveStatus; // What is the status of A2,3,4

/// Varibles
// For serial coms with master Teensy
const unsigned int MAX_MESSAGE_LENGTH = 12; // Max length of serial msg that can be sent
char newValueSerial[12];                    // What the latest serial value
char newModeSerial[3];                      // What the latest serial mode
bool newCommandSerialBool = false;          // Has there been a new command

// ActiveControl
bool active = false;
bool sendUpdate = false;

// Global Posns (mm)
float A2_Pos;                     // A2
float A5_Pos;                     // A5
float A8_Pos;                     // A5

// Encoder Posns (counts)
volatile long A2_encoderPos = 0;  // A2
volatile long A5_encoderPos = 0;  // A5
volatile long A8_encoderPos = 0;  // A8

// Homing status
bool A2_Homed = false;            // A2
bool A5_Homed = false;            // A5
bool A8_Homed = false;            // A8

// Homing offset - What is the pos of axis when homed
float A2_HomeOffset = 15567;      // A2 (counts)
float A5_HomeOffset = 110;        // A5 (mm)
float A8_HomeOffset = 110;        // A8 (mm)

// Lims - The axis limits
float A2_MaxEnc = 22600;          // A2 (counts)
float A2_MinEnc = 0;
float A5_Max = A5_HomeOffset + 1; // A5 (mm)
float A5_Min = 0;
float A8_Max = A8_HomeOffset + 1; // A8 (mm)
float A8_Min = 0;

// Encoder counts per mm of axis motion
double A2_cpmm = 153280 / 77;     // A2
double A5_cpmm = 80;              // A5
double A8_cpmm = 80;              // A8

// Positioning tolerence - accuracy of PID positioning (counts)
int A2_tol = 50;                  // A2 
int A5_tol = 5;                   // A5
int A8_tol = 5;                   // A8

// Active control
int A5SensorValRng = 200;         // Range of sensor
int A5SensorValDeadband = 50;     // Deadband of sensor
int A5staticVal;                  // This is the static val of sensor (for active control)
bool A5Cal;                       // Has the sensor been calibrated
int A8SensorValRng = 200;         // Range of sensor
int A8SensorValDeadband = 50;     // Deadband of sensor
int A8staticVal;                  // This is the static val of sensor (for active control)
bool A8Cal;                       // Has the sensor been calibrated

// Shunt during shift
float m_down = 50;                // How far is the shunt down during pivot 
int shiftSpeed = 50;              // How fast is the shift during pivot

// PID Vals
double A2_P = 0.5;                // A2
double A2_I = 0;      
double A2_D = 0.08; 
double A5_P = 4;                  // A5
double A5_I = 0;      
double A5_D = 0; 
double A8_P = 4;                  // A8
double A8_I = 0;      
double A8_D = 0; 
double A2_Setpoint, A2_Input, A2_Output, A2_OutputA; 
PID A2PID(&A2_Input, &A2_Output, &A2_Setpoint, A2_P, A2_I, A2_D, DIRECT);
double A5_Setpoint, A5_Input, A5_Output, A5_OutputA; // Varibles wheel
PID A5PID(&A5_Input, &A5_Output, &A5_Setpoint, A5_P, A5_I, A5_D, DIRECT);
double A8_Setpoint, A8_Input, A8_Output, A8_OutputA; // Varibles wheel
PID A8PID(&A8_Input, &A8_Output, &A8_Setpoint, A8_P, A8_I, A8_D, DIRECT);

/// Pins
// Home
int A2_Home = 19;                 // A2 Home Pin

// PWM Drivers
int A2_RPWM = 23;                 // A2
int A2_LPWM = 22;                 
int A5_RPWM = 24;                 // A5
int A5_LPWM = 25;                 
int A8_RPWM = 37;                 // A8
int A8_LPWM = 36;                 

// Encoders
int A2_encoderPinA = 20;          // A2
int A2_encoderPinB = 21;        
int A5_encoderPinA = 27;          // A5
int A5_encoderPinB = 28;          
int A8_encoderPinA = 40;          // A8
int A8_encoderPinB = 39;          

// Sensors
int A5_SensorPin = 26;            // A5
int A8_SensorPin = 38;            // A8



void setup() {
  /// Start serials
  Serial.begin(9600);            // To PC (for debug)
  Serial1.begin(9600);            // To master controller

  // Initialise structures
  active = true;                 // Hack to not clog serial
  resetModeCommand(&machineStatus);
  active = false;
  
  /// Assign Pins
  // Drivers
  pinMode(A2_RPWM, OUTPUT);       // A2
  pinMode(A2_LPWM, OUTPUT);
  pinMode(A5_RPWM, OUTPUT);       // A5
  pinMode(A5_LPWM, OUTPUT);
  pinMode(A8_RPWM, OUTPUT);       // A8
  pinMode(A8_LPWM, OUTPUT);

  // Encoders
  // A2
  pinMode(A2_encoderPinA, INPUT_PULLUP); 
  pinMode(A2_encoderPinB, INPUT_PULLUP);
  attachInterrupt(A2_encoderPinA, A2_doEncoderA, CHANGE);
  attachInterrupt(A2_encoderPinB, A2_doEncoderB, CHANGE);
  // A5
  pinMode(A5_encoderPinA, INPUT_PULLUP);
  pinMode(A5_encoderPinB, INPUT_PULLUP);
  attachInterrupt(A5_encoderPinA, A5_doEncoderA, CHANGE);
  attachInterrupt(A5_encoderPinB, A5_doEncoderB, CHANGE);
  // A8
  pinMode(A8_encoderPinA, INPUT_PULLUP);
  pinMode(A8_encoderPinB, INPUT_PULLUP);
  attachInterrupt(A8_encoderPinA, A8_doEncoderA, CHANGE);
  attachInterrupt(A8_encoderPinB, A8_doEncoderB, CHANGE);

  // Home sensor
  pinMode(A2_Home, INPUT);        // A2

  // Active control sensors
  // Read by calling - analogRead(A5_SensorPin)
  //                 - analogRead(A8_SensorPin)
  
  /// PID Setup
  // A2
  A2PID.SetMode(AUTOMATIC);      
  A2PID.SetOutputLimits(-250, 250);
  A2PID.SetSampleTime(20);
  // A5
  A5PID.SetMode(AUTOMATIC);      
  A5PID.SetOutputLimits(-250, 250);
  A5PID.SetSampleTime(20);
  // A8
  A8PID.SetMode(AUTOMATIC);      
  A8PID.SetOutputLimits(-250, 250);
  A8PID.SetSampleTime(20);

}

// loop
void loop() {
  readSerial();
  processSerial();
  enactCommand();
}
