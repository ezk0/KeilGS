#include <stdio.h>
#include "owire.h"
#include <stm32f4xx.h>
#include "timer.h"
#include "TI_Lib.h"
#include "stm32f4xx_gpio.h"


void OWinit(Owire *ow){
	GPIO_InitTypeDef gpio_structure;
	gpio_structure.GPIO_Pin = DS18B20_PIN;
	gpio_structure.GPIO_Speed = GPIO_Speed_50MHz;
	
	gpio_structure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOG, &gpio_structure);
	
	uint32_t pinpos = 0x00, pos = 0x00, currentpin= 0x00;
	
	ow->m_BitMask=DS18B20_PIN;
	ow->m_Port=GPIOG;
	
	uint8_t RegShift;
	
	if((DS18B20_PIN & (uint32_t)0x00FF) > 0){
		ow->m_Port->BSRRH = ow->m_BitMask<<16;
		
		for(pinpos = 0x00; pinpos < 0x09 ; pinpos++){
		pos = ((uint32_t)0x01) >> pinpos;
		
		//get the port pins position
		currentpin = (uint16_t)((DS18B20_PIN) & pos);
		if(currentpin == pos){
			RegShift = (pinpos * 4);
			ow->m_RegMask = ((uint32_t)0x0F) << (pinpos * 4);
			break;
		}
	}
} else{
		ow->m_Port->BSRRH = ow->m_BitMask;
	
	for(pinpos = 0x00; pinpos < 0x08; pinpos++){
		pos = ((uint32_t) 0x01) << (pinpos + 0x08);
		
		currentpin = (uint16_t) ((DS18B20_PIN) & pos);
		if(currentpin == pos){
			RegShift = (pinpos * 4);
			ow->m_RegMask = ((uint32_t)0x0F) << (pinpos * 4 );
			break;
		}
	}
}
ow->m_InputMask = (((GPIO_Mode_IN << RegShift) & ow->m_RegMask));
ow->m_OutputMask = (((uint32_t) GPIO_Mode_OUT | (uint32_t) GPIO_Speed_50MHz) << RegShift) & ow->m_RegMask;

}

void OWinput(Owire *ow){
	*ow->m_Register &= ~ow->m_RegMask;
	*ow->m_Register |= ow->m_InputMask;
}

void OWoutput(Owire *ow){
	*ow->m_Register &= ~ow->m_RegMask;
	*ow->m_Register |= ow->m_OutputMask;
}

uint8_t OWreadPIN(Owire *ow){
	return (uint8_t) ((ow->m_Port->IDR & ow->m_BitMask) > 0 ? 1 : 0);
}

void OWwriteHIGH(Owire *ow){
	ow->m_Port->BSRRL = ow->m_BitMask;
}

void OWwriteLOW(Owire *ow){
	ow->m_Port->BSRRH = ow->m_BitMask;
}

static void OWnoInterrupts(void)
{
  __asm("cpsid i");
}

static void OWinterrupts(void)
{
  __asm("cpsie i");
}

void OWdelay(uint16_t TIME_uS){
	resetTimer();
	while(getTimeStamp() <= TIME_uS*CONVERT2US);
}

