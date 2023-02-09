#include <LiquidCrystal.h>
const int buttonPins[4] = {A0, A1, A2, A3};     // the number of the pushbutton pins
      // the number of the LED pin#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 8, en = 7, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the Serial Monitor @ 9600
  Serial.begin(9600);  
  // initialize the keypad pin(s) as an input:
  for(int x=0; x<4; x++)
  {
    pinMode(buttonPins[x], INPUT_PULLUP); 
  }  
  lcd.begin(20,4);
}

void loop(){
  // read the state of the keypad value:
  for(int x=0; x<4; x++)
  {
    //signifying the state of which the button is in by reading the appropriate pin #
    buttonState = digitalRead(buttonPins[x]);

    // check if the pushbutton on the keypad is pressed.
    // if it is, the buttonState is LOW:
    if (buttonState == LOW && buttonPins[x] == A0) {    
      Serial.print('1');
      lcd.print('1'); 
    }
    else if (buttonState == LOW && buttonPins[x] == A1) {
      Serial.print('2');
      lcd.print('2'); 
    }
    else if (buttonState == LOW && buttonPins[x] == A2) {
      Serial.print('3');
      lcd.print('3'); 
    }
    else if (buttonState == LOW && buttonPins[x] == A3) {
      Serial.print('A');
      lcd.print('A'); 
    }
  }
}

