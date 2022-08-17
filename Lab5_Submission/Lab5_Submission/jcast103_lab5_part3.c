/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 5  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	URL For Video: https://drive.google.com/drive/folders/1POXsyXyOXTPqcNs0RXwBIzqDHqxPsBPV
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum Light_States {Light_SMStart, Light_Seq1, Light_Wait1, Light_Seq2, Light_Wait2, Light_Seq3, Light_Wait3,
       	           Light_Seq4, Light_Wait4, Light_Seq5, Light_Wait5} Light_State;
void TickFct_Light(){
	unsigned char button = ~PINA & 0x01;
	unsigned char led = 0x00;
	switch(Light_State){
		case Light_SMStart:
			Light_State = Light_Seq1;
			break;

		case Light_Seq1:
			if(!button){ Light_State = Light_Seq1; }
			if(button) { Light_State = Light_Wait1; }
			break;

		case Light_Wait1:
			if(button) { Light_State = Light_Wait1;}
                        if(!button){ Light_State = Light_Seq2; }
                        break;

		case Light_Seq2:
			if(!button){ Light_State = Light_Seq2; }
			if(button) { Light_State = Light_Wait2;}
			break;

		case Light_Wait2:
			if(button) { Light_State = Light_Wait2; }
			if(!button){ Light_State = Light_Seq3; }
			break;

		case Light_Seq3:
			if(!button){ Light_State = Light_Seq3; }
                        if(button) { Light_State = Light_Wait3; }
			break;

		case Light_Wait3:
                        if(button) { Light_State = Light_Wait3; }
                        if(!button){ Light_State = Light_Seq4; }
                        break;

		case Light_Seq4:
		       if(!button){ Light_State = Light_Seq4; }
                       if(button) { Light_State = Light_Wait4; }	
		       break;

		case Light_Wait4:
			if(button) { Light_State = Light_Wait4; }
                        if(!button){ Light_State = Light_Seq5; }
			break;

		case Light_Seq5:
                        if(!button){ Light_State = Light_Seq5; }
                        if(button) { Light_State = Light_Wait5; }
                        break;

		case Light_Wait5:
			if(button){ Light_State = Light_Wait5; }
			if(!button) { Light_State = Light_Seq1; }
			break;

		default:
			Light_State = Light_SMStart;
			break;
	}

	switch(Light_State){
		case Light_Seq1:
			led = 0x21;
			break;

		case Light_Wait1:
			break;

		case Light_Seq2:
			led = 0x12;
			break;

		case Light_Wait2:
                        break;

		case Light_Seq3:
			led = 0x0C;
			break;

		case Light_Wait3:
			break;

		case Light_Seq4:
			led = 0x2A;
			break;

		case Light_Wait4:
                        break;

		case Light_Seq5:
			led = 0xFF;
			break;

		case Light_Wait5:
			break;

		default:
			break;
	}
	PORTB = led;
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; //Configure PORTA as input, intiialize to 1s
	DDRB = 0xFF; PORTB = 0x00; //Configure PORTB as output, intialize to 0s

	Light_State = Light_SMStart;

    	while (1) {
		TickFct_Light();
    	}
   	return 1;
}
