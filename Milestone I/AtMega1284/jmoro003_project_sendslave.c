// Partner(s) Name & E-mail: Ryan Meoni (rmeon001@ucr.edu) James Moron (jmoro003@ucr.edu)
//	Lab Section: 024
//	Assignment: Lab 7  Send
//
//
//	I acknowledge all content contained herein, excluding template
// 	or example code, is my own original work.


#include <avr/io.h>
#include <timer.h>
#include "nokia5110.c"
#include <bit.h>
#include <stdio.h>

void SPI_MasterInit()
{
	int SlaveSelect = 4;
	int MOSI = 5;
	int MISO = 6;
	int SCK = 7;
	DDRB |= (1<<MOSI);//| (1<<SlaveSelect) ; //| (1<<SCK);// | (1<<SlaveSelect) ;
	SPCR = (1<<SPE);//| (1<<MSTR) | (1<<SPR0);
}

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	// in Free Running Mode, a new conversion will trigger
	// whenever the previous conversion completes.
}

void SPI_MasterTransmit(char cData)
{
	char clear_buffer;
	SPDR = cData;
	while (!(SPSR & (1<<SPIF)))
	;
	clear_buffer = SPDR;
}

int main(void)
{
	DDRC = 0xF0;
	PORTC = 0x0F;
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00; 
	//unsigned char transmittingData = 0; 
	ADC_init();
	//Initialize Master
	TimerSet(1000); 
	TimerOn();
	SPI_MasterInit();
	
		//SPI_MasterTransmit(transmittingData);
	
	//Set initial states

	while (1)
	{
		//transmittingData = ADC; 
		SPI_MasterTransmit(ADC); 
		  
		while (!TimerFlag) {} //Guess I don't need a timer
		TimerFlag = 0;
	}
	
}
