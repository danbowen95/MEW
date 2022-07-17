/* G Code commands

  G00() - Print all machine codes
  G01() - Move A1 an incremental amount
  G02() - Move A2 an incremnetal amount
  G03() - Move A3 an incremnetal amount
  G04() - Move A3 an incremnetal amount
  G05() - Move A3 an incremnetal amount
  G06() - Move A3 an incremnetal amount
  G07() - Move A3 an incremnetal amount
  G08() - Move A3 an incremnetal amount
  .
  G12() - Move A2 to absolute position
  G13() - Move A3 to absolute position
  G14() - Move A4 to absolute position
  G15() - Move A5 to absolute position
  G16() - Move A6 to absolute position
  G17() - Move A7 to absolute position
  G18() - Move A8 to absolute position
  .
  G21() - Move A3 an incremental amount, moving A1 synchronously
  G22() - Move A3 to absolute position, moving A1 synchronously
  G23() - Move A6 an incremental amount, moving A1 synchronously
  G24() - Move A6 to absolute position, moving A1 synchronously
  G25() - Move A3 and A4 in arc, of radius A3 to abs theta position
  G26() - Move A6 and A7 in arc, of radius A6 to abs theta position
  .
  G28() - Move A3A1 in and out n times
  .
  G31() - Raise A5, then AC
  G32() - Raise A8, then AC
  G33() - Shift down A5 50, or to bottom
  G34() - Shift down A8 50, or to bottom
  .
  G42() - Home A2
  G43() - Home A3
  G44() - Home A4
  G45() - Home A5
  G46() - Home A6
  G47() - Home A7
  G48() - Home A8

  G50() - Establish connection (not debug)



*/

void G00() {
  /* Print all machine codes */

  // Print
  printMachineCodes();
}


void G01() {
  /* Move A1 an incremental amount */

  bool  Good2Move;
  float Pos = NewCommand.X;

  Good2Move = axisCal(1);

  if (Good2Move) {
    A1Move( Pos );
  }
}

void G02() {
  /* Move A2 an incremental amount */

  bool  Good2Move;
  float A2NewPos;
  float Pos = NewCommand.X;

  // New = Old + inc
  A2NewPos = A2Pos + Pos;

  // Check
  Good2Move = axisHome(2) * withinLims(2 , A2NewPos);

  if (Good2Move) {
    // Move
    A2Move( A2NewPos );
  }
}

void G03() {
  /* Move A3 an incremental amount */

  bool  Good2Move;
  float A3NewPos;
  float Pos = NewCommand.X;

  // New = Old + inc
  A3NewPos = A3Pos + Pos;

  // Check
  Good2Move = axisHome(3) * withinLims(3 , A3NewPos) * axisCal(3);

  if (Good2Move) {
    // Move
    A3Move( A3NewPos );
  }
}

void G04() {
  /* Move A4 an incremental amount */

  bool  Good2Move;
  float A4NewPos;
  float Pos = NewCommand.X;

  // New = Old + inc
  A4NewPos = A4Pos + Pos;

  // Check
  Good2Move = axisHome(4) * withinLims(4 , A4NewPos) * axisCal(4);

  if (Good2Move) {
    // Move
    A4Move( A4NewPos );
  }
}

void G05() {
  /* Move A5 an incremental amount */

  bool  Good2Move;
  float A5NewPos;
  float Pos = NewCommand.X;

  // New = Old + inc
  A5NewPos = A5Pos + Pos;

  // Check
  Good2Move = axisHome(5) * withinLims(5 , A5NewPos) * A5Known;

  if (Good2Move) {
    // Move
    A5Move( A5NewPos );
  }
}

void G06() {
  /* Move A6 an incremental amount */

  bool  Good2Move;
  float A6NewPos;
  float Pos = NewCommand.X;

  // New = Old + inc
  A6NewPos = A6Pos + Pos;

  // Check
  Good2Move = axisHome(6) * withinLims(6 , A6NewPos) * axisCal(6);

  if (Good2Move) {
    // Move
    A6Move( A6NewPos );
  }
}

void G07() {
  /* Move A7 an incremental amount */

  bool  Good2Move;
  float A7NewPos;
  float Pos = NewCommand.X;

  // New = Old + inc
  A7NewPos = A7Pos + Pos;

  // Check
  Good2Move = axisHome(7) * withinLims(7 , A7NewPos) * axisCal(7);

  if (Good2Move) {
    // Move
    A7Move( A7NewPos );
  }
}

