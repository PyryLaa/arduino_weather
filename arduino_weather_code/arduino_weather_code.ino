#include <LiquidCrystal.h>                                        //including all the libraries needed
#include <TimerOne.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <SPI.h>
#define inTopic    "ICT1B_in_2020"                                // * MQTT channel where data are received 
#define outTopic   "ICT4_out_2020" 
#define  mac_6    0x69 



const short int speed_pin = 2; //Pin for wind speed signal


const short int rs = 8, en = 7, d4 = 6, d5 = 5, d6 = 4, d7 = 3;         //LCD display pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);                        //Define the lcd display as "lcd" and the pins
const short int buttonPins[4] = {A0, A1, A2, A3};                       //Button pins for keypad inputs


bool tm = false;
short int buttonState = 0;                                              //Button state of keypad buttons


//These are for debouncing grounding button for analog input
int gnd_btn = A5;
int gnd_state = 0;
int gnd_output = 9;
unsigned long lastDebounceTime = 0;
short debounceDelay = 50;


volatile int step_counter = 0;                                    //Counter for rising edges of wind speed signal
volatile int freq = 0;                                            //Frequency of wind speed signal

int wind_speed = 0;
int sensor_value = 0;
int wind_deg = 0;

bool dir_state = false;
bool speed_state = false;




EthernetClient ethClient;

char* deviceId = "2023PJS420";
char* clientId = "8481024";
char* deviceSecret = "tamk";  



byte server[] = {10, 6, 0, 21}; //Server IP address
void callback(char* topic, byte* payload, unsigned int length);
PubSubClient client (server, 1883, callback, ethClient);



static uint8_t mymac[6] = { 0x44,0x76,0x58,0x10,0x00,mac_6 };      //Mac address for ethernet module

void testmode();
void Timer_int_routine();
void pulse_counter();
void Connect_MQTT_server();
void send_MQTT_message();

void setup() {
  pinMode(9, OUTPUT);
  pinMode(A5, INPUT);
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

  fetch_IP();
  
  
  
  Connect_MQTT_server();
  
}

void loop() {
  for (int i = 0; i < 4; i++){
    buttonState = digitalRead(buttonPins[i]);
    

    if (buttonState == LOW && buttonPins[i] == A0){ //Button 1
      
      tm = true;
    }
    if (buttonState == LOW && buttonPins[i] == A3){ //Button A
      peli();
    }

    if (tm == true){
      testmode();
    }
    if (buttonState == LOW && buttonPins[i] == A1){ //Button 2
      if (dir_state){
        dir_state = false;        
      }
      else{
      dir_state = true;
      }      

    }
    if (buttonState == LOW && buttonPins[i] == A2){ //Button 3
      if (speed_state){
        speed_state = false;        
      }
      else{      
      speed_state = true;
      }
    }
 }

  
  if (dir_state){ //Start measuring wind direction
    wind_dir();

  }
  else if(dir_state == false){
    lcd.setCursor(3, 3);
    lcd.print("DIR:OFF");
    lcd.setCursor(11, 1);
    lcd.print("  ");    
  }
      
  if (speed_state){ //Start measuring wind speed 
    lcd.setCursor(12, 0);
    lcd.print(wind_speed);
    lcd.print(" m/s ");
    lcd.setCursor(12, 3);
    lcd.print("SPD:ON ");
  }
  else if (speed_state == false){
    lcd.setCursor(12, 3);
    lcd.print("SPD:OFF");
    lcd.setCursor(12, 0);
    lcd.print("        ");
  }
  if(speed_state == true || dir_state == true){  
    send_MQTT_message();
  }

}

void wind_dir(){//Wind direction calculated here
  lcd.setCursor(3, 3);
  lcd.print("DIR:ON ");
  sensor_value = analogRead(A4);
  float wind_direction = sensor_value*(5/1023.0);
  
  
  if(wind_direction >= 0 && wind_direction < 0.47){
    
    lcd.setCursor(11,1);
    wind_deg = 360;
    lcd.print("N ");
    delay(500);
  }
  else if(wind_direction > 0.47 && wind_direction < 0.95){
    
    lcd.setCursor(11,1);
    wind_deg = 45;
    lcd.print("NE");
    delay(500);
  }
  else if(wind_direction > 0.95 && wind_direction < 0.95){
    
    lcd.setCursor(11,1);
    wind_deg = 90;
    lcd.print("E ");
    delay(500);
  }
  else if(wind_direction > 1.43 && wind_direction < 1.9){
    
    lcd.setCursor(11,1);
    wind_deg = 135;
    lcd.print("SE");
    delay(500);
  }
  else if(wind_direction > 1.9 && wind_direction < 2.38){
    
    lcd.setCursor(11,1);
    wind_deg = 180;
    lcd.print("S ");
    delay(500);
  }
  else if(wind_direction > 2.38 && wind_direction < 2.85){
    
    lcd.setCursor(11,1);
    wind_deg = 225;
    lcd.print("SW");
    delay(500);
  }
  else if(wind_direction > 2.85 && wind_direction < 3.33){
    
    lcd.setCursor(11,1);
    wind_deg = 270;
    lcd.print("W ");
    delay(500);
  }
  else if(wind_direction > 3.33){
    
    lcd.setCursor(11,1);
    wind_deg = 315;
    lcd.print("NW");
    delay(500);
  }


}


