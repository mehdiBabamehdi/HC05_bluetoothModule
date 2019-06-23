/*
 * HC05_bt.c
 *
 * Created: 10/4/2016 9:09:52 PM
 * Ver: 1.0
 * Final Edited: 10/28/2016
 * Author : PC
 */ 

#define F_CPU 11059200UL
//#define F_CPU 7372800UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "Gen_Def.h"
#include "UART_3.h"
#include "LCD.h"

void ADC_Init(void);
char* ADCon(void);
void Data_Extender (uint16_t input_int);

char* String="Temperature = ";
char* String2=" C";		// SELECT THE UNIT OF TEMP. MEASUREMENT C: CELSIUS, F: FAURENHEID, K: KELVIN

char Send_Char[4];

int main(void)
{	
	char* HC05_Get_Data;
	char* Room_Temp;
	char* Send_Data;

/************************************************************************/
/*                         Set up the uC	                            */
/************************************************************************/																	

	ADC_Init();	// Initialize ADC
	dely_s(2);
	
	USART_Initialization(9600,8,NONE,1,0);	// Initialize the UART; Baud Rate=9.6k, 8-byte data size, No parity, one stop bit, disable Double Speed in Asynchronization
	USART_Interrupt_Int(TRUE,FALSE,FALSE);		// Initialization of USART Interrupt (RXC, TXC, UDRE)
	dely_s(2);
	
	LCDInit(LS_BLINK|LS_ULINE);	// Initialize LCD module, LCD Blink & Cursor is "underline" type
	dely_s(2);
	
	DDRB  |= 1 << PINB0 | 1 << PINB1 | 1 << PINB2 | 1 << PINB3 | 1 << PINB4;
	PORTB &= ~(1 << PINB0) & ~(1 << PINB1) & ~(1 << PINB2) & ~(1 << PINB3) & ~(1 << PINB4);

	
	LCDWriteStringXY(4,1,"Hello World!");
	dely_s(longdelay);
	LCDClear();
	
		
	while(1)
	{
		HC05_Get_Data = USART_Receive_String_ISR();
		_delay_ms(2000);
		LCDClear();
		LCDWriteStringXY(0,0,HC05_Get_Data);
				
		if ((strncmp(HC05_Get_Data,"RT",2)) == 0)
		{			
	
			Room_Temp = ADCon();	// Reading Room Temp.
			_delay_ms(1000);
			Send_Data = Concat("Room Temp. = ",Room_Temp);
			Send_Data = Concat(Send_Data," C \r\n");
			
			LCDWriteStringXY(0,1,Send_Data);
			
			USART_Transmit_String(Send_Data);
			_delay_ms(1000);

		}
		else if ((strncmp(HC05_Get_Data,"L1ON",4)) == 0)
		{			
			LCDWriteStringXY(0,1,"LIGHT 1 ON");
			PORTB |= (1 << PINB1);
			
		}
		else if ((strncmp(HC05_Get_Data,"L1OFF",5)) == 0)
		{
			LCDWriteStringXY(0,1,"LIGHT 1 OFF");
			PORTB &= ~(1 << PINB1);
		}
		else if ((strncmp(HC05_Get_Data,"L2ON",4)) == 0)
		{
			LCDWriteStringXY(0,1,"LIGHT 2 ON");
			PORTB |= (1 << PINB2);
		}
		else if ((strncmp(HC05_Get_Data,"L2OFF",5)) == 0)
		{
			LCDWriteStringXY(0,1,"LIGHT 2 OFF");
			PORTB &= ~(1 << PINB2);
		}
	}	
}

/**
 * Name: ADC_Init
 * Description: The Routine Set Initial Conditions for ADC
 * @Author: Mehdi
 */
void ADC_Init(void)		
{
	ADCSRA |= 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0; // ADC PRESCALAR; CK/128
	ADMUX  |= 1<<REFS0;		// AREF = AVcc
	ADMUX  |= 1<<ADLAR;		// SET 8-BIT RESOLUTION
	ADCSRA |= 1<<ADEN;		// ENABLE ADC
}

/**
 * Name: ADCon
 * Description: The Routine Read the Temp. from Sensor and Convert Analog to Digital, 10-bit, left-justified
 * @Author: Mehdi
 *
 * @Params	tenBitvalue: the temp. sensed by analog sensor and convert to digital
 * @Return	Temp_Char: 10-bit temperature in string
 */
char* ADCon(void)		
{	
	uint16_t ADCTemp;
	int Data_Accuracy = 4;
	char* Temp_Char = malloc(Data_Accuracy + 1);
	
	ADCSRA |= 1 << ADSC;	// Start Conversion
	
	while(!(ADCSRA & (1 << ADIF)));	// Wait to complete ADC
	
	uint8_t theLow = ADCL;
	uint16_t tenBitvalue = ADCH << 2 | theLow >> 6;
	
	ADCTemp = tenBitvalue*0.432;	// ADC*Vref/2^bit_size/Temp. scale(LM35 Temp. scale = 10 mV/C)
	itoa(ADCTemp,Temp_Char,10);		// Convert integer to string
	Temp_Char[4] = '\0';
	
	return(Temp_Char);
}



/**
 * Name: Data_Extender
 * Description: The routine extend the data should be sent to six characters, 
 *				first part of the data is the same of "input", and the rest is filled with "#"
 * @Author: Mehdi
*/

/*
void Data_Extender (uint16_t input_int)
{
	
	long int factor = 1;
	int i,j;
	
	int length_data = log10(input_int) + 1;
	
	for (i = length_data; i >0  ; i--){
		factor = 1;
		for (j = 1; j < i; j++){
			factor = factor * 10;
		}
		
		Send_Char[length_data - i + 1] = abs(input_int/factor);
		input_int = input_int % factor;
	}
	
	for (i = length_data + 1; i < 7; i++){
		Send_Char[i] = '#';
	}
}
*/
