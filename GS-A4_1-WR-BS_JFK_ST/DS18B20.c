#include "DS18B20.h"

DS18B20_Sensor DS18B20[DS18B20_MAX_SENSORS];

OW_t ONEWIRE;
uint8_t ONEWIRE_DEVICES;
uint8_t SensorCount = 0;
uint8_t DS18B20_START_CONVERT = 0;
uint16_t DS18B20_TIMEOUT = 0;

uint8_t DS18B20_FAMCHECK(uint8_t *ROM) 
{
	/* Checks if first byte is equal to DS18B20's family code */
	if (*ROM == FAMILY_ROM){
		return 1;
	}
	return 0;
}

uint8_t DS18B20_START(OW_t *ow_struct, uint8_t *ROM){
	
	/* check if device is DS18B20 */
	if(DS18B20_FAMCHECK(ROM)){
		return 0;
	}
	
	/* reset line */
	OW_RST(ow_struct);
	
	/* select ROM */
	OW_SELECTwPOINTER(ow_struct, ROM);
	
	/* start temp conversion */
	OW_WRITEBYTE(ow_struct, DS18B20_CONVERTTEMP);
	
	return 1;
}

void DS18B20_STARTALL(OW_t *ow_struct){
	
	/* reset pulse */
	OW_RST(ow_struct);
	
	/* skip ROM */
	OW_WRITEBYTE(ow_struct, SKIPROM);
	
	/* start conversion on all connected sensors */
	OW_WRITEBYTE(ow_struct, DS18B20_CONVERTTEMP);
}

bool DS18B20_READ(OW_t *ow_struct, uint8_t *ROM, float *dest){
	uint16_t temperature;
	uint8_t resolution;
	int8_t digit, minus = 0;
	float decimal;
	uint8_t i = 0;
	uint8_t data[9];
	uint8_t crc;
	
	/* device check */
	if(!DS18B20_FAMCHECK(ROM)){
		return false;
	}
	
	/* check if line is released */
	if(!OW_READBIT(ow_struct)){
		
		/* conversion not finished by now */
		return false;
	}
	
	/* reset line */
	OW_RST(ow_struct);
	
	/* select ROM */
	OW_SELECTwPOINTER(ow_struct, ROM);
	
	/* read Scratchpad command */
	OW_WRITEBYTE(ow_struct, RSCRATCHPAD);
	
	/* get data */
	for(i=0; i<9; i++){
		
		/* byte by byte */
		data[i] = OW_READBYTE(ow_struct);
	}
	
	/* calculate CRC */
	crc = OW_CRC(data, 8);
	
	/* CRC check */
	if(crc !=data[8]){
		
		/* CRC invalid */
		return 0;
	}
	
	/* first two bytes of Scratchpad are temp values */
	temperature = data[0] | (data[1] << 8);
	
	/* reset line */
	OW_RST(ow_struct);
	
	/* check if temp is negative */
	if(temperature & 0x8000){
		
		/* complement of 2, temperature is neg */
		temperature = ~temperature +1;
		minus = 1;
	}
	
	/* get sensor resolution */
	resolution = ((data[4] & 0x60) >> 5) + 9;
	
	/* store int digits of temperature */
	digit = temperature >> 4;
	digit |= ((temperature >> 8) & 0x7) << 4;
	
	/* store decimal digitis */
	switch(resolution){
		case 9:
			decimal = (temperature >> 3) & 0x01;
			decimal *= (float)DS18B20_DECIMAL_STEPS_9BIT;
			break;
		case 10:
			decimal = (temperature >> 2) & 0x03;
			decimal *= (float)DS18B20_DECIMAL_STEPS_10BIT;
			break;
		case 11:
			decimal = (temperature >> 1) & 0x07;
			decimal *= (float)DS18B20_DECIMAL_STEPS_11BIT;
			break;
		case 12:
			decimal = temperature & 0x0F;
			decimal *= (float)DS18B20_DECIMAL_STEPS_12BIT;
			break;
		default:
			decimal = 0xFF;
			digit = 0;
	}
	
	/* check for negatives */
	decimal = digit + decimal;
	if(minus){
		decimal = 0-decimal;
	}
	
	/* set to pointer */
	*dest = decimal;
	
	/* temperature valid */
	return true;
}

uint8_t DS18B20_GETRESOLUTION(OW_t *ow_struct, uint8_t *ROM){
	uint8_t conf;
	
	if(!DS18B20_FAMCHECK(ROM)){
		return 0;
	}
	
	/* reset line */
	OW_RST(ow_struct);
	
	/* select ROM */
	OW_SELECTwPOINTER(ow_struct, ROM);
	
	/* read Scratchpad */
	OW_WRITEBYTE(ow_struct, RSCRATCHPAD);
	
	/* skip first 4 bytes */
	OW_READBYTE(ow_struct);
	OW_READBYTE(ow_struct);
	OW_READBYTE(ow_struct);
	OW_READBYTE(ow_struct);
	
	/* 5th byte of Scratchpad is config register */
	conf = OW_READBYTE(ow_struct);
	
	/* return 9-12 value according to number of bit */
	return ((conf & 0x60) >> 5) + 9;
}

uint8_t DS18B20_SETRESOLUTION(OW_t *ow_struct, uint8_t *ROM, DS18B20_Resolution_t resolution){
	uint8_t tH, tL, conf;
	if(DS18B20_FAMCHECK(ROM)){
		return 0;
	}
	
	/* reset line */
	OW_RST(ow_struct);
	
	/* select ROM */
	OW_SELECTwPOINTER(ow_struct, ROM);
	
	/* read Scratchpad */
	OW_WRITEBYTE(ow_struct, RSCRATCHPAD);
	
	/* ignore first 2 bytes */
	OW_READBYTE(ow_struct);
	OW_READBYTE(ow_struct);
	
	tH = OW_READBYTE(ow_struct);
	tL = OW_READBYTE(ow_struct);
	conf = OW_READBYTE(ow_struct);
	
	
	switch(resolution){
		case DS18B20_Resolution_9bits:
			conf &= ~(1 << DS18B20_RESOLUTION_R1);
			conf &= ~(1 << DS18B20_RESOLUTION_R0);
			break;
		case DS18B20_Resolution_10bits:
			conf &= ~(1 << DS18B20_RESOLUTION_R1);
			conf |= 1 << DS18B20_RESOLUTION_R0;
			break;
		case DS18B20_Resolution_11bits:
			conf |= 1 << DS18B20_RESOLUTION_R1;
			conf &= ~(1 << DS18B20_RESOLUTION_R0);
			break;
		case DS18B20_Resolution_12bits:
			conf |= 1 << DS18B20_RESOLUTION_R1;
			conf |= 1 << DS18B20_RESOLUTION_R0;
			break;
		default:
			conf = 0;
	}
	
	/* reset line */
	OW_RST(ow_struct);
	
	/* select ROM */
	OW_SELECTwPOINTER(ow_struct, ROM);
	
	/* write Scratchpad */
	OW_WRITEBYTE(ow_struct, WSCRATCHPAD);
	
	/* write bytes */
	OW_WRITEBYTE(ow_struct, tH);
	OW_WRITEBYTE(ow_struct, tL);
	OW_WRITEBYTE(ow_struct, conf);
	
	/* reset line */
	OW_RST(ow_struct);
	
	/* select ROM */
	OW_SELECTwPOINTER(ow_struct, ROM);
	
	/* copy scratchpad to EEPROM */
	OW_WRITEBYTE(ow_struct, CPYSCRATCHPAD);
	
	return 1;
}
