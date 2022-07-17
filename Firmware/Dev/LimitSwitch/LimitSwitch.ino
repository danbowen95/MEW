// Check ptm circuit




#define LIMIT_SWITCH_PIN 33
 
void setup() {
  Serial.begin(9600);
  pinMode(LIMIT_SWITCH_PIN, INPUT);
}
 
void loop() {
 
  if (digitalRead(LIMIT_SWITCH_PIN) == HIGH)
  {
    Serial.println("Activated!");
  }
 
  else
  {
    Serial.println("Not activated.");
  }
   
  delay(100);
}
