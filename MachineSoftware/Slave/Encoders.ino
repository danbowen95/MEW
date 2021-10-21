//// Encoders
//// includes - A2a, A2b, A5a, A5b, A8a, A8b
////
//// A2a
void A2_doEncoderA() {
  // look for a low-to-high on channel A
  if (digitalRead(A2_encoderPinA) == HIGH) {

    // check channel B to see which way encoder is turning
    if (digitalRead(A2_encoderPinB) == LOW) {
      A2_encoderPos = A2_encoderPos + 1;         // CW
    }
    else {
      A2_encoderPos = A2_encoderPos - 1;         // CCW
    }
  }

  else   // must be a high-to-low edge on channel A
  {
    // check channel B to see which way encoder is turning
    if (digitalRead(A2_encoderPinB) == HIGH) {
      A2_encoderPos = A2_encoderPos + 1;          // CW
    }
    else {
      A2_encoderPos = A2_encoderPos - 1;          // CCW
    }
  }
  // use for debugging - remember to comment out
  Serial.println(A2_encoderPos);
  //slaveStatus.A2Pos = A2_encoderPos;
}
//
// A2b
void A2_doEncoderB() {
  // look for a low-to-high on channel B
  if (digitalRead(A2_encoderPinB) == HIGH) {

    // check channel A to see which way encoder is turning
    if (digitalRead(A2_encoderPinA) == HIGH) {
      A2_encoderPos = A2_encoderPos + 1;         // CW
    }
    else {
      A2_encoderPos = A2_encoderPos - 1;         // CCW
    }
  }

  // Look for a high-to-low on channel B
  else {
    // check channel B to see which way encoder is turning
    if (digitalRead(A2_encoderPinA) == LOW) {
      A2_encoderPos = A2_encoderPos + 1;          // CW
    }
    else {
      A2_encoderPos = A2_encoderPos - 1;          // CCW
    }
  }
  Serial.println(A2_encoderPos);
  //slaveStatus.A2Pos = A2_encoderPos;
}
////
//// A5a
//void A5_doEncoderA() {
//  if (digitalRead(A5_encoderPinA) == HIGH) {
//    if (digitalRead(A5_encoderPinB) == LOW) {
//      A5_encoderPos = A5_encoderPos + 1;         // CW
//    }
//    else {
//      A5_encoderPos = A5_encoderPos - 1;         // CCW
//    }
//  }
//  else
//  {
//    if (digitalRead(A5_encoderPinB) == HIGH) {
//      A5_encoderPos = A5_encoderPos + 1;          // CW
//    }
//    else {
//      A5_encoderPos = A5_encoderPos - 1;          // CCW
//    }
//  }
//}
////
//// A5b
//void A5_doEncoderB() {
//  if (digitalRead(A5_encoderPinB) == HIGH) {
//    if (digitalRead(A5_encoderPinA) == HIGH) {
//      A5_encoderPos = A5_encoderPos + 1;         // CW
//    }
//    else {
//      A5_encoderPos = A5_encoderPos - 1;         // CCW
//    }
//  }
//  else {
//    if (digitalRead(A5_encoderPinA) == LOW) {
//      A5_encoderPos = A5_encoderPos + 1;          // CW
//    }
//    else {
//      A5_encoderPos = A5_encoderPos - 1;          // CCW
//    }
//  }
//}
//
////
//// A8a
//void A8_doEncoderA() {
//  if (digitalRead(A8_encoderPinA) == HIGH) {
//    if (digitalRead(A8_encoderPinB) == LOW) {
//      A8_encoderPos = A8_encoderPos + 1;         // CW
//    }
//    else {
//      A8_encoderPos = A8_encoderPos - 1;         // CCW
//    }
//  }
//  else
//  {
//    if (digitalRead(A8_encoderPinB) == HIGH) {
//      A8_encoderPos = A8_encoderPos + 1;          // CW
//    }
//    else {
//      A8_encoderPos = A8_encoderPos - 1;          // CCW
//    }
//  }
//}
////
//// A5b
//void A8_doEncoderB() {
//  if (digitalRead(A8_encoderPinB) == HIGH) {
//    if (digitalRead(A8_encoderPinA) == HIGH) {
//      A8_encoderPos = A8_encoderPos + 1;         // CW
//    }
//    else {
//      A8_encoderPos = A8_encoderPos - 1;         // CCW
//    }
//  }
//  else {
//    if (digitalRead(A8_encoderPinA) == LOW) {
//      A8_encoderPos = A8_encoderPos + 1;          // CW
//    }
//    else {
//      A8_encoderPos = A8_encoderPos - 1;          // CCW
//    }
//  }
//}
//
//void errorCodes( int n ) {
//  /* These display error codes for debugging.
//      When running fine, red LED on pin XX does not flash
//      When error, LED flashes.
//      Count the flashes between 2 sec long flash for error number
//
//      Error codes
//      1 - Not used
//      2 - Home axis number not recognised
//      3 -
//  */
//}
