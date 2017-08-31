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
//Storage Variables

#define MOVING_AVERAGE_SIZE 10

volatile int analog_values[4][MOVING_AVERAGE_SIZE];
volatile int analog_means[4];


void setup_timer0(){
	/*TIMER 0 Interrupt at 2kHz*/
	//Set TCR0A and B to 0
	TCCR0A = 0;
	TCCR0B = 0;
	//Initialize the counter
	TCNT0 = 0;
	//Set compare match register for 2Khz
	OCR0A = 100;
	TCCR0A |= (1<<WGM01);
	TCCR0B |= (1<<CS12) | (1<<CS10);
	TIMSK0 |= (1<<OCIE0A);
}

void setup_timer1(){
	/*TIMER 1*/
	//Set TCCR1A and B to 0
	TCCR1A = 0;
	TCCR1B = 0;
	//Initialize the counter
	TCNT1 = 0;
	//Set Compare Match Register A 1Hz 15624 * 1024/16000000 = 1 s = 1000 ms
	OCR1A = 15624;
	//Set Compare Match Regigister B 10ms 156 * 1024/16000000 = 0.01 s = 10 ms
	OCR1B = 156;
	//Turn on CTC mode
	TCCR1B |= (1<<WGM12);
	//SEt prescaler to 1024
	TCCR1B |= (1<<CS12) | (1<<CS10);
	//enable timer compare interrupt
	TIMSK1 |= (1<<OCIE1A) | (1<<OCIE1B) ;
}

void setup(){
	//Initialize Serial Communication
	Serial.begin(9600);
	//Define Ports
	pinMode(13,OUTPUT);
	pinMode(9,OUTPUT);
	//Setup the timer interruptions
	cli();
	setup_timer0();
	setup_timer1();
	sei();
}


void loop(){
	long int timer_init = 0;
	timer_init = millis();

	while(1){

		if (millis() - timer_init > 500){
			//Every 500 ms build msg and send the command

		}

	}

	Serial.println("Program Running....");
	Serial.println(analogRead(0));
	_delay_ms(2000);
}



ISR(TIMER1_COMPA_vect){
	bool a = False;
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
	}

}

ISR(TIMER0_COMPA_vect){
	bool b = False;
}

