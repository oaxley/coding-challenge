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

// game choices
const char* choices[] = { "Rock", "Paper", "Scissors" };
const int ROCK = 5, PAPER = 4, SCISSORS = 3;

// winning matrix
// first dimension is the user game, second is the computer game
// 0=draw game, +1=user wins, -1=computer wins
const int win_matrix[3][3] = {
    { 0, -1,  1},
    { 1,  0, -1},
    {-1.  1,  0}
};

// keep track of the scores
int usr_wins = 0, com_wins = 0;


// initialize the LCD
LiquidCrystal lcd(rs, en, d4, d5, d6, d7)


//----- functions

// Arduino setup
void setup() {

}

// Arduino main loop
void loop() {

}