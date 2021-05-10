// This simply demonstrates how to write to the serial monitor


unsigned long myTime;
int loopNo = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Time, Milliseconds Since Start, Loop Number");
}
void loop() {
  Serial.print(",");
  myTime = millis();

  Serial.print(myTime); // prints time since program started
  Serial.print(",");
  Serial.println(loopNo);

  loopNo++; // Increment LoopNo
  
  delay(1000);          // wait a second so as not to send massive amounts of data
}
