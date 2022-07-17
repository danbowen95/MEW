/*   = = = = = = = = = = = = = = =
    | FIRMWARE FOR MAIN CONTROLLER |
     = = = = = = = = = = = = = = =


    This sketch is to be uploaded to the Master Arduino - Mega 2560.
    See full documentation in 'Mechanising the English Wheel'
    for more info on opperating the machine.

    ** WARNING **
    ! ! When uploading code to this controller, ensure no ground loops are formed ! !

   Pin Assignments:
    GND - GND of 5v/12v rail
    Vin - +12v
     .
     .
     .
     14 - TX3 to RX1 of slave 2
     15 - RX3 to TX1 of slave 2
     16 - TX2 to RX1 of slave 1
     17 - RX2 to TX1 of slave 1
     .
     .
     52 - Dig A5 vac
     53 - Dig A8 Vac
*/

/// Libraries
#include "structsMain.h"

/// Function forward declaration
void printCommand(CommandStruct* NewCommand);

/// Structures
CommandStruct NewCommand;        // What mode are we running?

/// Define Varibles:
// For serial coms with PC
const unsigned int MAX_MESSAGE_LENGTH = 64; // Max length of serial
char newCommandSerial[64];                  // What the latest cmd is
bool newCommandSerialBool = false;

/// Coms with PC
// If true,  there will be lots of feedback printed down serial 1.
// If false, prints "1" to serial when done
bool debug = false;

// Calibration - Has the odrive been calibrated?
bool A1Cal = false;
bool A3Cal = false;
bool A4Cal = false;
bool A6Cal = false;
bool A7Cal = false;

// Homing - Has the axis been homed?
bool A2Home = false;
bool A3Home = false;
bool A4Home = false;
bool A5Home = false;
bool A6Home = false;
bool A7Home = false;
bool A8Home = false;

// Position - What is the position (mm) of the axis
float A2Pos;
float A3Pos;
float A4Pos;
float A5Pos;
float A6Pos;
float A7Pos;
float A8Pos;

// Axis lims (mm)
// ! ! These should align with Slave Teensy ! !
float A2MinPos = 0;
float A2MaxPos = 11.35;
float A3MinPos = 110;
float A3MaxPos = 510;
float A4MinPos = -68;
float A4MaxPos = 68;
float A5MinPos = 0;
float A5MaxPos = 111;
float A6MinPos = 110;
float A6MaxPos = 510;
float A7MinPos = -68;
float A7MaxPos = 68;
float A8MinPos = 0;
float A8MaxPos = 111;

// Homing Pos - When homed, what position is the axis at?
// ! ! These should align with Slave Teensy ! !
float A2HomePos = 7.82;   // Checked
float A3HomePos = 110 + 16.5;
float A4HomePos = 59;
float A5HomePos = 50;
float A6HomePos = 110 + 15.5;
float A7HomePos = 59;
float A8HomePos = 50;

// Vacuum status (true = vacuum on, false = vacuum off)
bool A5Vac;
bool A8Vac;

// Active control status
bool A5Active = false;   // Is AC currently on?
bool A8Active = false;
bool A5Known  = false;   // Does master know pos of A5/A8
bool A8Known  = false;

/// Define some pins
// Vacuum Pins
int A5VacPin = 8;
int A8VacPin = 9;

void setup() {
  // Set up serial to PC:
  Serial.begin(9600);

  // Set up serial to Slaves:
  Serial1.begin(9600);
  Serial2.begin(9600);

  /// Set up pins
  // Vacuum solenoids
  pinMode(A5VacPin, OUTPUT);
  pinMode(A8VacPin, OUTPUT);

  // Initalise New command
  resetCommand(&NewCommand);

  // Switch off Vac
  VacControl( 5 , false );
  VacControl( 8 , false );
}

// This is the main loop
void loop()
{
  readSerial();
  processSerial();
  enactCommand();
}
