/*	Author: lab
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
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init(){
	ADCSRA |= (1<<ADEN) | (1<<ADSC) | (1<<ADATE);
}

int main(void) {
	/* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    	/* Insert your solution below */
	//unsigned char tempB = 0x00;
	//unsigned char tempD = 0x00;

	ADC_init();

	unsigned short x;
	unsigned short max = 0x30F;

	while (1) {
		x = ADC;
		
		if(x >= (max/2)){ PORTB = 0x01; }
		else{ PORTB = 0x00; } 
		PORTD = 0x00;
	}

	return 1;
}
