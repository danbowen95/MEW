#define joyX A0
#define joyY A1
int xValue,yValue;
int RPWM_Output = 0; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
int LPWM_Output = 1; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)
int tol = 50;
int sensorValueMid = 785;

void setup()
{
  //Serial.begin(115200);
  pinMode(RPWM_Output, OUTPUT);
  pinMode(LPWM_Output, OUTPUT);
}

void loop()
{
  int sensorValue = analogRead(joyX);
  Serial.println(sensorValue);
 
  // sensor value is in the range 0 to 1023
  // the lower half of it we use for reverse rotation; the upper half for forward rotation
  if (sensorValue < (sensorValueMid - tol))
  {
    // reverse rotation
    int reversePWM = -(sensorValue - 511) / 2;
    analogWrite(LPWM_Output, 0);
    analogWrite(RPWM_Output, reversePWM);
    Serial.println("Go that way");
  }
  else if (sensorValue > (sensorValueMid + tol))
  {
    // forward rotation
    int forwardPWM = (sensorValue - 512) / 2;
    analogWrite(RPWM_Output, 0);
    analogWrite(LPWM_Output, forwardPWM);
    
    Serial.println("Go this way");
  }
  else
  {
    // Do nothing
    Serial.println("Do Nothing");
    analogWrite(RPWM_Output, 0);
    analogWrite(LPWM_Output, 0);
  }
}
