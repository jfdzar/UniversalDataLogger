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





