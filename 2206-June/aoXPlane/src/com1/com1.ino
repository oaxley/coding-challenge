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
// @brief   Main entry point

//----- includes
#include "manager.h"

//----- globals
const int SERIAL_BAUD = 9600;

enum FSM_STATE {
    BEGIN,
    SETUP_ENCODER,
    RUNNING
};

FSM_STATE fsm = FSM_STATE::BEGIN;
Manager mgr;


//----- functions
void setup() {
    // Serial communication
    Serial.begin(SERIAL_BAUD);

    // setup the LCD Screen
    lcdSetup();

    // setup the encoder
    encoderSetup(&mgr);
}

void loop() {
    
    // wait for the value from the Serial port
    if (fsm == FSM_STATE::BEGIN) {
        lcdWelcome();
        Serial.println("Waiting for initial value from Simulator...");

        bool done = false;
        while(!done) {
            if (Serial.available() > 0) {
                // read the string from Serial port
                String init_value = Serial.readString();

                // cast value to int
                int value = init_value.toInt();
                Serial.print("Initial value set to ");
                Serial.println(value);

                // setup the manager value
                mgr.setValues(value);

                done = true;
            }
        }

        // switch to next step
        lcdPrintFrequency(mgr.number(), mgr.fraction());
        fsm = FSM_STATE::SETUP_ENCODER;
    }

    // setup the encoder ISR
    if (fsm == FSM_STATE::SETUP_ENCODER) {
        Serial.println("Attaching ISR to PIN 2,3");
        attachISR();
        fsm = FSM_STATE::RUNNING;
    }

    if (fsm == FSM_STATE::RUNNING) {
        // update LCD only when changes occur
        if (mgr.hasChanged())
            lcdPrintFrequency(mgr.number(), mgr.fraction());
        
        readSwitch();
    }

    // small delay for smooth reading
    delay(1);
}
