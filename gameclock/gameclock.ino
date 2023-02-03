#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "Arduino.h"
#include "uRTCLib.h"
#include "DHT.h"
#include "NewPing.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define MENU_MAX 2      // max. menu counter (num. of menus - 1)
#define DHTPIN 15
#define DHTTYPE DHT11
#define TRIGGER_PIN 10
#define ECHO_PIN 11
#define MAX_DISTANCE 400

bool gameMode = 0;
bool newGame = 0;
int menuPage = 0;
bool clockTap = 0;
unsigned long tapCheckTime = 0;
unsigned long menuChangeTime = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, OLED_RESET);
uRTCLib rtc(0x68);
Adafruit_MPU6050 mpu;
DHT dht(DHTPIN, DHTTYPE);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

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
  if (!mpu.begin(0x69, &Wire, 0)) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  //display.display();
  //delay(1000);
  display.clearDisplay();
  display.display();
  
  // for example to set January 13 2022 at 12:56 you would call:
  // rtc.set(0, 9, 20, 7, 21, 1, 23);
  // rtc.set(second, minute, hour, dayOfWeek, dayOfMonth, month, year)
  // set day of week (1=Sunday, 7=Saturday)

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  dht.begin();

  
}

void setup1(){
  pinMode(16, INPUT);
}

void loop() {
  if(gameMode){
    if(newGame){
      resetGame();
    }
    gameLoop();
  }
  else {
    if (millis() - tapCheckTime > 20){
      if(clockMenuTap()){
        if(millis() - menuChangeTime > 500){
          menuChangeTime = millis();
          if(menuPage >= MENU_MAX){
            menuPage = 0;
          }
          else {
            menuPage += 1;
          }
        }
      }
    }
    
    if(menuPage == 0){
      mainClockMenu();
    }
    else if (menuPage == 1){
      dateMenu();
    }
    else if (menuPage == 2){
      ambienceMenu();
    }
    else {
      
    }
  }
}

void loop1(){
  if(gameMode==0 && digitalRead(16)==1){
    newGame = 1;
    gameMode = 1;
  }
  else{
    gameMode = digitalRead(16);
    newGame = 0;
  }
  //Serial.println(sonar.ping_cm());
  delay(200);
}
