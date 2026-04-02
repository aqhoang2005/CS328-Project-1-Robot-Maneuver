void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

<<<<<<< Updated upstream
}
=======
    if (cmd == 'F') {
      Forward(128);
    }
    else if (cmd == 'B') {
      Backward(128);
    }
    else if (cmd == 'S') {
      StopMotors();
    }
    else if (cmd == 'L') {
      Left(100);
    }
    else if (cmd == 'R') {
      Right(100);
    }
    else if (cmd == 'Q'){ //these were all calibrated using a dead ish battery
      int speed = 200;
      Forward(speed);
      delay(360);
      
      Halt(speed); //First turn
      delay(500);
      Turn(speed, 80);
      delay(105);
      Accelerate(speed);
      delay(320);

      Halt(speed); //Second turn
      delay(500);
      Turn(speed, 80);
      delay(110);
      Accelerate(speed);      
      delay(310);

      Halt(speed);
      delay(500);
      Turn(speed, 80);
      delay(100);
      Accelerate(speed);      
      delay(320);

      Halt(speed); //Fourth turn
      // delay(500);
      // Turn(speed, 100);
      // delay(100);
      // Accelerate(speed);      
      // delay(340);

      // Halt(speed);

      /*Old Code in case*/

      // Forward(speed);
      // delay(500);//
      // StopMotors();
      
      // delay(100);

      // Right(speed);
      // digitalWrite(RIGHTREAR, HIGH);
      // digitalWrite(RIGHTFRONT, HIGH);
      // delay(160);// delay was 160 super charged, delay was 180 when charged, 200 when dead ish
      // digitalWrite(RIGHTREAR, LOW);
      // digitalWrite(RIGHTFRONT, LOW);
      // StopMotors();
      // digitalWrite(LEFTREAR, HIGH);
      // digitalWrite(RIGHTREAR, HIGH);

      // delay(100);//new 0
      // digitalWrite(LEFTREAR, LOW);
      // digitalWrite(RIGHTREAR, LOW);

      // Forward(speed);
      // delay(500);// 400 dead, 600 alive
      // StopMotors();

      // delay(100);//new 2 and below

      // Right(speed);
      // digitalWrite(RIGHTREAR, HIGH);
      // digitalWrite(RIGHTFRONT, HIGH);
      // delay(240);//was 240
      // StopMotors();

      // delay(100);//new 3

      // Forward(speed);
      // delay(600);//300 dead, 400 alive was 500
      // StopMotors();
      // digitalWrite(RIGHTREAR, LOW);
      // digitalWrite(RIGHTFRONT, LOW);
      // StopMotors();
      // digitalWrite(LEFTREAR, HIGH);
      // digitalWrite(RIGHTREAR, HIGH);

      // delay(100);//new 4 and below

      // Right(speed);
      // digitalWrite(RIGHTFRONT, HIGH);
      // digitalWrite(RIGHTREAR, HIGH);
      // delay(180);
      // StopMotors();
      // digitalWrite(RIGHTREAR, LOW);
      // digitalWrite(RIGHTFRONT, LOW);
      // StopMotors();
      // digitalWrite(LEFTREAR, HIGH);
      // digitalWrite(RIGHTREAR, HIGH);

      // delay(100);

      // Forward(speed);
      // delay(500);
      // StopMotors();
      // digitalWrite(RIGHTREAR, LOW);
      // digitalWrite(RIGHTFRONT, LOW);
      // StopMotors();
      // digitalWrite(LEFTREAR, HIGH);
      // digitalWrite(RIGHTREAR, HIGH);

      // delay(100);
    }
  }
}
>>>>>>> Stashed changes
