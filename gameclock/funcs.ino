char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

float accZ[5] = {10.0,10.0,10.0,10.0,10.0};
float accZMax = -20.0;
float accZMin = 20.0;
sensors_event_t a, g, temp;

unsigned long lastDHTMeasurement = 0;
uint8_t humidity = 25;
uint8_t celsius = 25;
uint8_t fahrenheit = 70;
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B00000000,
  B00000000, B00110000,
  B00000000, B01110000,
  B00000000, B11100000,
  B00000001, B11100000,
  B00000011, B11100000,
  B00000111, B11110000,
  B00001111, B11110000,
  B00001111, B11111000,
  B00011101, B11111000,
  B00011001, B11111000,
  B00011001, B11111000,
  B00011001, B11111000,
  B00001100, B11110000,
  B00000111, B11100000,
  B00000011, B11000000 };
  
void mainClockMenu(){
  rtc.refresh();
  display.clearDisplay();
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setCursor(0,20);
  
  display.print(rtc.hour());
  display.print(':');
  display.print(rtc.minute());
  
  display.display();
  
}

bool clockMenuTap(){
  tapCheckTime = millis();

  accZ[0] = accZ[1]; accZ[1] = accZ[2]; accZ[2] = accZ[3]; accZ[3] = accZ[4];
  mpu.getEvent(&a, &g, &temp);
  accZ[4] = a.acceleration.z;

  accZMax = -20.0;
  accZMin = 20.0;
  for(int i=0; i<5; i++){
    if(accZ[i] > accZMax){
      accZMax = accZ[i];
    }
    if(accZ[i] < accZMin){
      accZMin = accZ[i];
    }
  }

  if(accZMax - accZMin > 0.25){
      //Serial.println("tap");
      return 1;
  }
  return 0;
}

void dateMenu(){
  rtc.refresh();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,4);
  
  display.print(daysOfTheWeek[rtc.dayOfWeek()-1]);

  display.setTextSize(2);
  display.setCursor(0,32);

  display.print(rtc.day());
  display.print('/');
  display.print(rtc.month());
  display.print("/20");
  display.print(rtc.year());
  
  display.display();
}

void ambienceMenu(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,4);

  display.print(celsius);
  display.print("C | ");
  display.print(fahrenheit);
  display.print("F");

  display.setCursor(0,32);
  display.print(humidity);
  display.print("%");

  display.setCursor(40,30);
  display.drawBitmap(40, 30, logo_bmp, 16, 16, WHITE);
  
  display.display();

  
    
  if(millis() - lastDHTMeasurement > 2000){
    lastDHTMeasurement = millis();
    
    humidity = dht.readHumidity();
    celsius = dht.readTemperature();
    fahrenheit = dht.readTemperature(true);
  
    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(celsius) || isnan(fahrenheit)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
  }
}

void hallMenu(){
  
}
