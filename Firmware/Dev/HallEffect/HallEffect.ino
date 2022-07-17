
int digitalPin = 8; // Hall magnetic sensor input 1 (high) or 0 (low)
int digitalInputValue ; // digital readings
int analogInputValue; // analog readings

void setup ()
{
  pinMode (digitalPin, INPUT); 
  Serial.begin(9600);
}
void loop ()
{
  Serial.print("Sensor: ");
  
  digitalInputValue = digitalRead(digitalPin) ; 
  if (digitalInputValue == HIGH) // When magnet is present, digitalInputValue gets 1 (HIGH) and turns LED on
  {
    Serial.println("ON");    
  }
  else
  {
    Serial.println("OFF");
  }
  delay(100);
}
