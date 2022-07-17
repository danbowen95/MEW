// Read analogue value

// Sensor pins
// Sensors
int A5_SensorPin = 26;            // A5
int A8_SensorPin = 38;            // A8

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pins
  int A5sensorValue = analogRead(A5_SensorPin);
  int A8sensorValue = analogRead(A8_SensorPin);
  
  // print out the value you read:
  Serial.print("A5: ");
  Serial.print(A5sensorValue);
  Serial.print("A8: ");
  Serial.println(A8sensorValue);
  
  delay(1);        // delay in between reads for stability
}
