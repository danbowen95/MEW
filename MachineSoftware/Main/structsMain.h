/*
   To be able to parse structures into functions, they need to be defined in a seperate file
   see: https://forum.arduino.cc/t/passing-typedef-structs-fails/79393/2

   The import: #include "struct.cpp"

*/

// This is a structure that takes the serial command and changes
// the mode of the slave controller
typedef struct {
  String CommandID; // What command
  float X;          // X Varible
  float Y;          // Y Varible
  float Z;          // Z Varible
  float A;          // A Varible
  float B;          // B Varible
  bool active;      // 0 - completed, 1 - currently in process
} CommandStruct;

// This structure keeps track of the state of each axis
// This is in encoder counts, run 'printPos' to see real displacements
typedef struct {
  bool A3Home;   // Has A2 been homed
  bool A4Home;   // Has A2 been homed
  bool A6Home;   // Has A2 been homed
  bool A7Home;   // Has A2 been homed
  bool A1Cal;   // Has A2 been homed
  bool A3Cal;   // Has A2 been homed
  bool A4Cal;   // Has A2 been homed
  bool A6Cal;   // Has A2 been homed
  bool A7Cal;   // Has A2 been homed
  float A1Pos;   // What is the pos of A2 (in encoder counts)
  float A3Pos;   // What is the pos of A5 (in encoder counts)
  float A4Pos;   // What is the pos of A8 (in encoder counts)
  float A6Pos;   // What is the pos of A2 (in encoder counts)
  float A7Pos;   // What is the pos of A5 (in encoder counts)
} masterStatusStruct;
