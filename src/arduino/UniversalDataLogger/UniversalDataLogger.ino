/*

UniversalDataLogger for Arduino
Created by @Jfdzar 31.08.2017

The program reads different signals from Arduino UNO Board and send it through the serial port

Following Signals
Analog Signals
- A0
- A1
- A2
- A3

Digital Signals Read rising edges every 500 ms

- 3
- 4
- 6

 */

#include <util/delay.h>
#include "UniversalDataLogger.h"
//Storage Variables

#define LOGGING_RATE 500 //Logging Rate in ms
#define FREQUENCY_TIMER_1A 1 //Frequency in Hz
#define FREQUENCY_TIMER_1B 100 //Frequency in Hz
#define PRESCALER 1024 //Prescaler of Timer_1
#define CLK_FREQUENCY 16000000
//Moving Average is calculated by (LOGGING_RATE*FREQUENCY_TIMER_1B)/1000
#define MOVING_AVERAGE_SIZE 50


volatile int analog_values[4][MOVING_AVERAGE_SIZE];
volatile int analog_means[4];
volatile int led_state = 0;

void setup(){
	//Initialize Serial Communication
	Serial.begin(9600);
	//Define Ports
	pinMode(13,OUTPUT);
  pinMode(9,OUTPUT);
	//Setup the timer interruptions
	cli();
	setup_timer1();
	//Set Compare Match Register A 1Hz 15624 * 1024/16000000 = 1 s = 1000 ms (OCR1A = 15624;)
	OCR1A = int(CLK_FREQUENCY/FREQUENCY_TIMER_1A/PRESCALER);
	//Set Compare Match Regigister B 10ms 156 * 1024/16000000 = 0.01 s = 10 ms (OCR1B = 156;)
	OCR1B = int(CLK_FREQUENCY/FREQUENCY_TIMER_1B/PRESCALER);
	sei();
}


void loop(){
	long int timer_init = 0;
	timer_init = millis();

	while(1){

		if (millis() - timer_init > LOGGING_RATE){
			//Every LOGGIN_RATE ms build Msg and send the command
			String Msg = "";
			for (int i=0; i < 4 ; i++){
				Msg = Msg + String(analog_means[i]) + String(";");
			}
			Msg = Msg + String("OK");
			Serial.println(Msg);
			timer_init = millis();
		}

	}
}




ISR(TIMER1_COMPB_vect){
	//Read the new value of the analog input as first element of the array and move the rest forward
	for (int i=0; i < 4 ; i++){
		for (int j=MOVING_AVERAGE_SIZE-1; j == 1; j--){
			analog_values[i][j] = analog_values[i][j-1];
		}
	    analog_values[i][0] = analogRead(i);
	}
	//Calculate the average of every analog signal and save it in the array
	for (int i=0; i < 4 ; i++){
		long int sum = 0;
			for (int j=0; j < MOVING_AVERAGE_SIZE; j++){
				sum = sum + analog_values[i][j];
			}
			analog_means[i] = int(sum / MOVING_AVERAGE_SIZE);
      analog_means[i] = analog_values[i][1];
	}

 if (led_state == 0) {
  digitalWrite(13,1);
  led_state = 1;
 }
 else{
  digitalWrite(13,0);
  led_state = 0;
 }
 TCNT1 = 0;
 

}
/*
ISR(TIMER0_COMPA_vect){
	bool b = 0;
}

ISR(TIMER1_COMPA_vect){
  bool a = 0;
}

*/

