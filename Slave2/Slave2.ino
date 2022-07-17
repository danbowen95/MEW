/*   = = = = = = = = = = = = = = = = =
    | FIRMWARE FOR SLAVE 2 CONTROLLER |
     = = = = = = = = = = = = = = = = =


    This sketch is to be uploaded to the Teensy 4.0.
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
      7 - RX2 --> TX Odrive 1 (via iso chip)
      8 - TX2 --> RX Odrive 1 (via iso chip)
      9 - Not used
     10 - Not used
     11 - Not used
     12 - Not used 
   - - - - - - - - - - - - - -
    Vin - Vin --> +5v
    GND - Not Used
   3.3v - 3.3v --> All iso chips +V
     23 - Not used
     22 - I/O --> A3 Home
     21 - I/O --> A4 Home
     20 - I/O --> A6 Home
     19 - I/O --> A7 Home
     18 - Not Used
     17 - TX4 --> RX Odrive 3 (via iso chip)
     16 - RX4 --> TX Odrive 3 (via iso chip)
     15 - RX3 --> TX Odrive 2 (via iso chip)
     14 - TX3 --> RX Odrive 2 (via iso chip)



  Some Notes:
    - Pick odrives from daisy chain when flashing the boards
    - odrive1 - motor 0 = NOT USED
              - motor 1 = Axis 1
    - odrive2 - motor 0 = Axis 3
              - motor 1 = Axis 4
    - odrive3 - motor 0 = Axis 6
              - motor 1 = Axis 7
              
- DEFG E C D
    
     
*/

/// Libraries
#include <HardwareSerial.h>
#include <ODriveArduino.h>
#include <Ramp.h>
#include "structsSlave.h"


// Function forward declaration
//void printCommand(ModeStatus* machineStatus);
void resetModeCommand(ModeStatus * machineStatus);

// Structures
ModeStatus machineStatus;      // What mode are we running?

/// Odrive helper
template<class T> inline Print& operator <<(Print &obj,     T arg) {
  obj.print(arg);
  return obj;
}
template<>        inline Print& operator <<(Print &obj, float arg) {
  obj.print(arg, 4);
  return obj;
}

// ODrive objects
ODriveArduino odrive1(Serial2);
ODriveArduino odrive2(Serial3);
ODriveArduino odrive3(Serial4);

/// Varibles
// For serial coms with master Teensy
const unsigned int MAX_MESSAGE_LENGTH = 12; // Max length of serial msg that can be sent
char newValueSerial[12];                    // What the latest serial value
char newModeSerial[3];                      // What the latest serial mode
bool newCommandSerialBool = false;          // Has there been a new command

// General Machine settings
float linespeed = 10;  // mm/s of linespeed
float arcspeed  = 1;   // mm/s of arc
float homespeed = 5;   // mm/s of homing speed
float dt = 100;         // how often do we send motor positions (millisecs)

// Calibration - Has the odrive been calibrated?
bool A1Cal = false;
bool A3Cal = false;
bool A4Cal = false;
bool A6Cal = false;
bool A7Cal = false;

// Homing - Has the axis been homed?
bool A3Home = false;
bool A4Home = false;
bool A6Home = false;
bool A7Home = false;

// Vacuum status
bool A5Vac;
bool A8Vac;

// Position - What is the position (mm) of the axis
float A3Pos;
float A4Pos;
float A6Pos;
float A7Pos;

// Motor Position - What is the motor position (for odrive)
float A1MotorPos;
float A3MotorPos;
float A4MotorPos;
float A6MotorPos;
float A7MotorPos;

// Axis Revs per mm
float A1Rpmm = 1 / 9.73058;
float A3Rpmm = 0.3333333;
float A4Rpmm = 0.5;
float A6Rpmm = 0.3333333;
float A7Rpmm = 0.5;

// To change direction of motor rev to coordinate system
// (Must be +1 or -1)
float A1Dir = 1;
float A3Dir = -1;
float A4Dir = 1;
float A6Dir = -1;
float A7Dir = 1;

// Axis lims (mm)
float A3MinPos =  110;
float A3MaxPos =  510; 
float A4MinPos = -68;
float A4MaxPos =  68;   
float A6MinPos =  110;
float A6MaxPos =  510; 
float A7MinPos = -68;
float A7MaxPos =  68;

// Homing Pos - When homed, what position is the axis at?
// * Should align with Slave Teensy
float A3HomePos = 110 + 16.5; // = 110 + (measured - 11)
float A4HomePos = 59; 
float A6HomePos = 110 + 15.5; // = 110 + (measured - 11)
float A7HomePos = 59;      

/// Pins
// Homeing Pins
int A3HomePin = 22;
int A4HomePin = 21;
int A6HomePin = 20;
int A7HomePin = 19;

// LED Pin
int LEDPin = 13;



void setup() {
  /// Start serials
  //Serial.begin(9600);             // To PC (for debug)
  Serial1.begin(9600);            // To master controller

  Serial2.begin(115200);          // To odrive 1
  Serial3.begin(115200);          // To odrive 2
  Serial4.begin(115200);          // To odrive 3

  // Initialise structures
  resetModeCommand(&machineStatus);

  /// Assign Pins
  // Home sensors
  pinMode(A3HomePin, INPUT);
  pinMode(A4HomePin, INPUT);
  pinMode(A6HomePin, INPUT);
  pinMode(A7HomePin, INPUT);

  // LED pin
  pinMode(LEDPin, OUTPUT);
}

void loop() {
  readSerial();
  processSerial();
  enactCommand();
}
