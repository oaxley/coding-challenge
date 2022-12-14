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
// @brief   Rotary Encoder helper functions

#include "manager.h"

//----- globals

// Arduino PINs definition
const int SW_PIN = 4;
const int CLK_PIN = 2;
const int DT_PIN = 3;

// button variables
const int MAX_BTN_INTERVAL = 50;
unsigned long last_btn_ts = 0;
int switch_state = 0;

// CLK states
int last_clk_state, curr_clk_state;

// manager
Manager *pMgr_ = NULL;


//----- functions
void encoderSetup(Manager* pMgr) {
    // encoder PIN
    pinMode(CLK_PIN, INPUT);
    pinMode(DT_PIN, INPUT);

    // switch
    pinMode(SW_PIN, INPUT_PULLUP);

    // initial CLK state
    last_clk_state = digitalRead(CLK_PIN);

    // setup the manager
    pMgr_ = pMgr;
}

// attach interrupt handler
void attachISR() {
    attachInterrupt(digitalPinToInterrupt(CLK_PIN), isrHandler, CHANGE);
    attachInterrupt(digitalPinToInterrupt(DT_PIN), isrHandler, CHANGE);
}

// interrupt service routine
void isrHandler() {
    // read the current value of CLK
    curr_clk_state = digitalRead(CLK_PIN);

    // if current != last, then know has been turned.
    // only consider knob when CLK goes 0->1 to avoid double count effect
    if ((curr_clk_state != last_clk_state) && (curr_clk_state == 1)) {
        
        // we need to determine which way the knob was turned
        if (digitalRead(DT_PIN) != curr_clk_state) {
            // Counter Clock-Wise
            if (switch_state == 0)
                pMgr_->decNumber();
            else
                pMgr_->decFraction();
        } else {
            // Clock-Wise
            if (switch_state == 0)
                pMgr_->incNumber();
            else
                pMgr_->incFraction();
        }
    }

    // keep track of last CLK value
    last_clk_state = curr_clk_state;
}

// read the switch value
void readSwitch() {

    if (digitalRead(SW_PIN) == LOW) {
        // only consider the button value if more than 50ms has passed
        if (millis() - last_btn_ts > MAX_BTN_INTERVAL) {
            // invert the switch state
            switch_state = (switch_state + 1) & 1;
        }

        last_btn_ts = millis();
    }

}