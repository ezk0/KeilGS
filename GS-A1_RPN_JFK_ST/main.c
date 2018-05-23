/**
	***************************************************************************
  * @file    	main.c 														
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
#include "screen.h"
#include "input.h"

/**
  * @brief  Main program
  * @param  None
  */
int
main(void) {
	initScreen();
	getInput();
	
	return 0;
}
