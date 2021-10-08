// includes
#include <HardwareSerial.h>
#include <ODriveArduino.h>

// Printing with stream operator helper functions
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

// ODrive objects
//ODriveArduino odrive1(Serial1);
ODriveArduino odrive2(Serial2);
//ODriveArduino odrive3(Serial3);

// delay value
int d = 500;

void setup() {
  
  // ODrive uses 115200 baud
//  Serial1.begin(115200);
  Serial2.begin(115200);
//  Serial3.begin(115200);
  
  // Serial to PC
  Serial.begin(115200);
  while (!Serial) ; // wait for Arduino Serial Monitor to open

}

void loop() {
  // put your main code here, to run repeatedly:
//  
//  Serial1 << "r vbus_voltage\n";
//  Serial << "Odrive on serial 1 - Vbus voltage: " << odrive1.readFloat() << '\n';
//  delay(d);
//  
  Serial2 << "r vbus_voltage\n";
  Serial << "Odrive on serial 2 - Vbus voltage: " << odrive2.readFloat() << '\n';
  delay(d);
////
//  Serial3 << "r vbus_voltage\n";
//  Serial << "Odrive on serial 3 - Vbus voltage: " << odrive3.readFloat() << '\n';
//  delay(d);

}
