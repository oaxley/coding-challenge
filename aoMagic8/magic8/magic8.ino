int switchState;
int previousState;
int trigger;

void setup() {
  // setup the LEDS
  ledSetup();

  // setup the switch
  pinMode(2, INPUT);

  // read the switch
  switchState = digitalRead(2);
  previousState = switchState;
  trigger = 0;

}

void loop() {
  // read the switch state
  switchState = digitalRead(2);

  if (switchState != previousState) {
    trigger = (trigger + 1) & 1;
  }

  if (trigger) {
    ledAnimation();    
  }

  previousState = switchState;
}
