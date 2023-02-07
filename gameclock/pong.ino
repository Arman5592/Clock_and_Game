static const unsigned char PROGMEM racket_logo_bmp[] = {  //16x16 pixels
  0xe0, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0xe0, 0x00,
  0xe0, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0xe0, 0x00
};

int leftY, rightY, ballX, ballY;
int scoreLeft, scoreRight;
int ballSpeedX, ballSpeedY;

void resetPongGame(){
  leftY = rightY = 24;
  ballX = 62;
  ballY = 30;
  scoreLeft = scoreRight = 0;
  ballSpeedX = random(2)?-1:1;
  ballSpeedY = random(2)?-1:1;
}

void pongGame(){
  display.clearDisplay();
  display.drawBitmap(0, leftY, racket_logo_bmp, 16, 16, WHITE);
  display.drawBitmap(124, rightY, racket_logo_bmp, 16, 16, WHITE);
  display.setCursor(ballX, ballY);
  display.setTextSize(1);
  display.print("o");
  display.setCursor(16,48);
  display.print(scoreLeft);
  display.setCursor(72,48);
  display.print(scoreRight);
  display.display();

  if(ballY == 0 || ballY == 60){
    ballSpeedY = -1 * ballSpeedY;
  }
  if(ballX == 2){
    if(ballY > leftY && ballY < leftY + 14){
      ballSpeedX = -1 * ballSpeedX;
      if(ballY < leftY + 7){
        ballSpeedY = 1;
      }
      else{
        ballSpeedY = -1;
      }
    }
    else{
      ballX = 62; ballY = 30;
      scoreRight ++;
      leftY = rightY = 24;
    }
  }
  if(ballX == 124){
    if(ballY > rightY && ballY < rightY + 14){
      ballSpeedX = -1 * ballSpeedX;
      if(ballY < rightY + 7){
        ballSpeedY = 1;
      }
      else{
        ballSpeedY = -1;
      }
    }
    else{
      ballX = 62; ballY = 30;
      scoreLeft ++;
      leftY = rightY = 24;
    }
  }

  ballX += ballSpeedX;
  ballY += ballSpeedY;

  if(digitalRead(18)){
    if(leftY > 0){
      leftY --;
    }
  }
  if(digitalRead(19)){
    if(leftY < 48){
      leftY ++;
    }
  }
  if(digitalRead(20)){
    if(rightY > 0){
      rightY --;
    }
  }
  if(digitalRead(21)){
    if(rightY < 48){
      rightY ++;
    }
  }
  
  
}
