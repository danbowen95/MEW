/*
 * To be able to parse structures into functions, they need to be defined in a seperate file
 * see: https://forum.arduino.cc/t/passing-typedef-structs-fails/79393/2
 * 
 * The import: #include "struct.cpp"
 * 
 */

// This is a structure that takes the serial command and....
typedef struct {
  String CommandID; // What command
  float X;          // X Varible
  float Y;          // Y Varible
  float Z;          // Z Varible
  float A;          // U Varible
  float B;          // V Varible
  bool active;      // 0 - completed, 1 - currently in process
} CommandStruct;


// This is a structure that defines the machine status at that given time
typedef struct {
  bool Armed[8]; // Is axis armed (calibrated and ready to go)
  bool Home[8];  // Is the axis Homed 
  float Pos[8];  // The position of the axis
  bool Vac[2];   // Is the vacuum on ([1] - front, [2] - back)
} MachineStatusStruct;
