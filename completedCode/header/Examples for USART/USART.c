/* USART Examples for Atmega1284 */ 

/* Initializes the desired USART 
    Call this function before the while loop in main 
*/ 
void initUSART( unsigned char usartNum); 

//Example
    initUSART(0); //initializes USART0 
    initUSART(1); //initializes USART1

//Empties the UDR Register of the desired USART, this will cause the USART_HasRecived to return to false 

void USART_Flush (unsigned char usartNum); 

//example
    USART_Flush(0); // empties the UDR of USART0 

//Returns a non-zero number if the desired USART is ready to send data 
//Returns 0 if  the desired USART is NOT ready to send data 

unsigned char USARR_IsSendReady(unsigned char usartNum); 

//example 
    if (USARR_IsSendReady(0) )
    {
        //..send data 
    }

//Retuens a non-zero number if desired Usart has finished sending data 
//returns 0 if USART has is NOT finished sending data 

unsigned char USART_HasTransmitted (unsigned char usartNum ); 

//example 
    if ( USART_HasTransmitted(0) ) 
    {
        //..do something 
    }

//Returns a non-zero number if the desired USART has recieved a byte of data 
//returns 0 if the desired USART has NOT received a byte of data 

unsigned char USART_HasReceived (unsigned char usartNum); 

//example
    if( USART_HasReceived(1)) 
    {
        //..receive data 
    }

/* Writes a byte of data to the desired USARTs UDR register 
The data is sent serially over the TXD pin of the desired USART. 
Call this function adter USART_IsSendReady returns 1 
*/ 
void USART_Send(unsigned char data, unsigned char usartNum); 

//example
    unsigned char USART_Send(0xFF, 1); //send 0xFF on USART1

/* Retuens the data received on RXD pin of the desired USART 
Call this function after USART_HasReceived returns 1 
*/ 

unsigned char USART_Receive (unsigned char usartNum); 

//exmaple 
    unsigned char temp; 
        temp = USART_Receive(1) // write data recieved by USART1 to temp 