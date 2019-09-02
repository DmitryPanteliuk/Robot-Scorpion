
#include <dht11.h>
dht11 DHT;
#define DHT11_PIN 8


#define CH_NUM 0x60   
#define SIG_POWER RF24_PA_HIGH
#define SIG_SPEED RF24_1MBPS
#define analogPin A0

#include <NewPing.h>
#define TRIGGER_PIN 12
#define ECHO_PIN 11
#define MAX_DISTANCE 200
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <Servo.h>
Servo servo;
RF24 radio(9, 10);



byte pipeNo;
byte address[][6] = { "1Node", "2Node", "3Node", "4Node", "5Node", "6Node" };

int resive_data[5];         // массив принятых данных
int telemetry[2];             // массив данных телеметрии (то что шлём на передатчик)
int IN1 = 40;
int IN2 = 41;
int IN3 = 38;
int IN4 = 43;
int led1 = 6;
int led2 = 7;
int firePin = A2;
int leftD = 34;
int centrD = 32;
int rigtD = 30;

float analogValue;
int lux = A3;

int servo1 = 2;
int servo2 = 3;
int servo3 = 4;

Servo myservo1;
Servo myservo2;
Servo myservo3;

int Time;

int data;
int h;
int t;

void setup() {
	Serial.begin(9600);
	myservo1.attach(servo1);
	myservo2.attach(servo2);
	myservo3.attach(servo3);





	pinMode(lux, INPUT);
	pinMode(analogPin, INPUT);
	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);
	pinMode(IN4, OUTPUT);
	pinMode(IN3, OUTPUT);
	pinMode(led1, OUTPUT);
	pinMode(led2, OUTPUT);
	pinMode(leftD, INPUT);
	pinMode(centrD, INPUT);
	pinMode(rigtD, INPUT);
	pinMode(firePin, INPUT);





	radioSetup();
}

