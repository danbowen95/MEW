// This simply demonstrates how to write to the serial monitor


unsigned long myTime;
int loopNo = 0;

void setup() {
  Serial.begin(9600);
}
void loop() {
  Serial.print("Time: ");
  myTime = millis();

  Serial.print(myTime); // prints time since program started
  Serial.print(" - Loop No:");
  Serial.println(loopNo);

  loopNo++; // Increment LoopNo
  
  delay(1000);          // wait a second so as not to send massive amounts of data
}