void testmode(){

  
  while(true){
    wind_speed = -0.24 + freq * 0.699;
    lcd.setCursor(0, 3);
    lcd.print("TM");
    
    lcd.setCursor(12, 0);
    lcd.print(wind_speed);
    lcd.print(" m/s ");
    

    wind_dir();
    
    
    delay(1000);
    
  }

}


void pulse_counter(){ //Interrupt routine, increments the step counter which counts the rising edges of wind speed signal
//Runs when there is input to digital pin 2
  step_counter++;
}
void Timer_int_routine(){ //This happens every 3 seconds, counts the frequency of wind speed signal
  freq = step_counter/3;
  wind_speed = -0.24 + freq * 0.699;
  step_counter = 0;
}



void fetch_IP(){ //Get ip address for ethernet module
  byte result = 1;

  result = Ethernet.begin( mymac);
  if (result == 0){
    lcd.setCursor(0,2);
    lcd.print("IP fail");
  }
  else{
    lcd.setCursor(0,2);
    lcd.print(Ethernet.localIP());
  }
}

void callback(char* topic, byte* payload, unsigned int length){
  char* received_topic = topic;
  byte received_payload = payload;
  unsigned int received_length = length;
}

void Connect_MQTT_server(){
  if (!client.connected()){                                   // check if allready connected  
      if (client.connect(clientId, deviceId, deviceSecret)){ // connection to MQTT server 
        
        client.subscribe(inTopic);                        // subscript to in topic        
      }
  }
}



void send_MQTT_message(){                     // Send MQTT message
  
  char spd_bufa[50];
  char dir_bufa[50];
  if (client.connected() && speed_state == true){ 
    sprintf(spd_bufa,"IOTJS={\"S_name\":\"Alykkaat wind speed\",\"S_value\":%d}",wind_speed);// create message with header and data
    
    client.publish(outTopic,spd_bufa);// send message to MQTT server        
  }
  if (client.connected() && dir_state == true){
    sprintf(dir_bufa,"IOTJS={\"S_name\":\"Alykkaat wind degree \",\"S_value\":%d}",wind_deg);
     
    client.publish(outTopic,dir_bufa);
  }
 
  
  else{//Re connect if connection is lost
    delay(500);
    
    client.connect(clientId, deviceId, deviceSecret);
    delay(1000);// wait for reconnecting
  }

  delay(1000);
  
}




void running_alphabet(){//Surprise
  char alphabet = 65;
  const int MAX_ASCII = 122;
  int a = 0, b = 0;
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

void peli(){

  lcd.clear();

  int satunnainenNumero = 0; //random number for game
  int juoksevaNumero = 19; //number to get files moving
  int score = 0;
  int hahmoPaikkaY = 0;
  int hahmoPaikkaX = 0;
  int aloitus = 0;
  int buttonStateLeft = HIGH;        
  int buttonStateRight = HIGH;
  int lastButtonStateLeft = 0;
  int lastButtonStateRight = 0;

  while(true){
  
  if(aloitus == 0){
    lcd.setCursor(2,2);
    lcd.print("Starting: 3");
    delay(1000);
    
    lcd.print("2");
    delay(1000);
    
    lcd.print("1 Go!");
    delay(1000);
    lcd.clear();

    aloitus = 1;
    }
 
  
  lcd.setCursor(juoksevaNumero, satunnainenNumero);
  lcd.print("** ");
  juoksevaNumero--;
  delay(20);  //pelin nopeus pienempi luku = nopeampi peli!!!
  
  
  
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
        lcd.print("score: ");
        lcd.setCursor(17,1);
        lcd.print(score);
        while(1){
          for (int i = 0; i < 4; i++){
    buttonState = digitalRead(buttonPins[i]);
           if (buttonState == LOW && buttonPins[i] == A3){
        running_alphabet();
    }
          }
        
        }}}
      


   buttonStateLeft = digitalRead(gnd_btn);

    
      if (buttonStateLeft != lastButtonStateLeft){
        if (buttonStateLeft == HIGH) {    
        
        lcd.clear();
        delay(20);
        hahmoPaikkaY--;


     }}
  
   lastButtonStateLeft = buttonStateLeft;

  
  }
} 
  
