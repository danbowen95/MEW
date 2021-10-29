/* This is to */

int potPin = 26; // input pin for the potentiometer


void setup() {
  Serial.begin(9600); // opens serial port
}

void loop() {
  int v;
  // put your main code here, to run repeatedly:
  Serial.println("Now going to average the pot value");
  Serial.println("Starting in: ");
  for (int i = 0; i <= 3; i++) {
    v = 3 - i;
    Serial.print(v);
    delay(1000);
  }
  Serial.println("Reading...");
  Serial.println(20);
  A5AvgSensorVal( 25 );
  Serial.println("Done!");
  delay(10000);

}


int A5AvgSensorVal( int nVals ) {
  // nVals           // The number of values we are looking at
  int   valRead;     // This is the final value
  float Value = 0;   // This is the temporary value


  // Loop thru for average
  for (int i = 0; i < nVals; i++) {
    valRead = analogRead(potPin);
    Value   = Value + valRead;
    Serial.print("Actual Value: ");
    Serial.println(valRead);
  }
  Value = round(Value/nVals);
  Serial.print("Averaged Value: ");
  Serial.println(Value);
  return Value;
}
