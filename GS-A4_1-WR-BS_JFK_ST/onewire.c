#include <stm32f4xx.h>
#include "onewire.h"
#include "timer.h"
#include "TI_Lib.h"
#include "stm32f4xx_gpio.h"
#include "DS18B20.h"

void OW_DELAY(uint16_t TIME_uS){
	resetTimer();
	while(getTimeStamp() <= TIME_uS*CONVERT2US);
}

void OW_LOW(OW_t *ow){
	ow->GPIOx->BSRRH = ow->GPIO_Pin<<16;
}

void OW_HIGH(OW_t *ow){
	ow->GPIOx->BSRRH = ow->GPIO_Pin;
}

void OW_INPUT(OW_t *ow){
	GPIO_InitTypeDef ow_init;
	ow_init.GPIO_Mode = GPIO_Mode_IN;
	ow_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	ow_init.GPIO_Speed = GPIO_Speed_50MHz;
	ow_init.GPIO_Pin = DS18B20_PIN;
	GPIO_Init(DS18B20_GPIO, &ow_init);
}

void OW_OUTPUT(OW_t *ow){
	GPIO_InitTypeDef ow_init;
	ow_init.GPIO_Mode = GPIO_Mode_OUT;
	ow_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	ow_init.GPIO_Speed = GPIO_Speed_50MHz;
	ow_init.GPIO_Pin = DS18B20_PIN;
	GPIO_Init(DS18B20_GPIO, &ow_init);
}

void OW_INIT(OW_t *ow_struct){
	timerinit();
	
	ow_struct->GPIOx = DS18B20_GPIO;
	ow_struct->GPIO_Pin = DS18B20_PIN;
	
	OW_OUTPUT(ow_struct);
	
	Delay(1000);
	
	OW_LOW(ow_struct);
	
	Delay(1000);
	
	OW_HIGH(ow_struct);
	
	Delay(2000);
}

uint8_t OW_RST(OW_t *ow_struct){
	uint8_t in;
	
	/* Low line first, wait 480us */
	OW_LOW(ow_struct);
	OW_OUTPUT(ow_struct);
	OW_DELAY(480);
	OW_DELAY(20);
	
	/* Release line, wait 70us */
	OW_INPUT(ow_struct);
	OW_DELAY(70);
	
	/* get bit value */
	in = GPIO_ReadInputDataBit(ow_struct->GPIOx, ow_struct->GPIO_Pin);
	
	/* wait another 480us */
	OW_DELAY(410);
	
	/* Return value of presence pulse: 0=OK, 1=FAILED */
	return in;
}

void OW_WRITEBIT(OW_t *ow_struct, uint8_t bit){
	if(bit){
		
		/* set line low */
		OW_LOW(ow_struct);
		OW_OUTPUT(ow_struct);
		OW_DELAY(10);
		
		/* Bit high */
		OW_INPUT(ow_struct);
		
		/* Wait 55us, then release line */
		OW_DELAY(55);
		OW_INPUT(ow_struct);
		
	} else{
		
		/* Set line low */
		OW_LOW(ow_struct);
		OW_OUTPUT(ow_struct);
		OW_DELAY(65);
		
		/* Bit high */
		OW_INPUT(ow_struct);
		
		/* wait 5us, then release line */
		OW_DELAY(5);
		OW_INPUT(ow_struct);
		
	}
}

uint8_t OW_READBIT(OW_t *ow_struct){
	
	uint8_t bit = 0;
	
	/* Line low */
	OW_LOW(ow_struct);
	OW_OUTPUT(ow_struct);
	OW_DELAY(2);
	
	/* Release line */
	OW_INPUT(ow_struct);
	OW_DELAY(10);
	
	/* Read value */
	if(GPIO_ReadInputDataBit(ow_struct->GPIOx, ow_struct->GPIO_Pin)){
		
		/* Bit is high */
		bit = 1;
	}
	
	/* wait 50us to complete 60us */
	OW_DELAY(50);
	
	return bit;
}

