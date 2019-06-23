#ifndef GeneralDefinition
#define GeneralDefinition

#include <avr/io.h>
#include <string.h>
#include <stdlib.h>


// Characters
#define FALSE 0
#define TRUE  1

#define shortdelay 1
#define longdelay 3

/**********************************************************************
*							Type definition Block
***********************************************************************/
typedef char			C8;
typedef unsigned char   U8;
typedef signed char		S8;
typedef unsigned short	U16;
typedef signed short	S16;
typedef unsigned long	U32;
typedef signed long		S32;

// Pointers
typedef char*				C8_ptr;
typedef unsigned char*		U8_ptr;
typedef signed char*		S8_ptr;
typedef unsigned short*		U16_ptr;
typedef signed short*		S16_ptr;
typedef unsigned long*		U32_ptr;
typedef signed long*		S32_ptr;


/**********************************************************************
*							 Bitwise Operations
***********************************************************************/
#define NOT(x)   (~(x))
#define OR(x,y)  ((x) | (y))
#define AND(x,y) ((x) & (y))
#define NOR(x,y) ((x) | (~(y)))
#define XOR(x,y) ((x) ^ (y))


// Bitwise Assignment Operations
#define BIT(x)         (1 << (x))

#define SETBIT(x,y)   ((x) |= (y))
#define CLEARBIT(x,y) ((x) &= (~(y)))
#define EXCLBIT(x,y)  ((x) ^= (y))

#define SETBITS(x,y)    (SETBITS((x),(BIT(y))))
#define CLEARBITS(x,y)  (CLEARBITS((x),(BIT(y))))
#define EXCLBITS(x,y)   ((x) ^= (BIT(y)))

#define QUERYBITS(x,y)  ((x) & (BIT(y)))

/**********************************************************************
*							 General Routines
***********************************************************************/

/*
 * Description: The Routine imposes delay in second 
 * @Author: Mehdi
 *
 * @Params	time: intended delay time in second
 */ 

void dely_s(uint16_t time) 
{
	for (int i = 0 ; i < time ; i++) {
		_delay_ms(1000);
	}
}


/**
 * Name: Concat
 * Description: The Routine Concatenates two strings.
 * @Author: Mehdi
 *
 * @Params	strN; the strings that Concatenated in the function
 * @Return	result: the string results from Concatenation of the received string	
 */
char* Concat(char* str1, char* str2)
{
	char* result = malloc(strlen(str1)+strlen(str2)+1);	// +1 for the zero-terminator
 
	strcpy(result, str1);
	strcat(result, str2); 
	
	return (result);
}

#endif 