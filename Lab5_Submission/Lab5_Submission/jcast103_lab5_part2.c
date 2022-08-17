#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum EX2_States { EX_SMStart, EX_Wait, EX_Init, EX_A0Press, EX_A1Press, EX_Wait0, EX_Wait1, EX_BothPress, EX_Wait3 } EX2_State;

//Global Variable
unsigned char tempC = 0x07;
unsigned char A0;
unsigned char A1;

void TickFct_EX2(){

	if(~PINA & 0x01){ 
		A0 = ~PINA & 0x01;
	        if(tempC == 0){ A1 = ~PINA & 0x02; }	
       	}
	else if(~PINA & 0x02){ 
		A1 = ~PINA & 0x02;
		if(tempC == 0){ A0 = ~PINA & 0x01; }	
	}
	else{ A0 = ~PINA & 0x01; A1 = ~PINA & 0x02; }
	
	switch (EX2_State){
		case EX_SMStart:
			EX2_State = EX_Wait;
			break;

		case EX_Wait:
			EX2_State = EX_Init;
			break;


		case EX_Init:
			if(A0 && A1){ EX2_State = EX_BothPress; } 
			else if(A0 && !A1){ EX2_State = EX_A0Press; }
			else if(A1 && !A0){ EX2_State = EX_A1Press; }
			else{ EX2_State = EX_Init; }
			break;

		case EX_A0Press:
			EX2_State = EX_Wait0;
			break;

		case EX_A1Press:
			EX2_State = EX_Wait1;
			break;

		case EX_Wait0:
			if(A0 && A1){ EX2_State = EX_BothPress; }
			else if(A0 && !A1){ EX2_State = EX_Wait0; }
			else{ EX2_State = EX_Init; }
			break;


		case EX_Wait1:
			if(A1 && A0){ EX2_State = EX_BothPress; }
			else if(A1 && !A0){ EX2_State = EX_Wait1; }
			else{ EX2_State = EX_Init; }
			break;

		case EX_BothPress:
			EX2_State = EX_Init;
		        break;

		case EX_Wait3:
			if(!A0 && !A1){ EX2_State = EX_Init; }
			else{ EX2_State = EX_Wait3; }
			break;

		default:
			EX2_State = EX_SMStart;
			break;

	}

	switch(EX2_State){
		case EX_SMStart:
			break;

		case EX_Wait:
			break;

		case EX_Init:
			//PORTC = tempC;
			break;

		case EX_A0Press:
			if(tempC < 9){ tempC++; }
			break;

		case EX_A1Press:
			if(tempC > 0){ tempC = tempC - 1; }
			break;

		case EX_Wait0:
			break;

		case EX_Wait1:
			break;

		case EX_BothPress:
			tempC = 0;
			//PORTC = tempC;
			break;

		case EX_Wait3:
			break;

		default:
			break;



	}
	PORTC = tempC;	

}

int main(void) {
    	/* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x07;

	EX2_State = EX_SMStart;

	/* Insert your solution below */
    
	while (1) {
		TickFct_EX2();
	}
    return 0;
}
