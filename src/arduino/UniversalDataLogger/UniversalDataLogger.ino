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

volatile long int analog_values[11];
volatile int analog_counters[11];


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
	//Set Compare Match Register A 1Hz
	OCR1A = 15624;
	//Set Compare Match Regigister B 200ms = 5Hz
	OCR1B = 1561;
	//Turn on CTC mode
	TCCR1B |= (1<<WGM12);
	//SEt prescaler to 1024
	TCCR1B |= (1<<CS12) | (1<<CS10);
	//enable timer compare interrupt
	TIMSK1 |= (1<<OCIE1A) | (1<<OCIE1B) ;
}

void setup(){
	pinMode(13,OUTPUT);
	pinMode(9,OUTPUT);

	Serial.begin(9600);


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
	int state = digitalRead(13);
	state = !state;
	digitalWrite(13,state);

	digitalWrite(9,LOW);
	Serial.println("On/off");
}

ISR(TIMER1_COMPB_vect){
	digitalWrite(9,HIGH);
	/*
	int state1 = digitalRead(9);
	state1 = !state1;
	digitalWrite(9,state1);
	*/
}

ISR(TIMER0_COMPA_vect){
  if(!(analogRead(0) > 0)){
    int state1 = digitalRead(9);
    state1 = !state1;
    digitalWrite(9,state1);
  }
}

