int read1;
char ident;


void setup() {
  Serial.begin(115200); // opens serial port, sets data rate to 9600 bps
}

void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    read1 = Serial.parseInt();
    ident = Serial.read();
  }

    // say what you got:
  Serial.print("I received: ");
  Serial.println(read1);

}
