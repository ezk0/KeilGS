/**
	***************************************************************************
  * @file    	functions.c 														
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
#include "functions.h"
#include "stack.h"
#include "tft.h"
#include "TI_Lib.h"
#include "input.h"
#include "screen.h"

//Operand for calculations, gets value from pop()
int value;

/**
  * @brief  Elementary arithmetic
  * @param  none
  * @retval none
  */
void
add() {
	if(!getSet()) {
		push();
	}
	value = pop();
	
	if (value > 0) {
		setBuffer(value + pop());
		push();
	} else {
		errorMsg(value);
	}
}

void
sub() {
	if(!getSet()) {
		push();
	}
	value = pop();
	
	if (value > 0) {
		setBuffer(pop() - value);
		push();
	} else {
		errorMsg(value);
	}
}

void
mul(){
	if(!getSet()) {
		push();
	}
	value = pop();
	
	if (value > 0) {
		setBuffer(value * pop());
		push();
	} else {
		errorMsg(value);
	}
}

void
div() {
	if(!getSet()) {
		push();
	}
	value = pop();
	printf("%d", value);
	
	if(value == -3) {
		errorMsg(-3);
	}	else if(value != 0){	
		setBuffer(pop() / value);
		push();
	} else {
		errorMsg(-4);
	}
}

