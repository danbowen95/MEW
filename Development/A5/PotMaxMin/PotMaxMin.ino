/* This is to measure the Max and min vals of the pot */

int potPin = 26; // input pin for the potentiometer
int valRead;
int maxVal = 0;
int minVal = 0;

void setup() {
  Serial.begin(9600); // opens serial port
}

void loop() {
  valRead = analogRead(potPin);

  if (valRead < minVal) {
    minVal = valRead;
  }
  if (valRead > maxVal) {
    maxVal = valRead;
  }
  Serial.print("Current val: ");
  Serial.print(valRead);
  Serial.print(" , minVal = ");
  Serial.print(minVal);
  Serial.print(" , maxVal = ");
  Serial.println(maxVal);

}
