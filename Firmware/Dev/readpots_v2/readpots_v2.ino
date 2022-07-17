// Read sensor vals

// Sensors
int A5_SensorPin = 26;            // A5
int A8_SensorPin = 38;            // A8


void setup() {
   /// Start serials
  Serial.begin(9600);            // To PC (for debug)

  
}

void loop() {
  // put your main code here, to run repeatedly:
  // read the input on analog pin 0:
  int A5sensorValue = analogRead(A5_SensorPin);
  int A8sensorValue = analogRead(A8_SensorPin);
  // print out the value you read:
  Serial.print("A5sensorValue: ");
  Serial.print(A5sensorValue);
  Serial.print(" , A8sensorValue: ");
  Serial.println(A8sensorValue);
  delay(1);        // delay in between reads for stability
}
