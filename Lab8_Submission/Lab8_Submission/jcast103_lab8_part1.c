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

void set_PWM(double frequency){
        //static double current_frequency;
        //if(frequency != current_frequency){
                if(!frequency) { TCCR3B &= 0x08; }
                else { TCCR3B |= 0x03; }

                if(frequency <  0.954) { OCR3A = 0xFFFF; }
                else if(frequency > 31250) { OCR3A = 0x0000; }
                else { OCR3A = (short)(8000000 / (128*frequency)) - 1; }

                TCNT3 = 0;
                //current_frequency = frequency;
        //}
}

void PWM_on(){
        TCCR3A = (1 << COM3A0);

        TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);

        set_PWM(0);
}

void PWM_off(){
        TCCR3A = 0x00;
        TCCR3B = 0x00;
}

enum EX_STATE { SMStart, Wait, Noise } state;

void EX_1(){
	unsigned char A0 = ~PINA & 0x01;
	unsigned char A1 = ~PINA & 0x02;
        unsigned char A2 = ~PINA & 0x04;	
	switch(state){
		case SMStart:
			state = Wait;
			break;

		case Wait:
			if(A0 || A1 || A2){ state = Noise; }
			else{ state = Wait; }
			break;

		case Noise:
			if(A0 || A1 || A2){ state = Noise; }
			else{ state = Wait; }
		        break;	

		default:
			state = SMStart;
			break;
	}

	switch(state){
		case Wait:
		        set_PWM(0);	
			break;

		case Noise:
			if((A0 && A1) || (A0 && A2) || (A1 && A2) || (A0 && A1 && A2)){ set_PWM(0); } 
			else if(A0 && !A1 && !A2){ set_PWM(261.63); }
			else if(A1 && !A0 && !A2){ set_PWM(293.66); }
			else if(A2 && !A0 && !A1){ set_PWM(329.63); }
			break;

		default:
			break;


	}

}


int main(void) {
	/* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(50);
	TimerOn();
	PWM_on();
	/* Insert your solution below */
	while (1) {
		EX_1();
		while(!TimerFlag);
		TimerFlag = 0;
	}
        return 1;
}
