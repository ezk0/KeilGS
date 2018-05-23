#include "one_wire.h"
#include "cfg_DS18B20.h"
#include "timer.h"
#include <stm32f4xx.h>

///////////////////////////////////////////////////////////////////////////

void ow_delay(uint16_t uSec){
	timerinit();
	uint32_t ticks = uSec * CONVERT2US;
	int startTicks = getTimeStamp();
	while(getTimeStamp() <= startTicks + ticks){
	//whatcha lookin at, dog?
	}
}

void ow_low(onewire_t *gp){
	gp->GPIOx->BSRRH = (1 << 0);	
}

void ow_high(onewire_t *gp){
	gp->GPIOx->BSRRL = (1 << 0);
}

void ow_input(onewire_t *gp){
	GPIO_InitTypeDef gpinit;
	gpinit.GPIO_Mode = GPIO_Mode_IN;
	gpinit.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpinit.GPIO_Speed = GPIO_Speed_50MHz;
	gpinit.GPIO_Pin = gp->GPIO_Pin;
	GPIO_Init(gp->GPIOx,&gpinit);
}

void ow_output(onewire_t *gp){
	GPIO_InitTypeDef gpinit;
	gpinit.GPIO_Mode = GPIO_Mode_OUT;
	gpinit.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpinit.GPIO_Speed = GPIO_Speed_50MHz;
	gpinit.GPIO_Pin = gp->GPIO_Pin;
	GPIO_Init(gp->GPIOx,&gpinit);
}

void ow_init(onewire_t* ow_struct, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
	timerinit();
	ow_struct->GPIOx=GPIOx;
	ow_struct->GPIO_Pin=GPIO_Pin;
	ow_output(ow_struct);
	ow_high(ow_struct);
	Delay(1000);
	ow_low(ow_struct);
	Delay(1000);
	ow_high(ow_struct);
	Delay(2000);
}
	
///////////////////////////////////////////////////////////////////////////

uint8_t
reset () {
	uint8_t status;
	
	LOW;
	Tx_DS18B20;
	Delay(480000);
	Rx_DS18B20;
	Delay(70000);
	
	status = GPIO_ReadInputDataBit(GPIOG, GPIOG->IDR);
	Delay(410000);
	
	return status;
}

void
write_1_0 (uint8_t bit) {
	if (bit) {
		LOW;
		Tx_DS18B20;
		Delay(6000);
		Rx_DS18B20;
		Delay(64000);
	} else {
		LOW;
		Tx_DS18B20;
		Delay(60000);
		Rx_DS18B20;
		Delay(10000);
	}
}

uint8_t
readBIT () {
	uint8_t bit = 0;
	
	LOW;
	Tx_DS18B20;
	Delay(6000);
	Rx_DS18B20;
	Delay(9000);
	
	if (GPIO_ReadInputDataBit(GPIOG, GPIOG->IDR)) {
		bit = 1;
	}
	
	Delay(55000);
	
	return bit;
}