void OW_WRITEBYTE(OW_t *ow_struct, uint8_t byte){
	uint8_t i = 8;
	
	while(i--){
		OW_WRITEBIT(ow_struct, byte & 0x01);
		byte >>=1;
	}
}

uint8_t OW_READBYTE(OW_t *ow_struct){
	uint8_t i=8, byte=0;
	while(i--){
		byte >>=1;
		byte |= (OW_READBIT(ow_struct) << 7);
	}
	
	return byte;
}

//////
void OW_RSTSEARCH(OW_t *ow_struct){
	
	/* reset search state */
	ow_struct->LastDiscrepancy = 0;
	ow_struct->LastDeviceFlag = 0;
	ow_struct->LastFamilyDiscrepancy = 0;
}

uint8_t OW_SEARCH(OW_t *ow_struct, uint8_t OW_CMD){
	uint8_t id_bit_number;
	uint8_t last_zero, ROM_byte_number, search_result;
	uint8_t id_bit, cmp_id_bit;
	uint8_t ROM_byte_mask, search_direction;
	
	/* Init for search */
	id_bit_number =1;
	last_zero = 0;
	ROM_byte_number = 0;
	ROM_byte_mask = 1;
	search_result = 0;
	
	/* if recent call was not the last */
	if(!ow_struct->LastDeviceFlag){
		
		/* ONEWIRE Reset */
		if(OW_RST(ow_struct)){
			
			/* reset search */
			OW_RSTSEARCH(ow_struct);
			
			return 0;
		}
		
		/* start search command */
		OW_WRITEBYTE(ow_struct, OW_CMD);
		
		/* loop for search */
		do{
			id_bit = OW_READBIT(ow_struct);
			cmp_id_bit = OW_READBIT(ow_struct);
			
			/* check for sensors */
			if((id_bit ==1) && (cmp_id_bit ==1)){
				break;
			} else{
				/* all sensor have either 0 or 1 */
				if(id_bit != cmp_id_bit){
					search_direction = id_bit;
				} else{
					if(id_bit_number < ow_struct->LastDiscrepancy){
						search_direction = ((ow_struct->ROM_NO[ROM_byte_number]\
						& ROM_byte_mask) > 0);
					} else{
						search_direction = (id_bit_number == ow_struct->LastDiscrepancy);
					}
					
					/* if zero was picked, save its index */
					if(search_direction == 0){
						last_zero = id_bit_number;
						
						/* check for last dicrepancy in family */
						if(last_zero < 9){
							ow_struct->LastFamilyDiscrepancy = last_zero;
						}
					}
				}
				
				/* set or clear the bit in ROM byte */
				if(search_direction == 1){
					ow_struct->ROM_NO[ROM_byte_number] |= ROM_byte_mask;
				} else{
					ow_struct->ROM_NO[ROM_byte_number] &= ~ROM_byte_mask;
				}
				
				/* serialnumber search direction */
				OW_WRITEBIT(ow_struct, search_direction);
				
				/* increment the byte counter id_bit_number
				and shift the mask rom_byte_mask by 1 */
				id_bit_number++;
				ROM_byte_mask <<=1;
				
				/* if mask is 0, go to new SN and reset mask */
				if(ROM_byte_mask == 0){
					ROM_byte_number++;
					ROM_byte_mask = 1;
				}
			}
		} while(ROM_byte_number < 8); //loop until all Rom bits
		
		/* if search was a success */
		if(!(id_bit_number < 65)){
			ow_struct->LastDiscrepancy = last_zero;
			
			/* check for last sensor */
			if(ow_struct->LastDiscrepancy == 0){
				ow_struct->LastDeviceFlag = 1;
			}
			
			search_result = 1;
		}
	}
	
	/* if NO sensor found, reset counters for next clean search */
	if(!search_result || !ow_struct->ROM_NO[0]){
		OW_RSTSEARCH(ow_struct);
		search_result = 0;
	}
	
	return search_result;
}

