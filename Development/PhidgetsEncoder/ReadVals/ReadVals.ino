int in1;
int in2;

void setup() {
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);

  Serial.begin(115200);
}

void loop() {
  in1 = digitalRead(2);
  in2 = digitalRead(3);

  Serial.print(in1);
  Serial.print(" , ");
  Serial.println(in2);
}
