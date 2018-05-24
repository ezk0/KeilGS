#ifndef OW_H
#define OW_H

#include "TI_Lib.h"

/* Sensor Konstrukt */
typedef struct {
	GPIO_TypeDef* GPIOx;           
	uint16_t GPIO_Pin;             
	uint8_t LastDiscrepancy;       
	uint8_t LastFamilyDiscrepancy; 
	uint8_t LastDeviceFlag;        
	uint8_t ROM_NO[8];             
} OW_t;

void OW_DELAY(uint16_t TIME_uS);

/* Pin settings */
void OW_LOW(OW_t *ow);			
void OW_HIGH(OW_t *ow);		
void OW_INPUT(OW_t *ow);		
void OW_OUTPUT(OW_t *ow);

/* OneWire commands */
#define RSCRATCHPAD			0xBE
#define WSCRATCHPAD			0x4E
#define CPYSCRATCHPAD		0x48
#define RECEEPROM			0xB8
#define RPWRSUPPLY			0xB4
#define SEARCHROM			0xF0
#define READROM				0x33
#define MATCHROM			0x55
#define SKIPROM				0xCC

extern uint8_t OW_RST(OW_t *ow_struct);
extern void OW_SELECTwPOINTER(OW_t *ow_struct, uint8_t *ROM);
extern void OW_WRITEBYTE(OW_t *ow_struct, uint8_t byte);
extern uint8_t OW_READBIT(OW_t *ow_struct);
extern uint8_t OW_READBYTE(OW_t *ow_struct);
extern uint8_t OW_CRC(uint8_t *adress, uint8_t lenght);
#endif
