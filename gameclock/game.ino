
static const unsigned char PROGMEM knight_logo_bmp[] = {  //24x24 pixels
  0x18, 0x00, 0x18, 0x30, 0x00, 0x0c, 0x30, 0x00, 0x0c, 0x30, 0x00, 0x0c, 0x38, 0x00, 0x1c, 0x1d, 0xff, 0xb8,
  0x1f, 0xff, 0xf8, 0x0f, 0xff, 0xf0, 0x0e, 0x7e, 0x70, 0x0c, 0x3c, 0x30, 0x0c, 0x3c, 0x30, 0x0c, 0x3c, 0x30,
  0x0e, 0x7e, 0x70, 0x07, 0xff, 0xe0, 0x07, 0xff, 0xe0, 0x03, 0xff, 0xc0, 0x00, 0x31, 0x00, 0x00, 0xd4, 0x80,
  0x00, 0x92, 0x40, 0x00, 0x89, 0x40, 0x00, 0x94, 0x40, 0x00, 0xf3, 0xc0, 0x00, 0x73, 0x80, 0x00, 0x73, 0x80
};
static const unsigned char PROGMEM bug_logo_bmp[] = {   //16x16 pixels
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0xa0, 0x08, 0xa2, 0x04, 0xa4,
  0x12, 0xa9, 0x09, 0xf2, 0x06, 0x0c, 0xf4, 0x45, 0x8d, 0x52, 0xa9, 0x11, 0xc8, 0x01, 0x7f, 0xff 
};
static const unsigned char PROGMEM dash_logo_bmp[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0xfe, 0x00, 0x00, 0x7f, 0xf8, 0x00, 0x00,
  0x00, 0x00, 0x07, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x9e, 0x00, 0x00,
  0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xee, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};
static const unsigned char PROGMEM thorn_logo_bmp[] = {
  0x1c, 0x0c, 0x0f, 0x3c, 0x0f, 0x0c, 0x3c, 0x0e
};

bool gameNotOver = 1;
bool isJumping = 0;
bool isFalling = 0;
int knightY = 40;
int bugDistanceCounter = 48;
int bugX[2] = {-1, -1};
int dashLeft = 0;
int pingCounter = 5;
int thornX = 350;
int score = 0;

void resetGame(){
  bugX[0] = bugX[1] = -1;
  knightY = 40;
  gameNotOver = 1;
  isJumping = isFalling = 0;
  bugDistanceCounter = 48;
  dashLeft = 0;
  pingCounter = 5;
  thornX = 350;
  score = 0;
}

void gameLoop(){
  
  display.clearDisplay();
  
  display.drawBitmap(40, knightY, knight_logo_bmp, 24, 24, WHITE);

  int dist;
  int i;
  //check losing condition - TODO: walls
  for(i=0; i<2; i++){
    dist = bugX[i] - 40;
    if (dist < 0){ dist = -dist; }

    if (dist < 15) {
      if (knightY > 34){
        gameNotOver = 0;
      }
    }
  }

  if (thornX < 42 && thornX > 38)
    gameNotOver = 0;

  if(!gameNotOver){
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,4);
    display.print("Game Over");

    display.setTextSize(1);
    display.setCursor(0,20);
    display.print("Score:");
    display.print(score);
    display.display();
    return;
  }

  i=0;
  for(i; i<2; i++){
    if (bugX[i] != -1)
      display.drawBitmap(bugX[i], 48, bug_logo_bmp, 16, 16, WHITE);
  }

  if (thornX < 128){
    i = 0;
    for(i; i<8; i++){
      display.drawBitmap(thornX, i*8, thorn_logo_bmp, 8, 8, WHITE);
    }

    
  }
  if (thornX > 40 && thornX < 90) {
    bugDistanceCounter = 40;
  }
  
  i=0;
  for(i; i<2; i++){
    if(bugX[i] == -1 and bugDistanceCounter<=0){
      if(random(10) < 6){
        bugX[i] = 127;
        bugDistanceCounter = 32;
        break;
      }
    }
  }
  bugDistanceCounter -= 1;
  
  i=0;
  for(i; i<3; i++){
    if (bugX[i] != -1){
      bugX[i] -= 2;
    }
    if (bugX[i] < 0){
      bugX[i] = -1;
    }
  }

  thornX -= 1;
  if (thornX == -1) thornX = 350;

  if (millis() - tapCheckTime > 20){
      if(clockMenuTap() && (!isJumping) && (!isFalling)){
        isJumping = 1;
      }
      pingCounter -= 1;
      if (pingCounter == -1) pingCounter = 5;
      
      if (!pingCounter){
         if (sonar.ping_cm() < 30){
           //dash
           dashLeft = 8;
           i=0;
           for(i; i<3; i++){
             if (bugX[i] != -1){
               bugX[i] -= 32;
             }
              if (bugX[i] < 0){
               bugX[i] = -1;
             }
           }
           thornX -= 32;
           if (thornX < 0) thornX = 350;
         }
      }
      score += 1;
  }

  if (isJumping){
    if (knightY > 12) knightY -= 2;
    else  {
      isJumping = 0;
      isFalling = 1;
    }
  }

  if (isFalling) {
    if (knightY < 40) knightY += 2;
    else isFalling = 0;
  }

  if (dashLeft>0) {
    display.drawBitmap(24, knightY, dash_logo_bmp, 16, 24, WHITE);
  }

  dashLeft -= 1;
  
  display.display();

  
  
}
