#include <LiquidCrystal.h>

// Functions to manipulate the LCD
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Magic8 responses
const int MAX_RESPONSES = 20;
const char* responses[] = {
  "It is certain.",
  "",
  "It is decidedly",
  "so.",
  "Without a doubt.",
  "",
  "Yes, definitely.",
  "",
  "You may rely on",
  "it.",
  "As I see it,",
  "yes.",
  "Most likely.",
  "",
  "Outlook good.",
  "",
  "Yes.",
  "",
  "Signs point to",
  "yes.",
  "Reply hazy, try",
  "again.",
  "Ask again later.",
  "",
  "Better not tell",
  "you now.",
  "Cannot predict",
  "now.",
  "Concentrate and",
  "ask again.",
  "Don't count on",
  "it.",
  "My reply is no.",
  "",
  "My sources say",
  "no.",
  "Outlook not so",
  "good.",
  "Very doubtful.",
  ""
};

void lcdSetup() {
  lcd.begin(16, 2);
}

void lcdPrintAnswer(int i) {
  int index = i << 1;
  lcd.clear();
  lcd.print(responses[index]);
  lcd.setCursor(0, 1);
  lcd.print(responses[index+1]);
}

void lcdPrintInvite() {
  lcd.clear();
  lcd.print("Make a wish and");
  lcd.setCursor(0, 1);
  lcd.print("push the button.");
}

void lcdPrintThinking() {
  lcd.clear();
  lcd.print("Thinking ...");
}