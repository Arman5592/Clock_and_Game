char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

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
  delay(500);
  Serial.print(rtc.hour());
  Serial.print(':');
  Serial.println(rtc.minute());
}
