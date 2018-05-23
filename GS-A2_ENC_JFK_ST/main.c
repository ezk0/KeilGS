#include <stdio.h>
#include "TI_Lib.h"
#include "encoder.h"


//#include "stm32f4xx.h"
//#include "stm32f4xx_gpio.h"
//#include "stm32f4xx_rcc.h"



//switch
int main(void) {
	
	//for simulation
	Init_TI_Board();
	
	//begin of super loop
	while (1) {
			encode();
	}
	// end of super loop
} // end of main
// EOF
