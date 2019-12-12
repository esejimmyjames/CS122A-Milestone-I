/*    Partner 1 Name & E-mail: James Moron (jmoro003@ucr.edu)
 *    Assignment: Final Project - CS122A Fall 2019
 *   
 *    I acknowledge all content contained herein, excluding template or example
 *    code, is my own original work.
 */

 #define F_CPU 8000000UL //define microcontroller clock speed
 #include <avr/io.h>
 #include <stdio.h>
 #include "timer.h"
 #include "io.c"
 #include "stdutils.h"
 #include "delay.c"
 #include "usart_ATmega1284.h"

 
 
 /*SPI*/
 void SPI_MasterInit()
 {
	 int SlaveSelect = 4;
	 int MOSI = 5;
	 int MISO = 6;
	 int SCK = 7;
	 DDRB |= (1<<MOSI);//| (1<<SlaveSelect) ; //| (1<<SCK);// | (1<<SlaveSelect) ;
	 SPCR = (1<<SPE);//| (1<<MSTR) | (1<<SPR0);
 }
 
 void SPI_MasterTransmit(char cData)
 {
	 char clear_buffer;
	 SPDR = cData;
	 while (!(SPSR & (1<<SPIF)))
	 ;
	 clear_buffer = SPDR;
 }
 /**/
 
 /*Other ADC*/
 /***************************************************************************************************
                         void ADC_Init()
****************************************************************************************************
 * I/P Arguments: none.
 * Return value	: none

 * description :This function initializes the ADC module.

***************************************************************************************************/
void ADC_Init()
 {
   ADCSRA=(1<<ADEN) | (1<<ADPS0); /* Enable ADC , sampling freq=osc_freq/2 */
   ADMUX=0x00;                    /* Result right justified, select channel zero */
 }


  
  
  
/***************************************************************************************************
                         uint16_t ADC_GetAdcValue(uint8_t v_adcChannel_u8)
****************************************************************************************************
 * I/P Arguments: uint8_t(channel number).
 * Return value	: uint16_t(10 bit ADC result)

 * description  :This function does the ADC conversion for the Selected Channel
                 and returns the converted 10bit result
                 The adc value per bit depends on the resolution of the ADC.
				 For AVR/PIC(10-bit adc) the adc value per lsb will be 5/1023=0048v
***************************************************************************************************/				 
uint16_t ADC_GetAdcValue(uint8_t v_adcChannel_u8)
 {
   
   ADMUX = v_adcChannel_u8;               /* Select the required channel */
   DELAY_us(10);                          /* Wait for some time for the channel to get selected */
    
   util_BitSet(ADCSRA,ADSC);              /* Start the ADC conversion by setting ADSC bit */
   
   while(util_IsBitCleared(ADCSRA,ADIF)); /* Wait till the conversion is over */
                                          /* ADIF will be set once ADC conversion is complete */
     return(ADCW);                        /* Return the 10-bit result */
 }
 
 void DELAY_sec(uint16_t sec_count)
 {
	 while (sec_count != 0) {
		 DELAY_ms(1000); //DELAY_ms is called to generate 1sec delay
		  
		 sec_count--;
	 }
 }
 
 
 #define         RL_VALUE                     (10)     //define the load resistance on the board, in kilo ohms
 #define         RO_CLEAN_AIR_FACTOR          (9.83)  //(Sensor resistance in clean air)/RO,
 //which is derived from the chart in datasheet
 
 #define         LPG                      (0)         // Gas identity no.
 #define         SMOKE                    (1)
 

 float           LPGCurve[3]  = {2.3,0.20,-0.45};   //two points from LPG curve are taken point1:(200,1.6) point2(10000,0.26)
 //take log of each point (lg200, lg 1.6)=(2.3,0.20)  (lg10000,lg0.26)=(4,-0.58)
 //find the slope using these points. take point1 as reference
 //data format:{ x, y, slope};

 float           SmokeCurve[3] ={2.3,0.53,-0.43};    //two points from smoke curve are taken point1:(200,3.4) point2(10000,0.62)
 //take log of each point (lg200, lg3.4)=(2.3,0.53)  (lg10000,lg0.63)=(4,-0.20)
 //find the slope using these points. take point1 as reference
 //data format:{ x, y, slope};
 
 float           Ro           =  10;                 //Ro is initialized to 10 kilo ohms
 
 
 int  GetPercentage(float rs_ro_ratio, float *pcurve);
 int GetGasPercentage(float rs_ro_ratio, int gas_id);
 float ReadSensor();
 float ResistanceCalculation(int raw_adc);
 //float SensorCalibration();
 



 float ResistanceCalculation(int raw_adc)
 {                                                         // sensor and load resistor forms a voltage divider. so using analog value and load value
	 return ( ((float)RL_VALUE*(1023-raw_adc)/raw_adc));     // we will find sensor resistor.
 }
 


 float SensorCalibration()
 {
	 int i;                                   // This function assumes that sensor is in clean air.
	 float val=0;
	 
	 for (i=0;i<50;i++) {                   //take multiple samples and calculate the average value
		 
		 //val += ResistanceCalculation(ADC_GetAdcValue(0));
		 val += ResistanceCalculation(ADC);
		 DELAY_ms(500);
		 
	 }
	 val = val/50;
	 val = val/RO_CLEAN_AIR_FACTOR;                        //divided by RO_CLEAN_AIR_FACTOR yields the Ro
	 //according to the chart in the datasheet
	 
	 return val;
 }

 
 float ReadSensor()
 {
	 int i;
	 float rs=0;
	 
	 for (i=0;i<5;i++) {                                 // take multiple readings and average it.
		 rs += ResistanceCalculation(ADC_GetAdcValue(0));   // rs changes according to gas concentration.
		 DELAY_ms(50);
	 }
	 
	 rs = rs/5;
	 
	 return rs;
 }
 

 int GetGasPercentage(float rs_ro_ratio, int gas_id)
 {
	 if ( gas_id == LPG ) {
		 return GetPercentage(rs_ro_ratio,LPGCurve);
		 
		 } else if ( gas_id == SMOKE ) {
		 return GetPercentage(rs_ro_ratio,SmokeCurve);
	 }
	 
	 return 0;
 }
 
 
 int  GetPercentage(float rs_ro_ratio, float *curve)
 {                                                                          //Using slope,ratio(y2) and another point(x1,y1) on line we will find
	 return (pow(10,( ((log(rs_ro_ratio)-curve[1])/curve[2]) + curve[0])));   // gas concentration(x2) using x2 = [((y2-y1)/slope)+x1]
	 // as in curves are on logarithmic coordinate, power of 10 is taken to convert result to non-logarithmic.
 }
 
 
 /**/

