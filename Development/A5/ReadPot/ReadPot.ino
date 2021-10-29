/* Analog Read to LED
   ------------------

   turns on and off a light emitting diode(LED) connected to digital
   pin 13. The amount of time the LED will be on and off depends on
   the value obtained by analogRead(). In the easiest case we connect
   a potentiometer to analog pin 2.

   Created 1 December 2005
   copyleft 2005 DojoDave <http://www.0j0.org>
   http://arduino.berlios.de

*/


const int numReadings = 10;     // How many values are we going to average over

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int potPin = 26;                // input pin for the potentiometer
int val = 0;                    // variable to store the value coming from the sensor

void setup() {
  Serial.begin(9600); // opens serial port

  // initialize all the analogue readings to 0:
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
}

void loop() {
  val = analogRead(potPin);    // read the value from the sensor
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = val;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  delay(10);        // delay in between reads for stability
  
  //Serial.print("Raw Value: ");
  Serial.print(val);
  Serial.print(" , ");
  //Serial.print("Smoothed value: ");
  Serial.println(average);
}
