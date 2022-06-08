void setup() {
  // LED + LCD setups
  ledSetup();
  lcdSetup();

  // switch setup
  pinMode(2, INPUT);

  // initialize random generator
  randomSeed(analogRead(0));
}

void loop() {
  // print the invite to the user
  lcdPrintInvite();

  // wait for the button to be pressed
  int switchState = digitalRead(2);
  while (switchState == LOW) {
    delay(100);
    switchState = digitalRead(2);
  }

  // start the LED animation
  lcdPrintThinking();
  ledAnimation();

  // get a random number [0, 19]
  int index = random(0, 20);

  // print the Magic8 answer
  lcdPrintAnswer(index);

  // wait 10s
  delay(10000);
}

