

void Reset() {

  // Reset cal of Active sensor
  A5Cal = false;
  //A8Cal = false;
}

// DriveToSpeed
void DriveToSpeed(int Val , int AxisTemp) {
  /* Setting 'AxisTemp' to speed 'Val'.
      - 'AxisTemp' must be 2,5 or 8
      - 'Val' must be between -255 and 255
      - ^^ Has no catch
      - The code can (and probably should) be written MUCH more efficiently
  */

  switch (AxisTemp) {
    case 2:
      {
        if (Val <= 0)
        {
          // reverse rotation
          analogWrite(A2_LPWM, 0);
          analogWrite(A2_RPWM, abs(Val));
        }
        else
        {
          // forward rotation
          analogWrite(A2_RPWM, 0);
          analogWrite(A2_LPWM, Val);
        }
      }
      break;
    case 5:
      {
        if (Val <= 0)
        {
          // reverse rotation
          analogWrite(A5_LPWM, 0);
          analogWrite(A5_RPWM, abs(Val));
        }
        else
        {
          // forward rotation
          analogWrite(A5_RPWM, 0);
          analogWrite(A5_LPWM, Val);
        }
      }
      break;
      //    case 8:
      //      {
      //        if (Val <= 0)
      //        {
      //          // reverse rotation
      //          analogWrite(A8_RPWM, 0);
      //          analogWrite(A8_LPWM, Val);
      //        }
      //        else
      //        {
      //          // forward rotation
      //          analogWrite(A8_LPWM, 0);
      //          analogWrite(A8_RPWM, Val);
      //        }
      //      }
      //      break;
  }
}

void moveup()
{
  DriveToSpeed( -200 , 5 );
  delay(500);
  DriveToSpeed( 0 , 5 );
  // Serial.print("Now at: ");
  // Serial.println(A5_encoderPos);
}

void movedown()
{
  DriveToSpeed( 200 , 5 );
  delay(500);
  DriveToSpeed( 0 , 5 );
  // Serial.print("Now at: ");
  // Serial.println(A5_encoderPos);
}



void blinker( float val ) {
  int n;
  int led = 13;

  // round float to int
  n = round(val);


  for (int i = 0; i <= n; i++) {
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);               // wait for a second
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(500);               // wait for a second
  }
}
