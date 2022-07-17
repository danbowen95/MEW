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
