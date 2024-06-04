/**
 * Title: Greeting Machine
 * Author: Daniel Hoang
 * Date: 05/20/2024
 * Version: v1.0
 * Purpose: Use for MakerLab Ciao Bot
*/

//Include nesscessery libraries
#include <Arduino.h>
#include <Wire.h>
#include "LiquidCrystal_I2C.h"
#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>

//Declare variables and objects
int int_sokhach;
int int_checkin;

SoftwareSerial mySoftwareSerial(2, 3);  // Turn Arduino D2 pin into RX pin, turn Arduino D3 pin into TX pin (Software UART)
LiquidCrystal_I2C lcd(0x27, 16, 2);     //Declare a lcd object which I2C address is 0x27, 16 columns, and 2 rows
DFRobotDFPlayerMini myDFPlayer;         //Declare DFPlayerMini object

void setup()
{
  // put your setup code here, to run once:

  //Initialize lcd and turn the backlight on
  lcd.init();
  lcd.backlight();

  //Initialize D10 and D11 pin as INPUT to get sensor value
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);

  //Initialize DFPlayer Mini using Software UART with baudrate 9600
  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial))
  {
    Serial.println("Failed to Start DFPlayer Mini");
  }
  myDFPlayer.setTimeOut(500); //Time out setup
}

void loop()
{
  // put your main code here, to run repeatedly:

  lcd.setCursor(0, 0); // At column 0, row 0 (which is column 1 and row 1 in real life)...
  lcd.print("Capacity"); // Print the word "Capacity: "
  lcd.setCursor(9, 0);
  lcd.print("CheckIn");
  lcd.setCursor(0, 1);
  lcd.print(String(int_sokhach) + String("/40  ")); //String() to force the data to turn into String type
  lcd.setCursor(9, 1);
  lcd.print(int_checkin);

  
  if (digitalRead(10) == 0) //If the sensor that connect to pin D10 is active (ACTIVE LOW <=> when is active, is 0)
  {
    while (!((digitalRead(11) == 0) && (digitalRead(10) == 1))) // Wait until a person go by the other sensor
    {
      delay(0);
    }
    // BYE
    myDFPlayer.playMp3Folder(2); //start playing the second file in order of which file you write to the micro SD card first
    if (int_sokhach > 0)
    {
      int_sokhach -= 1; //Decrease by 1 because someone have left
    }
    while (!((digitalRead(10) == 1) && (digitalRead(11) == 1))) //and wait until all sensors stop active (or standby)
    {
      delay(0);
    }
  }

  //The same as above, but D11 active first
  if (digitalRead(11) == 0)
  {
    while (!((digitalRead(11) == 1) && (digitalRead(10) == 0)))
    {
      delay(0);
    }
    myDFPlayer.playMp3Folder(1);
    int_checkin += 1;
    int_sokhach += 1;
    while (!((digitalRead(10) == 1) && (digitalRead(11) == 1)))
    {
      delay(0);
    }
  }

  delay(0);
}
