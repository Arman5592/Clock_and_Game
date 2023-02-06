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

int HXN,HYN,HZN; //hall effect readings
unsigned long lastHallMeasurement = 0;
  
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
  display.setCursor(0,0);

  display.print(celsius);
  display.print("C | ");
  display.print(fahrenheit);
  display.print("F");

  display.setCursor(0,28);
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
  if(millis() - lastHallMeasurement > 200){
    lastHallMeasurement = millis();
    Wire.beginTransmission(0x0c);
    Wire.write(0x00);
    Wire.endTransmission(false);
    Wire.requestFrom(0x0c, 13);
    delay(10);
    uint16_t reg[13];
    for (int i=0; i<13; i++){ 
      reg[i] = Wire.read();
    }

    
    //print regs in human readable format
    int16_t HX = reg[5] + (reg[6]<<8);
    int16_t HY = reg[7] + (reg[8]<<8);
    int16_t HZ = reg[9] + (reg[10]<<8);

    if((reg[4]>>1)%2 || (HX==0)&&(HY==0)&&(HZ)==0){
      resetHall();
    }
  
    HXN = ((HX + 39)*100) / 49; //bounds are -39 and 10
    HYN = ((HY - 2)*100) / 63; //bounds are 2 and 61
    HZN = ((HZ + 21)*100) / 49; //bounds are -21 and 28
    
    Serial.print("x:");Serial.println(HXN);
    Serial.print("y:");Serial.println(HYN);
    Serial.print("z:");Serial.println(HZN);
  }
  
  String hallDir;
  
  bool isWest=0;
  bool isEast=0;
  bool isNorth=0;
  bool isSouth=0;
  
  if (HXN > 60){
    isWest = 1;
  }
  if (HXN < 40) {
    isEast = 1;
  }
  if (HYN > 60) {
    isNorth = 1;
  }
  if (HYN < 40) {
    isSouth = 1;
  }

  if (isWest){
    if (isNorth)
      hallDir = "NW";
    else if (isSouth)
      hallDir = "SW";
    else
      hallDir = "W";
  }
  else if (isEast){
    if (isNorth)
      hallDir = "NE";
    else if (isSouth)
      hallDir = "SE";
    else
      hallDir = "E";
  }
  else if (isNorth){
    hallDir = "N";
  }
  else if (isSouth){
    hallDir = "S";
  }
  else{
    hallDir = "X";
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(0,0);
  display.print("Facing: ");
  display.setTextSize(2);
  display.print(hallDir);

  display.setCursor(0, 32);
  display.setTextSize(1);
  display.print("Incl.:");
  display.setTextSize(2);
  display.print(HZN);
  display.display();
}

void resetHall(){
  Wire.beginTransmission(0x0c);
  Wire.write(0x31);
  Wire.write(0x00);
  Wire.endTransmission(true);
  
  digitalWrite(14, LOW);
  delay(10);
  digitalWrite(14, HIGH);

  Wire.beginTransmission(0x0c);
  Wire.write(0x31);
  Wire.write(0x02);
  Wire.endTransmission(true);
}
