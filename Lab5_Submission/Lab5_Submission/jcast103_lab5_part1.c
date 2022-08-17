

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;

    unsigned char bA0 = 0x00;
    unsigned char bA1 = 0x00;
    unsigned char bA2 = 0x00;
    unsigned char bA3 = 0x00;
    unsigned char tempA = 0x00;
    unsigned char tempC = 0x00;

    /* Insert your solution below */
    while (1) {

	   tempA = ~PINA & 0x0F;
	   //bA0 = ~PINA & 0x01;
	   //bA1 = ~PINA & 0x02;
	   //bA2 = ~PINA & 0x04;
	   //bA3 = ~PINA & 0x08;
	   //tempA = bA3;
           //tempA = (tempA << 1) | bA2;
           //tempA = (tempA << 1) | bA1;
           //tempA = (tempA << 1) | bA0;
	   
	   if(tempA == 0){
		tempC = 0x40;
	   }
	   if(tempA == 1 || tempA == 2){
		tempC = 0x60;
	   }
	   if(tempA == 3 || tempA == 4){
		tempC = 0x70;
	   }
	   if(tempA == 5 || tempA == 6){
                tempC = 0x38;
           }
	   if(tempA >= 7 && tempA <= 9){
		tempC = 0x3C;
	   }
	   if(tempA >= 10 && tempA <= 12){
		tempC = 0x3E;
	   } 
	   if(tempA >= 13 && tempA <= 15){
		tempC = 0x3F;
	   }
	   PORTC = tempC;
				
    }
    return 0;
}