void loop() {

	while (radio.available(&pipeNo)) {                                 // слушаем эфир
		radio.read(&resive_data, sizeof(resive_data));              // чиатем входящий сигнал
		Serial.print(resive_data[0]);
		Serial.print(" ");
		Serial.print(resive_data[1]);
		Serial.print(" ");
		Serial.print(resive_data[2]);
		Serial.print(" ");
		Serial.print(resive_data[3]);
		Serial.print(" ");
		Serial.println(resive_data[4]);

		// первый режим работы
		if (resive_data[0] == 1) {

			myservo1.write(100);
			myservo2.write(100);

			digitalWrite(led1, HIGH);
			digitalWrite(led2, HIGH);

			if (resive_data[1] > 140) {        //правая вперед
				digitalWrite(IN1, LOW);
				digitalWrite(IN2, HIGH);
				Serial.println("левая вперед");
			}

			if (resive_data[4] > 140) {       // левая вперед
				digitalWrite(IN3, HIGH);
				digitalWrite(IN4, LOW);
				Serial.println("правая вперед");
			}

			if (resive_data[1] < 90) {        //правая назад
				digitalWrite(IN1, HIGH);
				digitalWrite(IN2, LOW);
				Serial.println("левая назад");
			}

			if (resive_data[4] < 90) {       // левая назад
				digitalWrite(IN3, LOW);
				digitalWrite(IN4, HIGH);
				Serial.println("правая назад");
			}

			if (resive_data[1] > 90 && resive_data[1] < 140) {
				digitalWrite(IN1, LOW);
				digitalWrite(IN2, LOW);
				Serial.println("STOP LEFT");
			}

			if (resive_data[4] > 90 && resive_data[4] < 140) {
				digitalWrite(IN3, LOW);
				digitalWrite(IN4, LOW);
				Serial.println("STOP RIGHT");
			}
		}




		//второй режим работы

		if (resive_data[0] == 2) {
			servo1 = map(resive_data[3], 0, 255, 0, 180);
			myservo1.write(servo1);
			servo2 = map(resive_data[4], 0, 255, 0, 180);
			myservo2.write(servo2);
			servo3 = map(resive_data[2], 0, 255, 0, 180);
			myservo3.write(servo3);
			digitalWrite(led1, HIGH);
			digitalWrite(led2, HIGH);


		}


		//третий режим работы

		if (resive_data[0] == 3) {
			/*
			   if(digitalRead(rigtD)== HIGH){
				delay(50);
			  analogWrite(EN1,55);
			  analogWrite(EN2,55);
			  digitalWrite(IN1,LOW);
			  digitalWrite(IN2,LOW);
			  digitalWrite(IN3,HIGH);
			  digitalWrite(IN4,LOW);
			 delay(50);
			 }

			  if(digitalRead(centrD)== HIGH){

			  analogWrite(EN1,100);
			  analogWrite(EN2,100);
			  digitalWrite(IN1,LOW);
			  digitalWrite(IN2,HIGH);
			  digitalWrite(IN3,HIGH);
			  digitalWrite(IN4,LOW);
			 }

			  if(digitalRead(leftD)== HIGH){
				delay(50);
			  analogWrite(EN1,55);
			  analogWrite(EN2,55);
			  digitalWrite(IN1,LOW);
			  digitalWrite(IN2,HIGH);
			  digitalWrite(IN3,LOW);
			  digitalWrite(IN4,LOW);
			   delay(50);
			 }
			 if(digitalRead(rigtD)== HIGH && digitalRead(centrD)== HIGH && digitalRead(leftD)== HIGH){

			  digitalWrite(IN1,LOW);
			  digitalWrite(IN2,LOW);
			  digitalWrite(IN3,LOW);
			  digitalWrite(IN4,LOW);
			 }
			 if(digitalRead(rigtD)== LOW && digitalRead(centrD)== LOW && digitalRead(leftD)== LOW ){

			  digitalWrite(IN1,LOW);
			  digitalWrite(IN2,LOW);
			  digitalWrite(IN3,LOW);
			  digitalWrite(IN4,LOW);
			   }
			}




			*/
		}

		if (resive_data[0] == 4) {
			int i = 0;

			if (digitalRead(leftD) == LOW) {
				i = 1;
			}
			if (digitalRead(centrD) == LOW) {
				i = 2;
			}
			if (digitalRead(rigtD) == LOW) {
				i = 3;
			}
			while (i == 1) {

				digitalWrite(IN1, LOW);
				digitalWrite(IN2, HIGH);
				digitalWrite(IN3, LOW);
				digitalWrite(IN4, HIGH);
				if (digitalRead(rigtD) == LOW) {
					i = 3;
				}
				if (digitalRead(centrD) == LOW) {
					i = 2;
				}

			}
			while (i == 2) {
				digitalWrite(IN1, LOW);
				digitalWrite(IN2, HIGH);
				digitalWrite(IN3, HIGH);
				digitalWrite(IN4, LOW);
				if (digitalRead(leftD) == LOW) {
					i = 1;
				}
				if (digitalRead(rigtD) == LOW) {
					i = 3;
				}

			}
			while (i == 3) {
				digitalWrite(IN1, HIGH);
				digitalWrite(IN2, LOW);
				digitalWrite(IN3, HIGH);
				digitalWrite(IN4, LOW);
				if (digitalRead(leftD) == LOW) {
					i = 1;
				}
				if (digitalRead(centrD) == LOW) {
					i = 2;
				}

			}

		}



		// формируем пакет данных телеметрии 




		if (resive_data[0] == 5) {
			digitalWrite(led1, LOW);
			digitalWrite(led2, LOW);
			int i;
			i = 1;

			if (i == 1) {
				data = DHT.read(DHT11_PIN);
				h = (DHT.humidity);
				t = (DHT.temperature);
				telemetry[0] = t;
				telemetry[1] = h;
				i = 2;
				radio.writeAckPayload(pipeNo, &telemetry, sizeof(telemetry));
				delay(500);
			}
			if (i == 2) {
				telemetry[0] = analogRead(analogPin);
				telemetry[1] = analogRead(lux);
				i = 1;
				radio.writeAckPayload(pipeNo, &telemetry, sizeof(telemetry));
				delay(500);
			}




			Serial.print(telemetry[0]);
			Serial.print(" ");
			Serial.println(telemetry[1]);

			// отправляем пакет телеметрии

			Time = millis();
			Serial.println(Time);
		}

	}

}



void radioSetup() {
	radio.begin();
	radio.setAutoAck(1);
	radio.setRetries(0, 15);
	radio.enableAckPayload();
	radio.setPayloadSize(32);
	radio.openReadingPipe(1, address[0]);
	radio.setChannel(CH_NUM);
	radio.setPALevel(SIG_POWER);
	radio.setDataRate(SIG_SPEED);


	radio.powerUp();
	radio.startListening();
}