/*    Partner 1 Name & E-mail: Ryan Meoni (rmeon001@ucr.edu)
 *    Partner 2 Name & E-mail: James Moron (jmoro003@ucr.edu)
 *    Lab Section: 024
 *    Assignment: Lab 7  Recieve
 *   
 *    I acknowledge all content contained herein, excluding template or example
 *    code, is my own original work.
 */

 #define F_CPU 8000000UL //define microcontroller clock speed
 #include <avr/io.h>
 #include <stdio.h>
 #include "timer.h"
 #include "nokia5110.c"

int SlaveSelect = 4;
int MOSI = 5;
int MISO = 6;
int SCK = 7;

void SPI_SlaveInit(void)
{
	/* Set MISO output, all others input */
	DDRB = (1<<MISO);
	/* Enable SPI */
	SPCR = (1<<SPE);
}


char SPI_SlaveRecieve(void)
{
	while(!(SPSR & (1<<SPIF)))
	;
	return SPDR;
}

int main(void)
{
	DDRD = 0xFF; PORTD = 0x00; //Set all low to indicate startup
	DDRC = 0xFF; PORTC = 0x00; 
	SPI_SlaveInit(); 

	int number;
	int mod; 
	
	nokia_lcd_init(); 
	nokia_lcd_clear(); 
	nokia_lcd_set_cursor(0,10);
	
	/* Replace with your application code */
	while (1)
	{
		nokia_lcd_write_string("Avg Light ",1);
		number = SPI_SlaveRecieve(); 
		char str[20]; //create an empty string to store number
		sprintf(str, "%d", number);//make the number into string using sprintf function 
		nokia_lcd_set_cursor(0, 20);
		nokia_lcd_write_string("Intensity: ", 1); 
		nokia_lcd_write_string(str, 1);   
		nokia_lcd_render();
		_delay_ms(1000);
		nokia_lcd_clear();
	}
}