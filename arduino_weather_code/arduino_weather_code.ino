#include <LiquidCrystal.h>
#include <TimerOne.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <SPI.h>
#define outTopic "alykkaat2k23 Topic"
#define  mac_6    0x73 



const int speed_pin = 2; //Pin for wind speed signal


const int rs = 8, en = 7, d4 = 6, d5 = 5, d6 = 4, d7 = 3; //LCD display pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);//Define the lcd display as "lcd" and the pins
const int buttonPins[4] = {A0, A1, A2, A3};//Button pins for keypad inputs


bool tm = false;
int buttonState = 0; //Button state of keypad buttons


volatile int step_counter = 0; //Counter for rising edges of wind speed signal
volatile int freq = 0; //Frequency of wind speed signal

int wind_speed = 0;
int sensor_value = 0;

char alphabet = 65;
const int MAX_ASCII = 122;
int a = 0, b = 0;

void testmode();
void Timer_int_routine();
void pulse_counter();

EthernetClient ethClient;

char bufa[15] = "Alykkaat Data";
byte server[] = {10, 6, 0, 20};
//void callback(char* topic, byte* payload, unsigned int length);
//PubSubClient client (server, 1883, callback, ethClient);

//const byte my_ip[] = {192, 168, 112, 89};

static uint8_t mymac[6] = { 0x44,0x76,0x58,0x10,0x00,mac_6 };

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
  lcd.setCursor(0, 0);
  lcd.print("Wind speed: ");
  lcd.setCursor(0, 1);
  lcd.print("Direction: ");
  Ethernet.begin( mymac);
  

  char* deviceId = "2023PJS420";
  char* clientId = "8481024";
  char* deviceSecret = "tamk";  
  
  //client.connect(clientId, deviceId, deviceSecret);
  
}

void loop() {
  for (int i = 0; i < 4; i++){
    buttonState = digitalRead(buttonPins[i]);
    

    if (buttonState == LOW && buttonPins[i] == A0){
      
      tm = true;
    }
    if (buttonState == LOW && buttonPins[i] == A3){
      running_alphabet();
    }

    if (tm == true){
      testmode();
    }
  }
  sensor_value = analogRead(A4);
  float wind_direction = sensor_value*(5/1023.0);
  lcd.setCursor(0,2);
  lcd.print(Ethernet.localIP());
  
  
  lcd.setCursor(12,0);
  lcd.print(wind_speed);
  lcd.print(" m/s ");
      
  if(wind_direction >= 0 && wind_direction < 0.47){
    
    lcd.setCursor(11,1);
    /*lcd.print(wind_direction);
    lcd.setCursor(7,1);*/
    lcd.print("N ");
    delay(500);
  }
  else if(wind_direction > 0.47 && wind_direction < 0.95){
    
    lcd.setCursor(11,1);
    /*lcd.print(wind_direction);
    lcd.setCursor(7,1);*/
    lcd.print("NE");
    delay(500);
  }
  else if(wind_direction > 0.95 && wind_direction < 0.95){
    
    lcd.setCursor(11,1);
    /*lcd.print(wind_direction);
    lcd.setCursor(7,1);*/
    lcd.print("E ");
    delay(500);
  }
  else if(wind_direction > 1.43 && wind_direction < 1.9){
    
    lcd.setCursor(11,1);
    /*lcd.print(wind_direction);
    lcd.setCursor(7,1);*/
    lcd.print("SE");
    delay(500);
  }
  else if(wind_direction > 1.9 && wind_direction < 2.38){
    
    lcd.setCursor(11,1);
    /*lcd.print(wind_direction);
    lcd.setCursor(7,1);*/
    lcd.print("S ");
    delay(500);
  }
  else if(wind_direction > 2.38 && wind_direction < 2.85){
    
    lcd.setCursor(11,1);
    /*lcd.print(wind_direction);
    lcd.setCursor(7,1);*/
    lcd.print("SW");
    delay(500);
  }
  else if(wind_direction > 2.85 && wind_direction < 3.33){
    
    lcd.setCursor(11,1);
    /*lcd.print(wind_direction);
    lcd.setCursor(7,1);*/
    lcd.print("W ");
    delay(500);
  }
  else if(wind_direction > 3.33){
    
    lcd.setCursor(11,1);
    /*lcd.print(wind_direction);
    lcd.setCursor(7,1);*/
    lcd.print("NW");
    delay(500);
  }

  //client.publish(outTopic, bufa);

}