/*ADC*/
void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	// in Free Running Mode, a new conversion will trigger
	// whenever the previous conversion completes.
}
/**/

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

void clear(){
				LCD_DisplayString(1, "");
				while (!TimerFlag) {} //Guess I don't need a timer
				TimerFlag = 0;
}

void wait(){
	while (!TimerFlag) {} //Guess I don't need a timer
	TimerFlag = 0;
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRD = 0xFF; PORTD = 0x00; // LCD data lines
	DDRB = 0xFF; PORTB = 0x00; // LCD control lines
	
	
	initUSART(1);
	//USART_Flush(0);
	
	ADC_init();
	//Initialize Master
	TimerSet(1000);
	TimerOn();
	
	unsigned int data = 0; 
	 
	char str[40]; //create an empty string to store number
	char otherstr[200]; 
	// Initializes the LCD display
	LCD_init();
		//TimerSet(1000);
		//TimerOn();
		SPI_MasterInit();

	LCD_DisplayString(1, "Calibrating...");
	Ro = SensorCalibration();                       //Please make sure the sensor is in clean air
	clear();
	LCD_DisplayString(1, "Calibration is done...");
	clear(); 
	wait();
	//LCD_DisplayString(1, "Ro= ");
	sprintf(str, "Ro= %d kohm", Ro);//make the number into string using sprintf function
	LCD_DisplayString(1, str);
	wait(); 
	wait(); 
	clear(); 
	//UART_Printf("\n\r");
	while(1) {
			//LPG = ReadSensor()/Ro; 
			//SMOKE = ReadSensor()/Ro;
			//SPI_MasterTransmit(ADCW);
			//wait();  
			sprintf(otherstr, "LPG: %d ppm SMOKE: %d ppm", GetGasPercentage(ReadSensor()/Ro,LPG), GetGasPercentage(ReadSensor()/Ro,SMOKE));//make the number into string using sprintf function
			LCD_DisplayString(1, otherstr); 
			if ( USART_IsSendReady(1) ) {
				for(int i = 0; i < strlen(otherstr); i++){
					USART_Send(otherstr[i], 1);
				}
				USART_Send('\n',1);
			}
			
			wait();
			//clear(); 	 
	}

}