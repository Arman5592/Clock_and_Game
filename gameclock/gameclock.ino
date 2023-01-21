#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Arduino.h"
#include "uRTCLib.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

bool gameMode = 0;
int menuPage = 0;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, OLED_RESET);
uRTCLib rtc(0x68);


void setup() {
  pinMode(25, OUTPUT);
  Wire1.setSDA(2);
  Wire1.setSCL(3);
  
  Serial.begin(9600);
  
  URTCLIB_WIRE.setSDA(4);
  URTCLIB_WIRE.setSCL(5);
  URTCLIB_WIRE.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();
  // for example to set January 13 2022 at 12:56 you would call:
  // rtc.set(0, 21, 6, 7, 21, 1, 23);
  // rtc.set(second, minute, hour, dayOfWeek, dayOfMonth, month, year)
  // set day of week (1=Sunday, 7=Saturday)
}

void loop() {
  if(gameMode){
    
  }
  else {
    //inja tap ro check konim age tap karde bod menuPage update beshe
    if(menuPage == 0){
      mainClockMenu();
    }
    else {
      
    }
  }
}
