#include <LiquidCrystal.h>
#include <TimerOne.h>

const int speed_pin = 2; //Pin for wind speed signal
/*attachInterrupt(0, pulse_counter, RISING);
Timer1.intialize(5000000);
Timer1.attachInterrupt(Timer_int_routine);*/

const int rs = 8, en = 7, d4 = 6, d5 = 5, d6 = 4, d7 = 3; //LCD display pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);//Define the lcd display as "lcd" and the pins
const int buttonPins[4] = {A0, A1, A2, A3};//Button pins for keypad inputs

bool tm = false;
int buttonState = 0; //Button state of keypad buttons


volatile int step_counter = 0; //Counter for rising edges of wind speed signal
volatile int freq = 0; //Frequency of wind speed signal

int wind_speed = 0;
int sensor_value = 0;

void testmode();
void Timer_int_routine();
void pulse_counter();


void setup() {
  attachInterrupt(digitalPinToInterrupt(2), pulse_counter, RISING);
  Timer1.initialize(3000000);
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
    if (buttonState == LOW && buttonPins[i] == A1){
      wind_speed_func();      
    }

    if (tm == true){
      testmode();
    }
  }
  sensor_value = analogRead(A4);
  float wind_direction = sensor_value*(5/1023.0);
  
      
  if(wind_direction >= 0 && wind_direction < 0.47){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(wind_direction);
    lcd.setCursor(7,1);
    lcd.print("N ");
    Serial.print("N ");
    delay(500);
  }
  else if(wind_direction > 0.47 && wind_direction < 0.95){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(wind_direction);
    lcd.setCursor(7,1);
    lcd.print("NE");
    Serial.print("NE ");
    delay(500);
  }
  else if(wind_direction > 0.95 && wind_direction < 0.95){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(wind_direction);
    lcd.setCursor(7,1);
    lcd.print("E ");
    Serial.print("E ");
    delay(500);
  }
  else if(wind_direction > 1.43 && wind_direction < 1.9){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(wind_direction);
    lcd.setCursor(7,1);
    lcd.print("SE");
    Serial.print("SE ");
    delay(500);
  }
  else if(wind_direction > 1.9 && wind_direction < 2.38){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(wind_direction);
    lcd.setCursor(7,1);
    lcd.print("S ");
    Serial.print("S ");
    delay(500);
  }
  else if(wind_direction > 2.38 && wind_direction < 2.85){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(wind_direction);
    lcd.setCursor(7,1);
    lcd.print("SW");
    Serial.print("SW ");
    delay(500);
  }else if(wind_direction > 2.85 && wind_direction < 3.33){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(wind_direction);
    lcd.setCursor(7,1);
    lcd.print("W ");
    Serial.print("W ");
    delay(500);
  }
  else if(wind_direction > 3.33){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(wind_direction);
    lcd.setCursor(7,1);
    lcd.print("NW");
    Serial.print("NW ");
    delay(500);
  }

}


void testmode(){

  
  while(true){
    wind_speed = -0.24 + freq * 0.699;
    lcd.setCursor(18, 3);
    lcd.print("TM");
    lcd.setCursor(0, 0);
    lcd.print("nopeus:");
    lcd.print(wind_speed);
    lcd.print("m/s");
    lcd.setCursor(0, 1);
    lcd.print("suunta: ");
    
    delay(1000);
    lcd.clear();
  }
}
void wind_speed_func(){
  while(true){

    wind_speed = -0.24 + freq * 0.699;
    lcd.setCursor(3, 0);
    lcd.print("Tuulen nopeus");
    lcd.setCursor(8,1);
    lcd.print(wind_speed);
    lcd.print("m/s");
    delay(1000);
    lcd.clear();
  }
}
void pulse_counter(){ //Interrupt routine, increments the step counter which counts the rising edges of wind speed signal
  step_counter++;
}
void Timer_int_routine(){ //This happens every 5 seconds, counts the frequency of wind speed signal
  freq = step_counter/3;
  step_counter = 0;
}
