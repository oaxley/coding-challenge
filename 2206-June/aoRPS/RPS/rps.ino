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
const char* choices[] = { "Scissors", "Paper", "Rock" };
const int ROCK = 5, PAPER = 4, SCISSORS = 3;

// winning matrix
// first dimension is the user game, second is the computer game
// 0=draw game, +1=user wins, -1=computer wins
const int win_matrix[3][3] = {
    { 0, +1, -1},
    {-1,  0, +1},
    {+1, -1,  0}
};

// keep track of the scores
int usr_wins = 0, com_wins = 0;


// initialize the LCD
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


//----- functions

// read the user input from the push buttons
int readUserInput() {

    lcd.clear();
    lcd.print("Select your move");

    int done = -1;
    while (done < 0) {

        // read all the buttons
        for (int i = 3; i < 6; i++) {
            if (digitalRead(i) != LOW) {
                done = i - 3;
                break;
            }
        }

        // wait before doing it again
        delay(100);
    }

    lcd.setCursor(0, 1);
    lcd.print(choices[done]);
    delay(2000);

    return done;
}

// computer input
int readComInput() {

    lcd.clear();
    lcd.print("My move");
    delay(1000);

    // select a random number between 0 - 100 to increase randomness
    int value = random(0, 100) % 3;

    lcd.setCursor(0, 1);
    lcd.print(choices[value]);
    delay(2000);

    return value;
}

// check the winner
void checkWinner(int usr_move, int com_move) {

    lcd.clear();

    int value = win_matrix[usr_move][com_move];
    switch(value) {
        case -1:
            lcd.print("I Win!");
            com_wins++;
            break;
        case 0 :
            lcd.print("Draw Game!");
            break;
        case 1 :
            lcd.print("You Win!");
            usr_wins++;
            break;
    }

    delay(2000);
}

// print scores
void printScores() {
    lcd.clear();
    lcd.print("-- SCORES --");

    lcd.setCursor(0, 1);
    lcd.print("You:");
    lcd.print(usr_wins);

    lcd.print("   Me:");
    lcd.print(com_wins);

    delay(2000);
}

// Arduino setup
void setup() {
    // LCD Setup
    lcd.begin(16, 2);

    // push buttons
    pinMode(ROCK, INPUT);
    pinMode(PAPER, INPUT);
    pinMode(SCISSORS, INPUT);

    // random generator
    randomSeed(analogRead(0));
}

// Arduino main loop
void loop() {
    // wait for user input and print it
    int usr_move = readUserInput();

    // get computer move
    int com_move = readComInput();

    // check the winner
    checkWinner(usr_move, com_move);

    // print scores
    printScores();
}