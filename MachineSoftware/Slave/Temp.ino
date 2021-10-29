void moveup()
{
  DriveToSpeed( -200 , 5 );
  delay(500);
  DriveToSpeed( 0 , 5 );
  Serial.print("Now at: ");
  Serial.println(A5_encoderPos);
}

void movedown()
{
  DriveToSpeed( 200 , 5 );
  delay(500);
  DriveToSpeed( 0 , 5 );
  Serial.print("Now at: ");
  Serial.println(A5_encoderPos);
}
