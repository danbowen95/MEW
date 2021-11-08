/* Sketch to turn on/off relay */




void setup() {

  Serial.begin(9600);

  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println("HIGH");
  digitalWrite(13,HIGH);
  digitalWrite(12,HIGH);

  delay(1500);
  
  Serial.println("LOW");
  digitalWrite(13,LOW);
  digitalWrite(12,LOW);

  delay(1500);

}