void testmode(){

  
  while(true){
    wind_speed = -0.24 + freq * 0.699;
    lcd.setCursor(18, 3);
    lcd.print("TM");
    lcd.setCursor(0, 0);
    lcd.print("Wind speed: ");
    lcd.print(wind_speed);
    lcd.print(" m/s ");
    lcd.setCursor(0,1);
    lcd.print("Direction: ");

    sensor_value = analogRead(A4);
    float wind_direction = sensor_value*(5/1023.0);

    if(wind_direction >= 0 && wind_direction < 0.47){
    
    lcd.setCursor(11,1);
    /*lcd.print(wind_direction);
    lcd.setCursor(7,1);*/
    lcd.print("N ");
    delay(500);
  }
  else if(wind_direction > 0.47 && wind_direction < 0.95){
    
    lcd.setCursor(11,1);
    /*lcd.print(wind_direction);
    lcd.setCursor(7,1);*/
    lcd.print("NE");
    delay(500);
  }
  else if(wind_direction > 0.95 && wind_direction < 0.95){
    
    lcd.setCursor(11,1);
    /*lcd.print(wind_direction);
    lcd.setCursor(7,1);*/
    lcd.print("E ");
    delay(500);
  }
  else if(wind_direction > 1.43 && wind_direction < 1.9){
    
    lcd.setCursor(11,1);
    /*lcd.print(wind_direction);
    lcd.setCursor(7,1);*/
    lcd.print("SE");
    delay(500);
  }
  else if(wind_direction > 1.9 && wind_direction < 2.38){
    
    lcd.setCursor(11,1);
    /*lcd.print(wind_direction);
    lcd.setCursor(7,1);*/
    lcd.print("S ");
    delay(500);
  }
  else if(wind_direction > 2.38 && wind_direction < 2.85){
    
    lcd.setCursor(11,1);
    /*lcd.print(wind_direction);
    lcd.setCursor(7,1);*/
    lcd.print("SW");
    delay(500);
  }
  else if(wind_direction > 2.85 && wind_direction < 3.33){
    
    lcd.setCursor(11,1);
    /*lcd.print(wind_direction);
    lcd.setCursor(7,1);*/
    lcd.print("W ");
    delay(500);
  }
  else if(wind_direction > 3.33){
    
    lcd.setCursor(11,1);
    /*lcd.print(wind_direction);
    lcd.setCursor(7,1);*/
    lcd.print("NW");
    delay(500);
  }
    
    
    delay(1000);
    lcd.clear();
  }

}


void pulse_counter(){ //Interrupt routine, increments the step counter which counts the rising edges of wind speed signal
  step_counter++;
}
void Timer_int_routine(){ //This happens every 3 seconds, counts the frequency of wind speed signal
  freq = step_counter/3;
  wind_speed = -0.24 + freq * 0.699;
  step_counter = 0;
}



void running_alphabet(){
  while (true){
  while (a < 20){
    if (alphabet == 123){
      alphabet = 65;
    }
    if (alphabet == 91){
      alphabet = 97;
    }
    lcd.clear();
    lcd.setCursor(a, b);
    lcd.print(alphabet);
    alphabet++;
    a++;
    delay(200);
  }
  b++;
  while (a > 0){
    if (alphabet == 123){
      alphabet = 65;
    }
    if (alphabet == 91){
      alphabet = 97;
    }
    a--;
    lcd.clear();
    lcd.setCursor(a, b);
    lcd.print(alphabet);
    alphabet++;
    
    delay(200);
  }
  b++;
  while (a < 20){
    if (alphabet == 123){
      alphabet = 65;
    }
    if (alphabet == 91){
      alphabet = 97;
    }
    lcd.clear();
    lcd.setCursor(a, b);
    lcd.print(alphabet);
    alphabet++;
    a++;
    delay(200);
  }
  b++;
  while (a > 0){
    if (alphabet == 123){
      alphabet = 65;
    }
    if (alphabet == 91){
      alphabet = 97;
    }
    a--;
    lcd.clear();
    lcd.setCursor(a, b);
    lcd.print(alphabet);
    alphabet++;
    
    delay(200);
  }
  b = 0;
}
}

void callback(char* topic, byte* payload, unsigned int length){
  char* received_topic = topic;
  byte received_payload = payload;
  unsigned int received_length = length;
}

