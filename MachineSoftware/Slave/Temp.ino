void moveup()
{
  DriveToSpeed( -200 , 2 );
  delay(500);
  DriveToSpeed( 0 , 2 );  
}

void movedown()
{
  DriveToSpeed( 200 , 2 );
  delay(500);
  DriveToSpeed( 0 , 2 );  
}
