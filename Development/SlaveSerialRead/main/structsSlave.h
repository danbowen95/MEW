/*
 * To be able to parse structures into functions, they need to be defined in a seperate file
 * see: https://forum.arduino.cc/t/passing-typedef-structs-fails/79393/2
 * 
 * The import: #include "struct.cpp"
 * 
 */

// This is a structure that takes the serial command and....
typedef struct {
  char  mode;    // What mode are we in
  float value;   // what value are we working to
  bool  active;  // 0 - completed, 1 - currently in process
} ModeStatus;