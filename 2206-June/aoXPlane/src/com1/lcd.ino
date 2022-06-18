// vim: set ft=ino
// 
// This source file is subject to the Apache License 2.0
// that is bundled with this package in the file LICENSE.txt.
// It is also available through the Internet at this address:
// https://opensource.org/licenses/Apache-2.0
// 
// @author	Sebastien LEGRAND
// @license	Apache License 2.0
// 
// @brief   LCD helper functions

//----- includes
#include <LiquidCrystal.h>


//----- globals

// LCD definition
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


//----- functions

// setup the LCD
void lcdSetup() {
    lcd.begin(16, 2);
    lcd.clear();
}

// print the current frequency
void lcdPrintFrequency(int num, int fraq) {
    lcd.clear();
    lcd.print("COM1 Frequency:");
    lcd.setCursor(0, 1);
    lcd.print(num);
    lcd.print(".");
    lcd.print(fraq);
    lcd.print(" MHz");
}

void lcdWelcome() {
    lcd.clear();
    lcd.print("Welcome to ");
    lcd.setCursor(0, 1);
    lcd.print("Arduino COM1");
    delay(2000);
}