void G08() {
  /* Move A8 an incremental amount */

  bool  Good2Move;
  float A8NewPos;
  float Pos = NewCommand.X;

  // New = Old + inc
  A8NewPos = A8Pos + Pos;

  // Check
  Good2Move = axisHome(8) * withinLims(8 , A8NewPos) * A8Known;

  if (Good2Move) {
    // Move
    A8Move( A8NewPos );
  }
}


void G12() {
  /* Move A2 to absolute position */

  bool  Good2Move;
  float A2NewPos = NewCommand.X;

  // Check
  Good2Move = axisHome(2) * withinLims(2 , A2NewPos);

  if (Good2Move) {
    // Move
    A2Move( A2NewPos );
  }
}

void G13() {
  /* Move A3 to absolute position */

  bool  Good2Move;
  float A3NewPos = NewCommand.X;

  // Check
  Good2Move = axisHome(3) * withinLims(3 , A3NewPos) * axisCal(3);

  if (Good2Move) {
    // Move
    A3Move( A3NewPos );
  }
}

void G14() {
  /* Move A4 to absolute position */

  bool  Good2Move;
  float A4NewPos = NewCommand.X;

  // Check
  Good2Move = axisHome(4) * withinLims(4 , A4NewPos) * axisCal(4);

  if (Good2Move) {
    // Move
    A4Move( A4NewPos );
  }
}

void G15() {
  /* Move A5 to absolute position */

  bool  Good2Move;
  float A5NewPos = NewCommand.X;

  // Check
  Good2Move = axisHome(5) * withinLims(5 , A5NewPos);

  if (Good2Move) {
    // Move
    A5Move( A5NewPos );
  }
}

void G16() {
  /* Move A6 to absolute position */

  bool  Good2Move;
  float A6NewPos = NewCommand.X;

  // Check
  Good2Move = axisHome(6) * withinLims(6 , A6NewPos) * axisCal(6);

  if (Good2Move) {
    // Move
    A6Move( A6NewPos );
  }
}

void G17() {
  /* Move A7 to absolute position */

  bool  Good2Move;
  float A7NewPos = NewCommand.X;

  // Check
  Good2Move = axisHome(7) * withinLims(7 , A7NewPos) * axisCal(7);

  if (Good2Move) {
    // Move
    A7Move( A7NewPos );
  }
}

void G18() {
  /* Move A8 to absolute position */

  bool  Good2Move;
  float A8NewPos = NewCommand.X;

  // Check
  Good2Move = axisHome(8) * withinLims(8 , A8NewPos);

  if (Good2Move) {
    // Move
    A8Move( A8NewPos );
  }
}

void G21() {
  /* Move A3 an incremental amount, moving A1 synchronously */

  bool  Good2Move;
  float A1A3NewPos;
  float Pos = NewCommand.X;

  // New = Old + inc
  A1A3NewPos = A3Pos + Pos;

  // Check
  Good2Move = axisHome(3) * withinLims(3 , A1A3NewPos) * axisCal(3) * axisCal(1);

  if (Good2Move) {
    // Move
    A1A3Move( A1A3NewPos );
  }
}

void G22() {
  /* Move A3 to absolute position, moving A1 synchronously */

  bool  Good2Move;
  float A1A3NewPos = NewCommand.X;

  // Check
  Good2Move = axisHome(3) * withinLims(3 , A1A3NewPos) * axisCal(3) * axisCal(1);

  if (Good2Move) {
    // Move
    A1A3Move( A1A3NewPos );
  }
}

void G23() {
  /* Move A6 an incremental amount, moving A1 synchronously */

  bool  Good2Move;
  float A1A6NewPos;
  float Pos = NewCommand.X;

  // New = Old + inc
  A1A6NewPos = A6Pos + Pos;

  // Check
  Good2Move = axisHome(6) * withinLims(6 , A1A6NewPos) * axisCal(6) * axisCal(1);

  if (Good2Move) {
    // Move
    A1A6Move( A1A6NewPos );
  }
}

void G24() {
  /* Move A6 to absolute position, moving A1 synchronously */

  bool  Good2Move;
  float A1A6NewPos = NewCommand.X;

  // Check
  Good2Move = axisHome(6) * withinLims(6 , A1A6NewPos) * axisCal(6) * axisCal(1);

  if (Good2Move) {
    // Move
    A1A6Move( A1A6NewPos );
  }
}