int OW_VERIFY(OW_t *ow_struct){
	unsigned char rom_backup[8];
	int i,result,lstdisc_backup,lstdevflg_backup,lstfamdisc_backup;
	
	/* make backup of current state */
	for(i = 0; i<8; i++){
		rom_backup[i] = ow_struct->ROM_NO[i];
	}
		lstdisc_backup = ow_struct->LastDiscrepancy;
		lstdevflg_backup = ow_struct->LastDeviceFlag;
		lstfamdisc_backup = ow_struct->LastFamilyDiscrepancy;
		
		/* start search to find same sensor */
		ow_struct->LastDiscrepancy = 64;
		ow_struct->LastDeviceFlag = 0;
		
		if(OW_SEARCH(ow_struct, SEARCHROM)){
			
			/* check if same sensor is found */
			result = 1;
			for(i=0;i<8;i++){
				if(rom_backup[i] != ow_struct->ROM_NO[i]){
					result = 1;
					break;
				}
			}
		}else{
			result = 0;
		}
		
		/* restore search states */
		for(i=0;i<8;i++){
			ow_struct->ROM_NO[i] = rom_backup[i];
		}
		
		ow_struct->LastDiscrepancy = lstdisc_backup;
		ow_struct->LastDeviceFlag = lstdevflg_backup;
		ow_struct->LastFamilyDiscrepancy = lstfamdisc_backup;
		
		return result;
}
			
void OW_TARGETSETUP(OW_t *ow_struct){
	uint8_t i;
	ow_struct->ROM_NO[i] = FAMILY_ROM;
	for(i = 1; i<8; i++){
		ow_struct->ROM_NO[i] = 0;
	}
	
	ow_struct->LastDiscrepancy = 64;
	ow_struct->LastFamilyDiscrepancy = 0;
	ow_struct->LastDeviceFlag = 0;
}

void OW_FAMILYSKIP(OW_t * ow_struct){
	/* set LastDiscrepanxy to LastFamilyDiscrepancy */
	ow_struct->LastDiscrepancy = ow_struct->LastDiscrepancy;
	ow_struct->LastFamilyDiscrepancy = 0;
	
	/* check for end of list */
	if(ow_struct->LastDiscrepancy == 0){
		ow_struct->LastDeviceFlag = 1;
	}
}

uint8_t OW_getROM(OW_t *ow_struct, uint8_t index){
	return ow_struct->ROM_NO[index];
}

void OW_SELECT(OW_t *ow_struct, uint8_t* adress){
	uint8_t i;
	OW_WRITEBYTE(ow_struct, MATCHROM);
	
	for(i=0; i<8; i++){
		OW_WRITEBYTE(ow_struct, *(adress + i));
	}
}

void OW_SELECTwPOINTER(OW_t *ow_struct, uint8_t *ROM){
	uint8_t i;
	OW_WRITEBYTE(ow_struct, MATCHROM);
	for(i=0; i<8; i++){
		OW_WRITEBYTE(ow_struct, *(ROM + i));
	}
}

void OW_GETFULLROM(OW_t *ow_struct, uint8_t *firstIndex){
	uint8_t i;
	
	for(i=0; i<8; i++){
		*(firstIndex + i) = ow_struct->ROM_NO[i];
	}
}

uint8_t OW_CRC(uint8_t *adress, uint8_t lenght){
	uint8_t crc = 0, inbyte, i, mix;
	
	while(lenght--){
		inbyte = *adress++;
		for(i=8; i; i--){
			mix = (crc ^ inbyte) & 0x01;
			crc >>=1;
			if(mix){
				crc ^=0x8C;
			}
			inbyte >>=1;
		}
	}
	
	/* return calculated CRC */
	return crc;
}

uint8_t OW_FIRST(OW_t * ow_struct){
	OW_RSTSEARCH(ow_struct);
	
	/* start search */
	return OW_SEARCH(ow_struct, SEARCHROM);
}

uint8_t OW_NEXT(OW_t *ow_struct){
	return OW_SEARCH(ow_struct, SEARCHROM);
}
