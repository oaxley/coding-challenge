// -*- coding: utf-8 -*-
// vim: set ft=python
//
// This source file is subject to the Apache License 2.0
// that is bundled with this package in the file LICENSE.txt.
// It is also available through the Internet at this address:
// https://opensource.org/licenses/Apache-2.0
//
// @author	Sebastien LEGRAND
// @license	Apache License 2.0
//
// @brief	Main entry point for RPS Game

//----- includes
#include <LiquidCrystal.h>


//----- globals

// LCD pins description
const int rs = 7,   // Register Select
          en = 8,   // Enable
          d4 = 9,   // Data-0
          d5 = 10,  // Data-1
          d6 = 11,  // Data-2
          d7 = 12;  // Data-3

// initialize the LCD
LiquidCrystal lcd(rs, en, d4, d5, d6, d7)


//----- functions

// Arduino setup
void setup() {

}

// Arduino main loop
void loop() {

}