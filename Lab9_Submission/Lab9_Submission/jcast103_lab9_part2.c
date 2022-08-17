/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *      URL for Video: https://drive.google.com/drive/u/1/folders/1POXsyXyOXTPqcNs0RXwBIzqDHqxPsBPV
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

//Variables for setting the state of the LEDs
unsigned char threeLEDs = 0;
unsigned char blinkingLED = 0;

unsigned short secCnt1 = 1;
unsigned short tickCnt2 = 0;

enum THREE_STATES { T_SMStart, Light1, Light2, Light3 } state1; 
enum BLINK_STATES { B_SMStart, On, Off } state2;
//enum COMBI_STATES { C_SMStart, WaitC, Output} state3;

void ThreeLEDsSM(){
	switch(state1){ 
		case T_SMStart:
			state1 = Light1;
			break;

		case Light1:
			state1 = Light2;
			break;

		case Light2:
			state1 = Light3;
			break;

		case Light3:
			state1 = Light1;
			break;

		default:
			state1 = T_SMStart;
			break;

	}

	switch(state1){
		case Light1:
			threeLEDs = 0x01;
			break;

		case Light2:
			threeLEDs = 0x02;
			break;

		case Light3:
			threeLEDs = 0x04;
			break;

		default:
			break;

	}

}

void BlinkingLEDSM(){
	switch(state2){
		case B_SMStart:
			state2 = On;
			break;

		case On:
			state2 = Off;
			break;

		case Off:
			state2 = On;
			break;

		default:
			state2 = B_SMStart;
			break;
	}

	switch(state2){
		case On:
			blinkingLED = 0x01;
			break;

		case Off:
			blinkingLED = 0x00;
			break;

		default:
			break;

	}
}

void CombineLEDsSM(){
	unsigned char temp = 0x00;
	temp = threeLEDs | (blinkingLED << 3);
	PORTB = temp;

}

int main(void) {
	/* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00; //Setting port B to output
	/* Insert your solution below */
	unsigned short tickCnt1 = 1;
	unsigned short tickCnt2 = 1;
	TimerSet(1);
	TimerOn();

	while (1) {
		if(tickCnt1 == 300){
			ThreeLEDsSM();
			tickCnt1 = 0;
		}
		if(tickCnt2 == 1000){
			BlinkingLEDSM();
			tickCnt2 = 0;
		}
		CombineLEDsSM();
		while(!TimerFlag);
		TimerFlag = 0;
		tickCnt1++;
		tickCnt2++;
    	}
    	return 1;
}
