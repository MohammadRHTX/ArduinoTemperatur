#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

int temperature = 0;
int wheel = 1;
int button = 2;
int buzzer = 3;

const int B = 4275;
const int R0 = 10000;

int state = 0;
int x = 1;

int maxTemp = 30;
int minTemp = 15;

void setup() {
  lcd.begin(16, 2);
  lcd.setRGB(0, 255, 0);
  
  pinMode(temperature,INPUT);
  pinMode(wheel,INPUT);
  pinMode(button,INPUT); //button
  pinMode(buzzer,OUTPUT); //buzzer
}

float Temperature(float a, int B, int R0) {
  float temp;
  float R = 1023.0/a-1.0;
  R = R0 * R;
  temp = 1.0/(log(R/R0)/B+1/298.15)-273.15; //finds temperature in celcius
  return temp;
}

void loop() {
  float a = analogRead(temperature);
  float b = analogRead(wheel);

  float temp = Temperature(a,B,R0); //gets the temperature in celcius (a is resistance, B & R0 are constants)

  
  if (digitalRead(button)==HIGH and x==1) { //changes state when button is pressed
    state += 1;
    x = 0;
  } else if (digitalRead(button)==LOW) {
    x = 1;
  }
  if (state>2) { //changes state back to 0 if it's more than 2
    state = 0;
  }

  if ((temp>maxTemp or temp<minTemp) and state == 0) { //if temp is more than maxTemp or less than minTemp and it's the "normal" state
    tone(buzzer, 100);
    if (temp>maxTemp) { //if it's too hot print too hot
      lcd.setRGB(255,0,0);
      lcd.setCursor(0,0);
      lcd.print("    Too hot!        ");
      lcd.setCursor(0,1);
      lcd.print("                ");
    }
    else if (temp<minTemp) { //if it's too cold print too cold
      lcd.setRGB(0,0,255);
      lcd.setCursor(0,0);
      lcd.print("    Too cold!                  ");
      lcd.setCursor(0,1);
      lcd.print("                ");
    }
  } 
  else if (state == 0) { //"normal" state - prints the temperature on the display
    noTone(buzzer); //makes sure the buzzer makes no sound
    lcd.setRGB(0, 255, 0); //sets display color to green

    lcd.setCursor(0, 0);
    lcd.print("    " + String(temp) + (char)223 + "C     ");

    lcd.setCursor(0, 1);
    lcd.print(String(minTemp) + (char)223 + "C       " + String(maxTemp) + (char)223 + "C");    
  } 
  else if (state == 1) { //state to change max temp
    noTone(buzzer); //makes sure the buzzer makes no sound
    lcd.setRGB(0, 0, 255); //sets display color to blue

    lcd.setCursor(0, 0);
    lcd.print(" Set Min Temp:  ");

    lcd.setCursor(0, 1);
    float x = map(b,0,1023,40,0); //takes the value of the wheel and changes it to a number between 0 and 40
    minTemp = x;
    lcd.print("      " + String(minTemp) + (char)223 + "C      ");
  } 
  else if (state == 2) { //state to change min temp
    noTone(buzzer); //makes sure the buzzer makes no sound
    lcd.setRGB(255, 0, 0); //sets display color to red

    lcd.setCursor(0, 0);
    lcd.print(" Set Max Temp:  ");

    lcd.setCursor(0, 1);
    float x = map(b,0,1023,40,0); //takes the value of the wheel and changes it to a number between 0 and 40
    maxTemp = x;
    lcd.print("      " + String(maxTemp) + (char)223 + "C      ");
  }
}
