/**
  ******************************************************************************
  * @file    	main.c 
  * @author  	Alfred Lohmann
  *        	  HAW-Hamburg
  *          	Labor für technische Informatik
  *          	Berliner Tor  7
  *          	D-20099 Hamburg
  * @version V1.0
  * @date    23.05.2013
  * @brief   Main program body
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "owire.h"
#include "tft.h"

/**
  * @brief  Main program
  * @param  None
  */
int main(void)
{
	Init_TI_Board();
Owire ow;
unsigned char DeviceAddress[8];
int nCount = 0;
 
OWinit(&ow);
 
 char out[100];
OWsearch(&ow, DeviceAddress);


	
	
	sprintf(out,"Device %d: ", nCount+1);
    nCount++;
	TFT_puts(out);

	for(int i=0; i < 8; i++){
		char rom[1];
		sprintf(rom, "%02X",ow.ROM_NO[i]);
		TFT_puts(rom);
	}
	

	

  return 0;

}
// EOF
