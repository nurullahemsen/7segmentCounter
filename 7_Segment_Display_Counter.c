#include <stdint.h>
#include <stdbool.h>
#include "TM4C123GH6PM.h"
#include "gpio.h"                    // Device header includes display pin values
 
volatile int i = 0; 
volatile uint8_t count=00;
volatile bool flag = false;
volatile uint16_t tens =out7_0;
volatile uint16_t ones =out7_0;

void PORTB_init(); //Main port driving the displays
void PORTF_init(); //Supports onboard buttons and display enabling/disabling switch pins
void TIMER0A_init(); //Used for switching between displays
 
int main(void){
  PORTB_init();
	PORTF_init();
	TIMER0A_init();

	GPIOF->DATA = 0x04; //At first display for ones will be enabled
	
	/**
	Main loop:
	
	It divides the digits and then assign real 7-segment pin values 
	according to the digits
	**/
	while(1){
		uint8_t onlar = count / 10;
		uint8_t birler = count % 10;
		
		switch (onlar){
			case 0:
				tens = out7_0;
				break;
			case 1:
				tens = out7_1;
				break;
			case 2:
				tens = out7_2;
				break;
			case 3:
				tens = out7_3;
				break;
			case 4:
				tens = out7_4;
				break;
			case 5:
				tens = out7_5;
				break;
			case 6:
				tens = out7_6;
				break;
			case 7:
				tens = out7_7;
				break;
			case 8:
				tens = out7_8;
				break;
			case 9:
				tens = out7_9;
				break;
		}
		
		switch (birler){
			case 0:
				ones = out7_0;
				break;
			case 1:
				ones = out7_1;
				break;
			case 2:
				ones = out7_2;
				break;
			case 3:
				ones = out7_3;
				break;
			case 4:
				ones = out7_4;
				break;
			case 5:
				ones = out7_5;
				break;
			case 6:
				ones = out7_6;
				break;
			case 7:
				ones = out7_7;
				break;
			case 8:
				ones = out7_8;
				break;
			case 9:
				ones = out7_9;
				break;
		}
			
	}
}
/* interrupt handler for TIMER0A
	 it switches between PF_2 and PF_3 enabling the first and second displays
	 and sets PORTF output for corresponding display
*/
void TIMER0A_Handler(void){
	TIMER0->ICR|= (1<<0); //Clear interrupt flag for t0
	GPIOF->DATA ^= 0x0C; //invert PORTF_2 and PF_3
	if(flag)GPIOB->DATA = ones; //set ones digit
	else GPIOB->DATA = tens;		//set tens digit
	flag = !flag;								//invert flag
}

/* interrupt handler for PORTF
	it counts up or down according to the puutton pressed
*/
void GPIOF_Handler(void){
	for(i = 0; i <100000; i++);
	i=0;
	
	if(GPIOF->RIS & (1<<0)){ // left button pressed
		GPIOF->ICR|= (1<<0); //clear int flag
		count--;
	}
	else if(GPIOF->RIS & (1<<4)){ //right button pressed
		GPIOF->ICR|= (1<<4); //clear int flag
		count++;
	}
	
	if(count == 100) count = 0;
	else if(count == 255) count = 99;
	
	GPIOF->DATA ^= 0x02; //invert PORTF_2 (red led 0b000000010) to detect PORTF interrupt on board visually
}

void PORTB_init(){
	SYSCTL->RCGCGPIO|= (1<<1); //Enable clock to PORTB
	GPIOB->LOCK=0x4C4F434B;//unlock PORTF to write on GPIOCR register
	GPIOB->CR=0xFF; //unlock changes for all pins on DIR, DEN and other pullup and pulldown regisers
	GPIOB->AFSEL = 0x00; //disable alternate functions
	GPIOB->AMSEL = 0x00; //disable analog inputs
  GPIOB->DIR=0xFF;	 //Setting the direction IO pins as OUTPUT
  GPIOB->DEN = 0xFF; // digital enable portf all pins
	
}

void PORTF_init(){
	SYSCTL->RCGCGPIO|= (1<<5); //Enable clock to PORTF
	GPIOF->LOCK=0x4C4F434B;//unlock PORTF and configure for blue LED
	GPIOF->CR=0xFF; //unlock changes for all pins on DIR, DEN and other pullup and pulldown regisers
	GPIOF->AFSEL = 0x00; //disable alternate functions
	GPIOF->AMSEL = 0x00; //disable analog inputs
  GPIOF->DIR=0xEE;	 //Setting the direction IO pins PF_0 and PF_4 input; others output
	GPIOF->PUR=0x11;		//pullup for PF_0 and PF_4; these pins are onboard button pins and they will be configured for falling edge interrupts
  GPIOF->DEN = 0xFF; // digital enable portf all pins
	
	GPIOF->IS&= ~(1<<4)& ~(1<<0); // Make PF4 and PF0 edge sensitive 
	GPIOF->IBE&= ~(1<<4)& ~(1<<0);// clear the bits for corresponding PF4 and PF0 as we are using only the fallin edge triggers
	GPIOF->IEV&=~(1<<4)& ~(1<<0); // Make PF4 and PF0 falling edge triggered
	GPIOF->ICR|= (1<<4)|(1<<0); // Clear any previous interrupt at PF4 and PF0
	GPIOF->IM|= (1<<4)|(1<<0);  // unmask the interrupt for PF4 and PF0
	NVIC_SetPriority(GPIOF_IRQn, 3); // Assign priority as 3
	
	NVIC_EnableIRQ(GPIOF_IRQn); // enable interrupt requests for PORTF
	
}

void TIMER0A_init(){
	SYSCTL->RCGCTIMER |= 0x01; // enable use of timer0
	TIMER0 -> CFG = 0x00; //32 bit timer configuration
	TIMER0 ->CTL &= 0x01; // disable timer0 for further configuration
	TIMER0 -> TAMR |= 0x02; //count down and periodic
	TIMER0 -> TAILR = 100000;
	TIMER0 -> IMR = 0x01; //set interrupt mask for TIMER0
	TIMER0 -> CTL |= 0x01; // enable timer0
	NVIC_SetPriority(TIMER0A_IRQn, 2); // Assign priority as 2 in order to make the display continuous in case of a button interrupt
	
	
	NVIC_EnableIRQ(TIMER0A_IRQn); // enable interrupt requests for TIMER0A
}
