/*
UniversalDataLogger for Arduino
Created by @Jfdzar 31.08.2017

The program reads different signals from Arduino UNO Board and send it through the serial port

Following Signals
Analog Signals value from 0-1023 0-Vin V:
- A0
- A1
- A2
- A3
- A4
- A5

Digital Signals Read the frequency in Hz (rising edges) every LOGGIN_RATE inverval
- 5
- 6
- 7
 */

#include <util/delay.h>
#include "UniversalDataLogger.h"
//Storage Variables

#define ANALOG_INPUTS 6
#define DIGITAL_INPUTS 3
#define LOGGING_RATE 500 //Logging Rate in ms
#define FREQUENCY_TIMER_1A 1 //Frequency in Hz
#define FREQUENCY_TIMER_1B 100 //Frequency in Hz
#define PRESCALER 1024 //Prescaler of Timer_1
#define CLK_FREQUENCY 16000000
//Moving Average is calculated by (LOGGING_RATE*FREQUENCY_TIMER_1B)/1000
#define MOVING_AVERAGE_SIZE 50



volatile int analog_values[ANALOG_INPUTS][MOVING_AVERAGE_SIZE];
volatile int analog_means[ANALOG_INPUTS];

volatile int di_counter[DIGITAL_INPUTS] = {0,0,0};
volatile int led_state = 0;


void setup(){
	//Initialize Serial Communication
	Serial.begin(9600);
	//Define Ports
	pinMode(13,OUTPUT);
	pinMode(5,INPUT);
	pinMode(6,INPUT);
	pinMode(7,INPUT);
	//Setup the timer interruptions
	cli();
	setup_timer1();
	//Set Compare Match Regigister B 10ms 156 * 1024/16000000 = 0.01 s = 10 ms (OCR1B = 156;)
	OCR1B = int(CLK_FREQUENCY/FREQUENCY_TIMER_1B/PRESCALER);

	setup_pcint2();
	sei();
}


void loop(){
  unsigned long timer_init = 0;
	timer_init = millis();
	int di_status[DIGITAL_INPUTS] = {0,0,0};
	int di_old_status[DIGITAL_INPUTS] = {0,0,0};
	while(1){

		/*
		for (int i=0; i < DIGITAL_INPUTS; i++){
			di_status[i] = digitalRead(5+i);
			if (di_status[i] == 1){
				if (di_status[i] != di_old_status[i]){
					di_counter[i] = di_counter[i] + 1;
				}
				di_old_status[i] = 1;
			}
			else{
				di_old_status[i] = 0;
			}
		}*/

		if (millis() - timer_init > LOGGING_RATE){
			//Every LOGGIN_RATE ms build Msg and send the command
			String Msg = "";
			for (int i=0; i < ANALOG_INPUTS ; i++){
				Msg = Msg + String(analog_means[i]) + String(";");
			}
			for (int i=0; i < DIGITAL_INPUTS ; i++){
				Msg = Msg + String(int(1000*di_counter[i]/LOGGING_RATE)) + String(";");
        di_counter[i] = 0;
			}
			Msg = Msg + String("OK");
			Serial.println(Msg);
			timer_init = millis();
		}

	}
}


ISR(PCINT2_vect){

	uint8_t pin_status;
	pin_status = PIND;
	if(pin_status & (1 << PIND5))
	{
		di_counter[0] = di_counter[0] + 1;
	}
	if(pin_status & (1 << PIND6))
	{
		di_counter[1] = di_counter[1] + 1;
	}
	if(pin_status & (1 << PIND7))
	{
		di_counter[2] = di_counter[2] + 1;
	}

	if (led_state){
		digitalWrite(13,LOW);
		led_state = 0;
	}
	else{
		digitalWrite(13,HIGH);
		led_state = 1;
	}
}



ISR(TIMER1_COMPB_vect){
	//Read the new value of the analog input as first element of the array and move the rest forward
	for (int i=0; i < ANALOG_INPUTS ; i++){
		for (int j=MOVING_AVERAGE_SIZE-1; j >= 1; j--){
			analog_values[i][j] = analog_values[i][j-1];
		}
		analog_values[i][0] = analogRead(i);
	}

	//Calculate the average of every analog signal and save it in the array analog_means
	for (int i=0; i < ANALOG_INPUTS ; i++){
		long int sum = 0;
			for (int j=0; j < MOVING_AVERAGE_SIZE; j++){
				sum = sum + analog_values[i][j];
			}
	analog_means[i] = int(sum / MOVING_AVERAGE_SIZE);
	}

	//Initialize Counter of Timer 1
	TCNT1 = 0;
}

