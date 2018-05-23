/**
	***************************************************************************
  * @file    	screen.c 														
  * @author  	Janaina Kaufmann #2326875, Sahin Tekes	#2288847			
  *        	  	HAW-Hamburg													
  *          	Labor für technische Informatik								
  *          	Berliner Tor  7												
  *          	D-20099 Hamburg												
  * @version 	V2.0														
  * @date    	17.04.2018													
  * @brief   	Main program body of the Reverse Polish notation calculator	
  ***************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <errno.h>
#include "screen.h"
#include "stack.h"
#include "TI_Lib.h"
#include "keypad.h"
#include "tft.h"

/* Defines -------------------------------------------------------------------*/
#define DELAYTIME		1000

//counter for user input (each symbol)
int inputCounter = 0;

//array for user input, sets a limit of 30 chars
char allInput[30];


/**
  * @brief  Prints the complete stack, uses a copy instead of original
  * @param  none
  * @retval none
  */
void
printStack() {
	int stackC[MAXSIZE];
	int pointerC;
	int i;
	
	char out[100];
	
	copyStack(stackC, &pointerC);
	
	TFT_gotoxy(1, 3);
	TFT_set_font_color(GREEN);
	TFT_puts("Stack: ");
	TFT_gotoxy(sizeof("Stack: "), 3);
	
	for (i = 0; i <= MAXSIZE - 1; i++) {
		printf("%d ", stackC[i]);
		sprintf(out, "%d ", stackC[i]);
		TFT_puts(out);
	}
	
	TFT_set_font_color(MINT);
}

/**
  * @brief  Prints the top (last) stack value
  * @param  none
  * @retval none
  */
void
printTopStack() {
	char out[100];
	
	TFT_gotoxy(1,2);
	TFT_set_font_color(BLUE);
	TFT_puts("Curr. Stack:");
	TFT_gotoxy(sizeof("Curr. Stack:") , 2); 
	
	sprintf(out, " %d", stackTop() );
	TFT_puts(out);
	TFT_set_font_color(MINT);
}

/**
  * @brief  Output on screen.
  * @param  user input
  * @retval none
  */
void
output(char input) {
	int i;
	
	if (input == 'e') {
		inputCounter = 30;
		TFT_cls();
	}
	
	if (inputCounter == 30) {
		for (i = 0; i <= (sizeof(allInput) - 1) ; i++) {
			allInput[i] = NULL;
		}
		
		inputCounter = 0;
	}
	
	if (input != 'f' && input != 'p') {
			TFT_cls();
	}
	
	TFT_gotoxy(1, 1);
	TFT_puts("Eingabe: ");
	TFT_gotoxy(sizeof ("Eingabe: "), 1);
	allInput[inputCounter] = input;
	TFT_puts(allInput);
	inputCounter++;
	
	if (input == 'f') {
		printStack();
	} else if (input == 'p') {
		printTopStack();
	}
}

/**
  * @brief  Process of initializing the TI-Board and text alignment
  * @param  none
  * @retval none
  */
void
initScreen() {
		Init_TI_Board();
		Make_Touch_Pad();
		TFT_set_window(GENEVA10, 1, 1, XPIXEL, 4);
		TFT_puts("Eingabe:");
}

/**
  * @brief  Error handling
  * @param  error code supplied by other functions
  * @retval none
  */
void
errorMsg(int error) {
	switch	(error){
			case -1:
				TFT_gotoxy(1,4);
				TFT_set_font_color(RED);
				TFT_puts("Stack is Empty!");
				TFT_set_font_color(MINT);
				Delay(DELAYTIME);
				break;
			
			case -2:
				TFT_gotoxy(1,4);
				TFT_set_font_color(RED);
				TFT_puts("Stack Overflow");
				TFT_set_font_color(MINT);
				Delay(DELAYTIME);
				break;
			
			case -3:
				TFT_gotoxy(1,4);
				TFT_set_font_color(RED);
				TFT_puts("Stack Underflow");
				TFT_set_font_color(MINT);
				Delay(DELAYTIME);
				break;
			
			case -4:
				TFT_gotoxy(1,4);
				TFT_set_font_color(RED);
				TFT_puts("Division with 0");
				TFT_set_font_color(MINT);
				Delay(DELAYTIME);
				break;
			
			default:
				break;
	}
}
