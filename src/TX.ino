#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define CH_NUM 0x60   
#define SIG_POWER RF24_PA_HIGH
#define SIG_SPEED RF24_1MBPS



RF24 radio(9, 10); 




byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; 

int transmit_data[5];         
int telemetry[2];              
byte rssi;
int trnsmtd_pack = 1, failed_pack;
unsigned long RSSI_timer;
byte leftX =  2;
byte leftY =  3;
byte rightX = 1;
byte rightY = 0;
byte led = 6;
byte mode1;
byte mode2;
byte mode3;
byte telemetryON = 7;
int mode4;
int mode5;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600); 
pinMode(4,INPUT);
pinMode(3,INPUT);
lcd.begin();
lcd.backlight();
lcd.setCursor(0,0);
lcd.print("Dmytro");
lcd.setCursor(4,1);
lcd.print("Panteliuk");
delay(2000);
lcd.clear();
lcd.setCursor(4,0);
lcd.print("SCORPION");

  radioSetup();
}

void loop() {

  
mode1 = digitalRead(4);
if(mode1 == HIGH){
    transmit_data[0] = 1;
}

if(mode1== LOW && mode2==LOW){
    transmit_data[0] = 2;
}

mode2 = digitalRead(3);
if(mode2 == HIGH){
  transmit_data[0] = 3;
}



mode4 = digitalRead(6);
if(mode4 == HIGH){
 transmit_data[0] = 4;
}

mode5 = digitalRead(5);
if(mode5 == HIGH ){
 transmit_data[0] = 5;
}
  transmit_data[0];
  transmit_data[1] = map(analogRead(leftX),0,1023,0,255);
  transmit_data[2] = map(analogRead(leftY),0,1023,0,255);
  transmit_data[3] = map(analogRead(rightX),0,1023,0,255);
  transmit_data[4] = map(analogRead(rightY),0,1023,0,255);
Serial.print(transmit_data[0]);
Serial.print(" ");
Serial.print(transmit_data[1]);
Serial.print(" ");
Serial.print(transmit_data[2]);
Serial.print(" ");
Serial.print(transmit_data[3]);
Serial.print(" ");
Serial.println(transmit_data[4]);
  
  if (radio.write(&transmit_data, sizeof(transmit_data))) {    
    trnsmtd_pack++;
    if (!radio.available()) {                                  
    } else {
      while (radio.available() ) {                    
        radio.read(&telemetry, sizeof(telemetry));    
        
      }
    }
  } else {
    failed_pack++;
  }
if(transmit_data[0] == 5){
if(telemetry[1]>100){
lcd.begin();
lcd.backlight();
lcd.setCursor(0,0);
lcd.print("Illumination");
lcd.print(" ");
lcd.print(telemetry[1]);
lcd.print("%");
lcd.setCursor(0,1);
lcd.print("CO2");
lcd.print(" ");
lcd.print(telemetry[0]);
lcd.print("%");
delay(4000);
}
if(telemetry[1]<100){
lcd.begin();
lcd.backlight();
lcd.setCursor(0,0);
lcd.print("Humidity");
lcd.print(" ");
lcd.print(telemetry[1]);
lcd.print("%");
lcd.setCursor(0,1);
lcd.print("Temperature");
lcd.print(" ");
lcd.print(telemetry[0]);
lcd.print("*c");
delay(4000);
}


if(telemetry[1] == 222 ){
lcd.begin();
lcd.backlight();
lcd.setCursor(0,0);
lcd.print("FIRE !!!");
delay(4000);
}



}


  if (millis() - RSSI_timer > 1000) {    // таймер RSSI
    
    
    rssi = (1 - ((float)failed_pack / trnsmtd_pack)) * 100;

    
    failed_pack = 0;
    trnsmtd_pack = 0;
    RSSI_timer = millis();
  }

}

void radioSetup() {
  radio.begin();              
  radio.setAutoAck(1);        
  radio.setRetries(0, 15);    
  radio.enableAckPayload();   
  radio.setPayloadSize(32);   
  radio.openWritingPipe(address[0]);   
  radio.setChannel(CH_NUM);            
  radio.setPALevel(SIG_POWER);         
  radio.setDataRate(SIG_SPEED);        
  

  radio.powerUp();         
  radio.stopListening();   
}
