#include <LiquidCrystal.h>
#include <TimerOne.h>

const int speed_pin = 2;
/*attachInterrupt(0, pulse_counter, RISING);
Timer1.intialize(5000000);
Timer1.attachInterrupt(Timer_int_routine);*/

const int rs = 8, en = 7, d4 = 6, d5 = 5, d6 = 4, d7 = 3; //LCD display pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);//Define the lcd display as "lcd" and the pins
const int buttonPins[4] = {A0, A1, A2, A3};//Button pins for keypad inputs

bool tm = false;
int buttonState = 0;


volatile int step_counter = 0;
volatile int freq = 0;

int wind_speed = 0;

void testmode();
void Timer_int_routine();
void pulse_counter();

void setup() {
  attachInterrupt(0, pulse_counter, RISING);
  Timer1.initialize(5000000);
  Timer1.attachInterrupt(Timer_int_routine);
  lcd.begin(20,4);
  pinMode(speed_pin, INPUT);
  Serial.begin(9600);
  pinMode(A4, INPUT);

  for (int i = 0; i < 4; i++){
    pinMode(buttonPins[i], INPUT_PULLUP);
  };
  

}

void loop() {
  for (int i = 0; i < 4; i++){
    buttonState = digitalRead(buttonPins[i]);
    

    if (buttonState == LOW && buttonPins[i] == A0){
      
      tm = true;
    }
    if (tm == true){
      testmode();
    }
  }
  float wind_direction = (analogRead(A4) / 1024)*5;
  lcd.print(analogRead(A4));
      
  if(wind_direction >= 0 && wind_direction < 0.47){
    lcd.setCursor(7,1);
    lcd.print("N ");
    Serial.print("N ");
    lcd.clear();
  }
  else if(wind_direction > 0.47 && wind_direction < 0.95){
    lcd.clear();
    lcd.setCursor(7,1);
    lcd.print("NE");
    Serial.print("NE ");
  }
  else if(wind_direction > 0.95 && wind_direction < 0.95){
    lcd.clear();
    lcd.setCursor(7,1);
    lcd.print("E ");
    Serial.print("E ");
  }
  else if(wind_direction > 1.43 && wind_direction < 1.9){
    lcd.clear();
    lcd.setCursor(7,1);
    lcd.print("SE");
    Serial.print("SE ");
  }
  else if(wind_direction > 1.9 && wind_direction < 2.38){
    lcd.clear();
    lcd.setCursor(7,1);
    lcd.print("S ");
    Serial.print("S ");
  }
  else if(wind_direction > 2.38 && wind_direction < 2.85){
    lcd.clear();
    lcd.setCursor(7,1);
    lcd.print("SW");
    Serial.print("SW ");
  }else if(wind_direction > 2.85 && wind_direction < 3.33){
    lcd.clear();
    lcd.setCursor(7,1);
    lcd.print("W ");
    Serial.print("W ");
  }
  else if(wind_direction > 3.33){
    lcd.clear();
    lcd.setCursor(7,1);
    lcd.print("NW");
    Serial.print("NW ");
  }

}


void testmode(){

  
  
  wind_speed = -0.24 + freq * 0.699;
  lcd.setCursor(18, 3);
  lcd.print("TM");
  lcd.setCursor(0, 0);
  lcd.print("nopeus:");
  lcd.setCursor(7, 0);
  lcd.print(wind_speed);
  lcd.setCursor(0, 1);
  lcd.print("suunta: ");
  
  delay(1000);
  lcd.clear();
}

void pulse_counter(){
  step_counter++;
}
void Timer_int_routine(){
  freq = step_counter/5;
  step_counter = 0;
}
