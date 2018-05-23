/**
	***************************************************************************
  * @file    	input.c 														
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
#include "input.h"
#include "keypad.h"
#include "tft.h"
#include "stack.h"
#include "functions.h"
#include "screen.h"

/* Defines -------------------------------------------------------------------*/
#define TRUE	1
#define FALSE	0

//buffer for user input, prevents from writing directly to the stack
int buffer = 0;

//boolean that checks if buffer was pushed
unsigned int set = FALSE;

/**
  * @brief  Puts data on top of the Stack
  * @param  data that will be put on the stack
  * @retval none
  */
void
put(int data) {
	setSet(FALSE);
	buffer = (buffer * 10) + data;
}

/**
  * @brief  Gets value of buffer
  * @param  buffer value
  * @retval none
  */
int
getBuffer() {
	return buffer;
}

/**
  * @brief  Sets the buffer to a specific int-value
  * @param  value to set buffer with
  * @retval none
  */
void
setBuffer(int value) {
	setSet(FALSE);
	buffer = value;
}

/**
  * @brief  Gets if buffer was already pushed
  * @param  none
  * @retval none
  */
unsigned int
getSet() {
	return set;
}

/**
  * @brief  Sets the state of the push-checker
  * @param  true or false
  * @retval none
  */
void
setSet(unsigned int value) {
	set = value;
}

/**
  * @brief  Input of user is catched via switch-case
  * @param  none
  * @retval none
  */
void
getInput() {
	unsigned int status = TRUE;

	while (status == TRUE) {
		char eingabe = Get_Touch_Pad_Input();
		
		switch	(eingabe){
			case '0':
				put(eingabe - '0');
				break;
			
			case '1':
				put(eingabe - '0');
				break;
			
			case '2':
				put(eingabe - '0');
				break;
			
			case '3':
				put(eingabe - '0');
				break;
			
			case '4':
				put(eingabe - '0');
				break;
			
			case '5':
				put(eingabe - '0');
				break;
			
			case '6':
				put(eingabe - '0');
				break;
			
			case '7':
				put(eingabe - '0');
				break;
			
			case '8':
				put(eingabe - '0');
				break;
			
			case '9':
				put(eingabe - '0');
				break;
			
			case '+':
				add();
				break;
			
			case '-':
				sub();
			break;
			
			case '*':
				mul();
				break;
			
			case '/':
				div();
				break;
			
			case 'p':
				break;
			
			case 'f':
				break;
			
			case 'c':
				errorMsg(clearStack());
				break;
			
			case 'd':
				duplicate();
				break;
			
			case 'r':
				swapPosition();
				break;
			
			case ' ':
				eingabe = '_';
				errorMsg(push());
				break;
			
			case 'e':
				break;
			
			default:
				continue;
		}
		
		output(eingabe);
	}
}
