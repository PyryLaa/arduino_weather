#include<LiquidCrystal.h>

const int buttonPin = A0;  
int buttonState;            
int lastButtonState = LOW;  
int counter = 0;
const int rs = 8, en = 7, d4=6,d5=5,d6=4,d7=3;
unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 50;  
  
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  lcd.begin(20,4);

}

void loop() {
  int reading = digitalRead(buttonPin);
  lcd.setCursor(0,0);
  lcd.print(counter);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
          counter++;
          
      }
    }
  }

  lastButtonState = reading;
}
