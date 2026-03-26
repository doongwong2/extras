#include <LiquidCrystal.h>
LiquidCrystal lcd(7,8,9,10,11,12);//declare what pins to use on the LCD.
// RS, E, Datapin 4 - datapin 7.

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2); // 16 X2 LCD.
  lcd.clear();
  lcd.print("Hello World!");
}

void loop() {
  // put your main code here, to run repeatedly:
}

