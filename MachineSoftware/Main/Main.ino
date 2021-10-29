/*   = = = = = = = = = = = = =  
    | FIRMWARE FOR MAIN TEENSY |
     = = = = = = = = = = = = = 
     This is trouble
 *  
 *  This sketch is to be uploaded to the Slave Teensy - Teensy 4.1.
    See full documentation in 'Chapter 6 - Mechanising the English Wheel'
    for more info on opperating the machine.

*  ** WARNING **
    ! ! When uploading code to this controller, ensure no ground loops are formed ! !

*   Documentation Contents:
     - Intro
     - Warnings!
     - Documentation Contents
     - Code Structure
     - Pin Assignments
     - Recognised Commands 

*   Code Structure:
     - Preamble
     - setup()
     - loop()

*  Pin Assignments:
    GND - GND of 5v rail
     0  - RX1 to TX1 of slave Teensy
     1  - Tx1 to RX1 of slave Teensy
     2  - Not used
     3  - LED (Grn) for serial connection
     4  - Not used
     5  - Not Used
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
     19 - Not Used
     18 - Not Used
     17 - RX3 to TX of odrive 2
     16 - TX3 to RX of odrive 2
     15 - Not Used
     14 - Not Used
     13 - Not Used


*   Recognised Commands:
     - M00 - Display machine status
     - M01 - Not Used


     
     - setup()
     - loop()
*/

// Libraries
#include <HardwareSerial.h>
#include <ODriveArduino.h>
#include "structsMain.h"

// Function forward declaration
void printCommand(CommandStruct* NewCommand);

// Structures
CommandStruct NewCommand;      // What mode are we running?
masterStatusStruct masterStatus; // What is the status of A1,3,4,6,7

// Odrive helper
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

// ODrive objects
ODriveArduino odrive1(Serial2);
ODriveArduino odrive2(Serial3);
ODriveArduino odrive3(Serial4);



/// Define Varibles:

// For serial coms with PC
const unsigned int MAX_MESSAGE_LENGTH = 64; // Max length of serial
char newCommandSerial[64];                        // What the latest cmd is
bool newCommandSerialBool = false;


int      s = 10;         // Line speed of sheet (mm/s)
float a1_r = 9.73551501; // Axis 1 movement per rev (mm/rev) = (12/72)*(16/72)*(16/34)*(2*PI())*88.9
float a3_r = 3;          // Axis 3 movement per rev (mm/rev)
float a4_r = 2;          // Axis 4 movement per rev (mm/rev)
float a6_r = 3;          // Axis 6 movement per rev (mm/rev) 
float a7_r = 2;          // Axis 7 movement per rev (mm/rev) 


void setup() {
  // Set up serial to PC:
  Serial.begin(115200); 
  
  // Set up ODrive serial lines using 115200 baud:
  Serial2.begin(115200);
  Serial3.begin(115200);
  Serial4.begin(115200);

  // Initalise New command
  resetCommand(&NewCommand);

  // Print to checkr
  printCommand(&NewCommand);

}

// This is the main loop
void loop()
{
  readSerial();

  processSerial();

  enactCommand();
}
