/**
	***************************************************************************
  * @file    	stack.c 														
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
#include "stack.h"
#include "TFT.h"
#include "TI_Lib.h"
#include "input.h"

/* Defines -------------------------------------------------------------------*/
#define TRUE	1
#define FALSE 	0

//int-array that realizes the stack
int stack[MAXSIZE];

//stackpointer
int pointer = -1;

/**
  * @brief  Test if stack is empty
  * @param  none
  * @retval 1 true or 0 false 
  */
unsigned int
Empty() {
	if (pointer < 0) {
		
		return TRUE;
	}
	
	return FALSE;
}

/**
  * @brief  Test if the stack is full
  * @param  none
  * @retval 1 true or 0 false 
  */
unsigned int
Full() {
	if (pointer == MAXSIZE - 1) {
		return TRUE;
		
	}
	
	return FALSE;
}

/**
  * @brief  Shows the pointer value from stack 
  * @param  none
  * @retval pointer stack value
  */
int
stackTop() {
	if (!Empty() ) {
		
		return stack[pointer];
	}
	
	return FALSE;
}

/**
  * @brief  Copies the value from stack at index pointer
  * @param  none
  * @retval data from the pointer of the stack
  * 		errcode -3: Stack Underflow
  */
int
pop() {
	int data = 0;
	
	if (!Empty() ) {
		data = stackTop();
		stack[pointer] = 0;
		pointer--;
		
		return data;
	} else {
		
		return -3;
	} 
}

/**
  * @brief  Writes value of buffer to stack
  * @param  none
  * @retval data from the pointer of the stack
  * 		errcode -2: Stack Overflow
  */
int
push() {
	if (!Full() ) {
		pointer++;
		stack[pointer] = getBuffer();
		setBuffer(0);
		setSet(TRUE);

		return TRUE;
	} else {
		
		return -2;
	}
}

/**
  * @brief  Wipes the stack
  * @param  none
  * @retval true
  * 		errcode -1: Empty Stack
  */
int
clearStack() {
	if (!Empty() ) {
		int i;
		for (i = 0; i < MAXSIZE; i++) {
			stack[i] = 0;
		}
		pointer = -1;
		setBuffer(0);
		
		return TRUE;
	} else {
		
		return -1;
	}
}

/**
  * @brief  Copies the complete stack for further operations
  * @param  Temp stack array, temp stackpointer
  * @retval none
  */
void
copyStack(int * cStack, int * pStack) {
	int i;
	
	for (i = 0; i < MAXSIZE; i++) {
		*(cStack++) = stack[i];
	}
	*pStack = pointer;
}

/**
  * @brief  Swaps the position of the last two stack values
  * @param  none
  * @retval none
  */
void
swapPosition() {
	int value = stackTop();
	stack[pointer] = stack[pointer - 1];
	stack[pointer - 1] = value;
}

/**
  * @brief  Duplicates the last entry of the stack
  * @param  none
  * @retval none
  */
void
duplicate() {
	int value = stackTop();
	setBuffer(value);
	push();
}
