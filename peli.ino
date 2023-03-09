#include <LiquidCrystal.h>


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int buttonPinLeft = A3;
const int buttonPinRight = A2;
const int rs = 8, en = 7, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


int satunnainenNumero = 0; //random number for game
int juoksevaNumero = 19; //number to get files moving
int score = 0;
int hahmoPaikkaY = 0;
int hahmoPaikkaX = 0;

// variables will change:
int buttonStateLeft = HIGH;         // variable for reading the pushbutton status
int buttonStateRight = HIGH;
int lastButtonStateLeft = 0;
int lastButtonStateRight = 0;
void setup() {
  pinMode(A3, INPUT);
  pinMode(A2, INPUT);
  // initialize the Serial Monitor @ 9600
  Serial.begin(9600);  
  // initialize the keypad pin(s) as an input:
  lcd.begin(20,4);
}

void loop(){
 
  
  lcd.setCursor(juoksevaNumero, satunnainenNumero);
  lcd.print("** ");
  juoksevaNumero--;
  delay(50);
  
  
  
  if(juoksevaNumero < 0){
      juoksevaNumero = 19;
      satunnainenNumero = random(4);
      lcd.clear();
      score++;

    }
  if(hahmoPaikkaY > 3){
    hahmoPaikkaY = 0;
    }
  if(hahmoPaikkaY < 0){
    hahmoPaikkaY = 3;
    }
    lcd.setCursor(hahmoPaikkaX, hahmoPaikkaY);
    
    lcd.print(":D");


    lcd.setCursor(juoksevaNumero, satunnainenNumero);

    
    if(juoksevaNumero == hahmoPaikkaX){
      if(satunnainenNumero == hahmoPaikkaY){
        lcd.setCursor(hahmoPaikkaX, hahmoPaikkaY);
        lcd.print(";(");
        lcd.setCursor(10,2);
        lcd.print("GAME OVER!");
        lcd.setCursor(10,1);
        lcd.print("score ");
        lcd.setCursor(16,1);
        lcd.print(score);
        while(1);
        
        }
      }



  
   buttonStateLeft = digitalRead(buttonPinLeft);
   buttonStateRight = digitalRead(buttonPinRight);
   //Serial.print(digitalRead(A7));
    
      if (buttonStateLeft != lastButtonStateLeft){
        if (buttonStateLeft == HIGH) {    
        Serial.print('1');
        lcd.clear();
        delay(20);
        hahmoPaikkaY--;


       }}

  
    if(buttonStateRight != lastButtonStateRight){
       if (buttonStateRight == HIGH) {
          Serial.print('3');
          lcd.clear();
          delay(20);
          hahmoPaikkaY++;
 
        }}
        

      lastButtonStateLeft = buttonStateLeft;
      lastButtonStateRight = buttonStateRight;


     }

