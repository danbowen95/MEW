/*   = = = = = = = = = = = = =
    | FIRMWARE FOR MAIN TEENSY |
     = = = = = = = = = = = = =


    This sketch is to be uploaded to the Slave Teensy - Teensy 4.1.
    See full documentation in 'Chapter 6 - Mechanising the English Wheel'
    for more info on opperating the machine.

*  ** WARNING **
    ! ! When uploading code to this controller, ensure no ground loops are formed ! !

    Documentation Contents:
     - Intro
     - Warnings!
     - Documentation Contents
     - Code Structure
     - Pin Assignments
     - Recognised Commands

    Code Structure:
     - Preamble
     - setup()
     - loop()

   Pin Assignments:
    GND - GND of 5v rail
     0  - RX1 to TX1 of slave Teensy
     1  - Tx1 to RX1 of slave Teensy
     2  - Not used
     3  - LED (Grn) for serial connection
     4  - Dig A5 Vac
     5  - Dig A8 Vac
     6  - Not Used
     7  - RX2 to TX of odrive 1
     8  - TX2 to RX of odrive 1
     9  - Not Used
     10 - Not Used
     11 - Dig reset PTM (220ohm res required)
     12 - Not Used
    - - - - - - - - - - - -
    Vin - 5v rail
    GND - Not used
     3v - To Vcc on ISO chips (3 off)
     23 - Not Used
     22 - Not Used
     21 - RX5 to TX of odrive 3
     20 - RX5 to TX of odrive 3
     19 - Home A6
     18 - Home A7
     17 - RX3 to TX of odrive 2
     16 - TX3 to RX of odrive 2
     15 - Home A3
     14 - Home A4
     13 - Not Used

*/

// Libraries
#include <HardwareSerial.h>
#include <ODriveArduino.h>
#include "structsMain.h"

// Function forward declaration
void printCommand(CommandStruct* NewCommand);

// Structures
CommandStruct NewCommand;        // What mode are we running?
masterStatusStruct masterStatus; // What is the status of A1,3,4,6,7

// Odrive helper
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

// Non-Axis Pins
int A5_VacPin = 15;      // Axis 5 Vacuum pin
int A8_VacPin = 14;      // Axis 8 Vacuum pin

/// Define Varibles:

// General Machine settings
float linespeed = 5;   // mm/s of linespeed
float arcspeed  = 1;   // mm/s of arc
float homespeed = 1;   // mm/s of homing speed
float dt = 5;          // how often do we send motor positions (millisecs)

// For serial coms with PC
const unsigned int MAX_MESSAGE_LENGTH = 64; // Max length of serial
char newCommandSerial[64];                  // What the latest cmd is
bool newCommandSerialBool = false;

// For Axis 1
// bool A1Home = false;     // Is Axis homed?
bool A1Cal = false;         // Is Axis calibrated?
float A1Rpmm = 1 / 9.73058; // Axis movement (mm/rev)
float A1mmpR = 1107449 / 273100.8; // Axis movement (rev/mm)
float A1MotorPos = 0;       // Axis motor position (rev)
float A1Pos;                // Axis current position (mm)
// float A1maxPos;          // Max position of Axis (mm)

// For Axis 2
bool A2Home = false;        // Is Axis homed?
float A2Pos;                // Axis current position (mm)
float A2maxPos;             // Max position of Axis (mm)

// For Axis 3
bool A3Home = false;        // Is Axis homed?
bool A3Cal = false;         // Is Axis calibrated?
float A3Rpmm = 0.3333333;   // Axis movement (mm/rev)
float A3mmpR = 3;           // Axis movement (rev/mm)
float A3MotorPos = 0;       // Axis motor position (rev)
float A3Pos;                // Axis current position (mm)
float A3HomingPos = 10;     // Homing switch pos (abs) (mm)
float A3minPod = 0;         // Min position of A3
float A3maxPos = 200;       // Max position of Axis (mm)
int A3HomePin = 15;         // Pin to A3 home

// For Axis 4
bool A4Home = false;        // Is Axis homed?
bool A4Cal = false;         // Is Axis calibrated?
float A4Rpmm = 0.5;         // Axis movement (mm/rev)
float A4mmpR = 2;           // Axis movement (rev/mm)
float A4MotorPos = 0;       // Axis motor position (rev)
float A4Pos;                // Axis current position (mm)
float A4HomingPos = 10;     // Homing switch pos (abs) (mm)
float A4maxPos;             // Max position of Axis (mm)
int A4HomePin = 14;         // Pin to A4 home

// For Axis 5
bool A5Home = false;        // Is Axis homed?
float A5Pos;                // Axis current position (mm)
float A5maxPos;             // Max position of Axis (mm)
bool A5Active = false;      // Is Axis active?



void setup() {
  // Set up serial to PC:
  Serial.begin(115200);

  // Set up serial to Slave:
  Serial1.begin(9600);

  // Set up serial to ODrive:
  Serial2.begin(115200);
  Serial3.begin(115200);
  Serial4.begin(115200);

  // Set up pins
  pinMode(A3HomePin, INPUT);  // Home sensors
  pinMode(A4HomePin, INPUT);
//  pinMode(A6HomePin, INPUT);
//  pinMode(A7HomePin, INPUT);

  // Initalise New command
  resetCommand(&NewCommand);

  // Print to check
  printCommand(&NewCommand);

}

// This is the main loop
void loop()
{
  readSerial();

  processSerial();

  enactCommand();
}
