/*
 * To be able to parse structures into functions, they need to be defined in a seperate file
 * see: https://forum.arduino.cc/t/passing-typedef-structs-fails/79393/2
 * 
 * The import: #include "struct.cpp"
 * 
 */

// This is a structure that takes the serial command and changes 
// the mode of the slave controller
typedef struct {
  char  mode;    // What mode are we in
  float value;   // what value are we working to
  bool  active;  // 0 - completed, 1 - currently in process
} ModeStatus;

// This structure keeps track of the state of each axis
typedef struct {
  bool A2Home;   // Has A2 been homed
  bool A3Home;   // Has A2 been homed
  bool A5Home;   // Has A2 been homed
  float A2Pos;   // What is the pos of A2
  float A5Pos;   // What is the pos of A5
  float A8Pos;   // What is the pos of A8
} slaveStatusStruct;