void G25() {
  /* Move A3 and A4 in arc, of radius A3 to abs theta position */

  bool  Good2Move;
  float theta = NewCommand.X;

  // Check
  Good2Move = axisHome(3) * axisCal(3) * axisHome(4) * axisCal(4);
  
  if (Good2Move) {
    // Move
    A3A4Arc( theta );
  }
}

void G26() {
  /* Move A6 and A7 in arc, of radius A6 to abs theta position */

  bool  Good2Move;
  float theta = NewCommand.X;

  // Check
  Good2Move = axisHome(6) * axisCal(6) * axisHome(7) * axisCal(7);

  if (Good2Move) {
    // Move
    A6A7Arc( theta );
  }

}


void G28() {
  /* Move A6 an incremental amount, moving A1 synchronously */

  // Move in 'm', in direction 'dir', 'p' times
  float   m = 150;
  float dir = -1;

  // Some vars
  bool  Good2Move;
  float A1A3NewPos;
  float p = NewCommand.X;

  for (int i = 1; i <= int(p); i++) {
    A1A3NewPos = A3Pos + (dir * m);

    // Check
    Good2Move = axisHome(3) * withinLims(3 , A1A3NewPos) * axisCal(3) * axisCal(1);

    if (Good2Move) {
      // Move
      A1A3Move( A1A3NewPos );

      delay(2000);
      dir = dir * -1;
    }
  }
}



void G31() {
  /* Raise A5, then AC */

  // Send command
  cmd2Slave ( 1 , 'q' , 0 );

  // Update
  A5Known  = false;
  A5Active = true;
}

void G32() {
  /* Raise A8, then AC */

  // Send command
  cmd2Slave ( 1 , 'r' , 0 );

  // Update
  A8Known  = false;
  A8Active = true;
}

void G33() {
  /* A5 shift down */

  // Send command
  cmd2Slave ( 1 , 'o' , 0 );

  // Update
  A5Known  = false;
  A5Active = false;
}

void G34() {
  /* A8 shift down */

  // Send command
  cmd2Slave ( 1 , 'p' , 0 );

  // Update
  A8Known  = false;
  A8Active = false;
}

void G35() {
  /* A5 Adaptive - ON */

  // Send command
  cmd2Slave ( 1 , 'k' , 0 );

  // Update
  A5Known  = false;
  A5Active = true;

}

void G36() {
  /* A5 Adaptive - OFF */

  // Send command
  cmd2Slave ( 1 , 'm' , 0 );

  // Update
  A5Known  = false;
  A5Active = false;
}

void G37() {
  /* A8 Adaptive - ON */

  // Send command
  cmd2Slave ( 1 , 'l' , 0 );

  // Update
  A8Known  = false;
  A8Active = true;
}

void G38() {
  /* A8 Adaptive - OFF */

  // Send command
  cmd2Slave ( 1 , 'n' , 0 );

  // Update
  A8Known  = false;
  A8Active = false;
}

void G40() {
  /* Home all axes */

  G42();
  G43();
  G44();
  G45();
  G46();
  G47();
  G48();
}






void G42() {
  /* Home A2 */

  cmd2Slave ( 1 , 'h' , 0 );

  A2Pos  = A2HomePos;
  A2Home = true;
}

void G43() {
  /* Home A3 */

  cmd2Slave ( 2 , 'p' , 0 );

  A3Pos  = A3HomePos;
  A3Home = true;
}

void G44() {
  /* Home A4 */

  cmd2Slave ( 2 , 'q' , 0 );

  A4Pos  = A4HomePos;
  A4Home = true;
}

void G45() {
  /* Home A5 */

  cmd2Slave ( 1 , 'i' , 0 );

  A5Pos   = A5HomePos;
  A5Home  = true;
  A5Known = true;
}

void G46() {
  /* Home A6 */

  cmd2Slave ( 2 , 'r' , 0 );

  A6Pos  = A6HomePos;
  A6Home = true;
}

void G47() {
  /* Home A7 */

  cmd2Slave ( 2 , 's' , 0 );

  A7Pos  = A7HomePos;
  A7Home = true;
}

void G48() {
  /* Home A8 */

  cmd2Slave ( 1 , 'j' , 0 );

  A8Pos   = A8HomePos;
  A8Home  = true;
  A8Known = true;
}

void G50() {
  delay(2000);
}
