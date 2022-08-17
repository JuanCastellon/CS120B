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
	unsigned char tempB = 0x00;
	unsigned char tempD = 0x00;

	ADC_init();

	unsigned short x;

	while (1) {
		x = ADC;
		tempB = (char)x;
		tempD = (char)x;
		tempD = (char)(x >> 8);
		PORTB = tempB;
		PORTD = tempD;
	}
	return 1;
}
