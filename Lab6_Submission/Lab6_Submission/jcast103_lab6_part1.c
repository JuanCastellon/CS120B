/*	Author: Juan Castellon
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	URL for Video: https://drive.google.com/drive/u/1/folders/1POXsyXyOXTPqcNs0RXwBIzqDHqxPsBPV
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0; //TimerISR() sets this to 1. C programmer should clear to 0.

//Internal Variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; //Start counter from here, down ot 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; //Current internal countere of 1ms ticks

void TimerOn(){
	//AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B; //bit3 = 0; CTC mode (clear timer on compare)
		       //bit2bit1bit0 = 011: pre-scaler /64
		       //0001011: 0x0B
		       //SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks
		       //Thus, TCNT1 register will count at 125,000 ticks
	
	//AVR output compare register OCR1A
	
	OCR1A = 125;   //Timer interrupt will be generated when TCNT1 == OCR1A
		       //We want a 1 ms tick 0.001 s * 125,000 ticks/s = 125;
		       //So when TCNT1 register equals 25,
		       //1 ms has passed. Thus we compare to 125.
		       //
	
	//AVR Timer interrupt mask register
	TIMSK1 = 0x02; //bit1: OCIE1A -- enables compare match interrupt

	//Intialize avr counter
	TCNT1 = 0;

	_avr_timer_cntcurr = _avr_timer_M;
	//TimerISR will be called every _avr_timer_cntcurr milliseconds
	
	//Enable global interrupts
	SREG |= 0x80; //0x80: 1000000
}

//The "enable global interrupts" line is SREG |= 0x80, not SREG |- 0x80;

void TimerOff(){
	TCCR1B = 0x00; //bit3bit1bit0 = 000: timer off
}

void TimerISR(){
	TimerFlag = 1;
}

//In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect){
	//CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		TimerISR(); //Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

//Set TimerISR() to tick every M ms
void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

enum P1_STATES { P1_SMStart, P1_PB0, P1_PB1, P1_PB2 } P1_State;

void TickFctP1(){
	unsigned char tmpB = 0x00;
	switch(P1_State){
		case P1_SMStart:
			P1_State = P1_PB0;
			break;

		case P1_PB0:
			P1_State = P1_PB1;
			break;

		case P1_PB1:
			P1_State = P1_PB2;
			break;

		case P1_PB2:
			P1_State = P1_PB0;
			break;

		default:
			P1_State = P1_SMStart;
			break;
	}

	switch(P1_State){
		case P1_SMStart:
			break;

		case P1_PB0:
			tmpB = 0x01;
			break;

		case P1_PB1:
			tmpB = 0x02;
			break;

		case P1_PB2:
			tmpB = 0x04;
			break;

		default:
			break;
	}

	PORTB = tmpB;
}

int main(void) {
	DDRB = 0xFF; //Set port B to output
    	PORTB = 0x00;//Initalize port B to 0s
	TimerSet(1000);
	TimerOn();

    	while (1) {
		TickFctP1();
		while(!TimerFlag);
		TimerFlag = 0;
    	
	}
    	return 1;
}
