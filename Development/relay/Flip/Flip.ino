/* Sketch to turn on/off relay */




void setup() {

  Serial.begin(9600);

  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println("HIGH");
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);

  delay(1500);
  
  Serial.println("LOW");
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);

  delay(1500);

}