uint8_t OWreset(Owire *ow){
	uint8_t result;
	uint8_t retry = 100;
	
	OWnoInterrupts();
	OWinput(ow);
	OWinterrupts();
	
	do{
		if(--retry == 0) return 0;
			OWdelay(2);
		} while(!OWreadPIN(ow));
		
		OWnoInterrupts();
		OWwriteLOW(ow);
		OWoutput(ow);
		OWinterrupts();
		OWdelay(480);
		OWnoInterrupts();
		OWinput(ow);
		OWdelay(70);
		result = !OWreadPIN(ow);
		OWinterrupts();
		OWdelay(410);
		
		return result;
	}

	void OWwrite_BIT(Owire *ow, uint8_t bit){
		
		if(bit & 1){
			OWnoInterrupts();
			OWwriteLOW(ow);
			OWoutput(ow);
			OWdelay(10);
			OWwriteHIGH(ow);
			OWinterrupts();
			OWdelay(55);
		} else{
			OWnoInterrupts();
			OWwriteLOW(ow);
			OWoutput(ow);
			OWdelay(65);
			OWwriteHIGH(ow);
			OWinterrupts();
			OWdelay(5);
		}
	}
	
	uint8_t OWread_BIT(Owire *ow){
		uint8_t result;
		
		OWnoInterrupts();
		OWwriteLOW(ow);
		OWoutput(ow);
		OWdelay(3);
		OWinput(ow);
		OWdelay(10);
		result = OWreadPIN(ow);
		OWdelay(53);
		
		return result;
	}
	
	void OWwrite(Owire *ow, uint8_t bit){
		uint8_t bitMask;
		
		for(bitMask = 0x01; bitMask; bitMask <<=1){
			OWwrite_BIT(ow, (bitMask & bit) ? 1 : 0);
		}
		
		OWnoInterrupts();
		OWinput(ow);
		OWwriteLOW(ow);
		OWinterrupts();
	}
	
	void OWwrite_BYTES(Owire *ow, const uint8_t *buffer, uint16_t counter){
		for(uint16_t i = 0; i < counter ; i++){
			OWwrite(ow, buffer[i]);
			
			OWnoInterrupts();
			OWinput(ow);
			OWwriteLOW(ow);
			OWinterrupts();
		}
	}
	
	uint8_t OWread(Owire *ow){
		uint8_t bitMask;
		uint8_t result = 0;
		
		for(bitMask = 0x01; bitMask; bitMask <<=1){
			if(OWread_BIT(ow)){
				result |= bitMask;
			}				
		}
		return result;
	}		
	
	void OWread_BYTES(Owire *ow, uint8_t *buffer, uint16_t counter){
		for(uint16_t i = 0; i < counter; i++){
			buffer[i] = OWread(ow);
		}
	}
	
	#ifdef ONEWIRE_SEARCH
	
	void OWreset_search(Owire *ow){
		ow->LastDiscrepancy = 0;
		ow->LastDeviceFlag = FALSE;
		ow->LastFamilyDiscrepancy = 0;
		
		for(int i = 7; ; i--)
  {
    ow->ROM_NO[i] = 0;
    if ( i == 0) break;
  }
	}
	#endif
	
	uint8_t OWsearch(Owire *ow, uint8_t *newAddress){
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
	if(!ow->LastDeviceFlag){
		
		/* ONEWIRE Reset */
		if(!OWreset(ow)){
			// reset the search
      ow->LastDiscrepancy = 0;
      ow->LastDeviceFlag = FALSE;
      ow->LastFamilyDiscrepancy = 0;
      return FALSE;
		}
		
		/* start search command */
		OWwrite(ow, 0xF0);
		
		/* loop for search */
		do{
			id_bit = OWread_BIT(ow);
			cmp_id_bit = OWread_BIT(ow);
			
			/* check for NO sensors */
			if((id_bit ==1) && (cmp_id_bit ==1)){
				break;
			} else{
				/* all sensor have either 0 or 1 */
				if(id_bit != cmp_id_bit){
					search_direction = id_bit;
				} else{
					if(id_bit_number < ow->LastDiscrepancy){
						search_direction = ((ow->ROM_NO[ROM_byte_number]\
						& ROM_byte_mask) > 0);
					} else{
						search_direction = (id_bit_number == ow->LastDiscrepancy);
					}
					
					/* if zero was picked, save its index */
					if(search_direction == 0){
						last_zero = id_bit_number;
						
						/* check for last dicrepancy in family */
						if(last_zero < 9){
							ow->LastFamilyDiscrepancy = last_zero;
						}
					}
				}
				
				/* set or clear the bit in ROM byte */
				if(search_direction == 1){
					ow->ROM_NO[ROM_byte_number] |= ROM_byte_mask;
				} else{
					ow->ROM_NO[ROM_byte_number] &= ~ROM_byte_mask;
				}
				
				/* serialnumber search direction */
				OWwrite_BIT(ow, search_direction);
				
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
			ow->LastDiscrepancy = last_zero;
			
			/* check for last sensor */
			if(ow->LastDiscrepancy == 0){
				ow->LastDeviceFlag = 1;
			}
			
			search_result = 1;
		}
	}
	
	/* if NO sensor found, reset counters for next clean search */
	if(!search_result || !ow->ROM_NO[0]){
      ow->LastDiscrepancy = 0;
      ow->LastDeviceFlag = FALSE;
      ow->LastFamilyDiscrepancy = 0;
      search_result = FALSE;
	}
	   for (int i = 0; i < 8; i++) newAddress[i] = ow->ROM_NO[i];
	return search_result;
}

