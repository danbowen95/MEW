/* M Code commands 

  M00() - Print machine status
  M01() - Print individual axis status
  M02() - Print all axes status
  M03() - Print status of active control
  M04() - Print status of vacuum
  M05() - Calibrate and home all axes
  M06() - A5 vac on
  M07() - A5 vac off
  M08() - A8 vac on
  M09() - A8 vac off
  M10() - Calibrate A1 A3 A4 A6 A7
  M11() - Calibrate A1 
  M13() - Calibrate A3
  M14() - Calibrate A4
  M16() - Calibrate A6
  M17() - Calibrate A7
  
*/


void M00() {
  /* Print All machine status*/

  // do eeit
  M02();
  M03();
  M04();
}

void M01() {
  /* Print axis status*/
  int axis = int(NewCommand.X);

  printAxisStatus(axis);
}

void M02() {
  /* Print all axis status */

  for (int i = 1 ; i <= 8 ; i++) {
    // Print
    printAxisStatus( i );
  }
}

void M03() {
  /* Print Active control axis status */

  // Print
  printActiveStatus();
}

void M04() {
  /* Print vacuum status */

  // Print
  printVacStatus();
}

void M05() {
  /* Calibrate and home all axes */

  int t = 500;
  
  M11();
  delay(t);
  G42();
  delay(t);
  M13();
  delay(t);
  G43();
  delay(t);
  M14();
  delay(t);
  G44();
  delay(t);
  G45();
  delay(t);
  M16();
  delay(t);
  G46();
  delay(t);
  M17();
  delay(t);
  G47();
  delay(t);
  G48;
  delay(t);
  
}

void M06() {
  /* A5 vac ON */
  VacControl( 5 , true );
}

void M07() {
  /* A5 vac OFF */
  VacControl( 5 , false );
}

void M08() {
  /* A8 vac ON */
  VacControl( 8 , true );
}

void M09() {
  /* A8 vac OFF */
  VacControl( 8 , false );
}


void M10() {
  /* Calibrate all axes */
  M11();
  M13();
  M14();
  M16();
  M17();
}

void M11() {
  /* Calibrate A1 */

  cmd2Slave ( 2 , 'k' , 0 );

  A1Cal = true;
}

void M13() {
  /* Calibrate A3 */

  cmd2Slave ( 2 , 'l' , 0 );

  A3Cal = true;
}

void M14() {
  /* Calibrate A4 */

  cmd2Slave ( 2 , 'm' , 0 );

  A4Cal = true;
}

void M16() {
  /* Calibrate A6 */

  cmd2Slave ( 2 , 'n' , 0 );

  A6Cal = true;
}

void M17() {
  /* Calibrate A7 */

  cmd2Slave ( 2 , 'o' , 0 );

  A7Cal = true;
}
