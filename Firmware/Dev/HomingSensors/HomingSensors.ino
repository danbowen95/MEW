/*   
 *   Test to see if homing switches work

   Pin Assignments:
    GND - GND --> 5v/12v rail
      0 - RX1 --> TX2 of master controller
      1 - TX1 --> RX2 of master controller
      2 - Not used
      3 - Not used
      4 - Not used
      5 - Not used 
      6 - Not used
      7 - RX2 --> TX Odrive 1 (via iso chip)
      8 - TX2 --> RX Odrive 1 (via iso chip)
      9 - Not used
     10 - Not used
     11 - Not used
     12 - Not used 
   - - - - - - - - - - - - - -
    Vin - Vin --> +5v
    GND - Not Used
   3.3v - 3.3v --> All iso chips +V
     23 - Not used
     22 - I/O --> A3 Home
     21 - I/O --> A4 Home
     20 - I/O --> A6 Home
     19 - I/O --> A7 Home
     18 - Not Used
     17 - TX4 --> RX Odrive 3 (via iso chip)
     16 - RX4 --> TX Odrive 3 (via iso chip)
     15 - RX3 --> TX Odrive 2 (via iso chip)
     14 - TX3 --> RX Odrive 2 (via iso chip)


     
*/

/// Pins
// Homeing Pins
//int A3HomePin = 22;
//int A4HomePin = 21;
//int A6HomePin = 20;
//int A7HomePin = 19;


int A7HomePin = 9;

void setup() {
  /// Start serials
  Serial.begin(9600);             // To PC (for debug)
  
  /// Assign Pins
  // Home sensors
//  pinMode(A3HomePin, INPUT);
//  pinMode(A4HomePin, INPUT);
//  pinMode(A6HomePin, INPUT);
  pinMode(A7HomePin, INPUT);

}

void loop() {

  // A3
//  if (digitalRead(A3HomePin)) {
//    Serial.print("A3: ON  ");
//  }
//  else {
//    Serial.print("A3: OFF ");
//  }

  // A4
//  if (digitalRead(A4HomePin)) {
//    Serial.print("A4: ON  ");
//  }
//  else {
//    Serial.print("A4: OFF ");
//  }
//
//  // A7
//  if (digitalRead(A6HomePin)) {
//    Serial.print("A6: ON  ");
//  }
//  else {
//    Serial.print("A6: OFF ");
//  }
//
  // A7
  if (digitalRead(A7HomePin)) {
    Serial.println("A7: ON  ");
  }
  else {
    Serial.println("A7: OFF ");
  }

  delay(500);
}
