/*
   This is the main program that runs the machine

   Some useful resources used when creating this code:
    - https://www.marginallyclever.com/2013/08/how-to-build-an-2-axis-arduino-cnc-gcode-interpreter/
    - Arduino Cookbook



*/

/// Some preamble
// Define structure called 'cmd' to carry input commands
typedef struct {
  bool valid;    // Is the command valid
  char MGXX[4];  // Geometry or Machine command raw
  int  MGidx;    // Geometry or Machine index
  float in[4];   // Array of inputs (max 4)
  char Raw[64];  // Raw serial command
} cmd;
// Initalize new command structure
cmd newCommand = {false, "ABC", 0, {0, 0, 0, 0}, "ABCDEF"};


// Define some varibles:
bool Active = false;         // Is the machine doing anything (True = yes, False = no)
boolean newData = false;     // For serial in



/// Run Setup
void setup() {
  Serial.begin(38400); // open coms
  clearscreen();
  startup();           // say hello
  help();              // list help
  ready();             // say that we're ready
}

/// Run Machine
void loop() {



  processCommand();

  enactCommand();

}
