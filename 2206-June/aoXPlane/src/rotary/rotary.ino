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
// @brief	Sketch to play with a Rotary Encoder Potentiometer

//----- globals

// Arduino PINs definition
const int SW_PIN = 2;
const int CLK_PIN = 3;
const int DT_PIN = 4;

// counter
int counter = 0;

// CLK states
int last_clk_state, curr_clk_state;

// button press timestamp
const int MAX_BTN_INTERVAL = 50;
unsigned long last_btn_ts = 0;

// FSM state
int fsm_state;

//----- functions
void setup() {
    // encoder PIN
    pinMode(CLK_PIN, INPUT);
    pinMode(DT_PIN, INPUT);

    // switch
    pinMode(SW_PIN, INPUT_PULLUP);

    // initial CLK state
    last_clk_state = digitalRead(CLK_PIN);

    // Serial monitor
    Serial.begin(9600);

    // fsm in initial state
    fsm_state = 0;
}


void loop() {

    // wait for the initial value
    if (fsm_state == 0) {
        Serial.println("Waiting for initial value");
        int done = 0;
        while (!done) {
          if (Serial.available() > 0) {
              String init_value = Serial.readString();
              counter = init_value.toInt();
              Serial.print("Initial value set to ");
              Serial.println(counter);
              done = 1;
          }
        }
        fsm_state = 1;
    }


    if (fsm_state == 1) {
        // read the current value of CLK
        curr_clk_state = digitalRead(CLK_PIN);

        // if current != last, then know has been turned.
        // only consider knob when CLK goes 0->1 to avoid double count effect
        if ((curr_clk_state != last_clk_state) && (curr_clk_state == 1)) {
            
            // we need to determine which way the knob was turned
            if (digitalRead(DT_PIN) != curr_clk_state) {
                // Counter Clock-Wise
                counter = counter - 1;
            } else {
                // Clock-Wise
                counter = counter + 1;
            }

            Serial.print("Counter: ");
            Serial.println(counter);
        }

        // keep track of last CLK value
        last_clk_state = curr_clk_state;

        // read the switch value
        if (digitalRead(SW_PIN) == LOW) {
            // only consider the button value if more than 50ms has passed
            if (millis() - last_btn_ts > MAX_BTN_INTERVAL) {
                Serial.println("Button Pressed!");
            }

            last_btn_ts = millis();
        }

        // small delay to have smooth reading
        delay(1);
    }
}
