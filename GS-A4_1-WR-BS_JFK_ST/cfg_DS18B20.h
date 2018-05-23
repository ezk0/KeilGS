#ifndef	_CFG_DS18B20CONFIG_H
#define	_CFG_DS18B20CONFIG_H


//	Init timer on cube    1us per tick				example 72 MHz cpu >>> Prescaler=(72-1)      counter period=65000 
//###################################################################################
#define _DS18B20_MAX_SENSORS 7
#define	_DS18B20_GPIO GPIOG
#define IDR_MASK_PIN_0 (0x01U << 0)
#define	_DS18B20_PIN GPIOG->IDR = GPIOG->IDR &~IDR_MASK_PIN_0;


#define	_DS18B20_UPDATE_INTERVAL_MS 10000 //  (((	if==0  >> Ds18b20_ManualConvert()  )))    ((( if>0  >>>> Auto refresh )))
#define	_DS18B20_CONVERT_TIMEOUT_MS 5000		

#define	_DS18B20_TIMER htim6						
//###################################################################################

#endif
