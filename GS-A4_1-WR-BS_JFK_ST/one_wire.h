#ifndef _ONE_WIRE_H
#define _ONE_WIRE_H
#include "TI_Lib.h"
//#include "TI_memory_map.h"
#include <stdbool.h>

//typedef struct{
//	GPIO_TypeDef*  GPIOx;
//	uint16_t GPIO_Pin;
//	uint8_t LastDiscrepancy;
//	uint8_t LastFamilyDiscrepancy;
//	uint8_t LastDeviceFlag;
//	uint8_t ROM_NO[8];
//}onewire_t;



//#define CMD_SEARCH_ROM		0xf0
//#define CMD_READ_ROM			0x33
//#define	CMD_MATCH_ROM			0x55
//#define CMD_SKIP_ROM			0xcc
//#define	CMD_ALARM_SEARCH	0xec


//// Initiates temperature conversion.
//// DS18B20 transmits conversion status to master
//#define CMD_CONVERT_T 0x44

//// Reads the entire scratchpad including the CRC byte.
//// DS18B20 transmits up to 9 data bytes to master.
//#define CMD_READ_SP		0xBE
//// Writes data into scratchpad bytes 2, 3, and 4 (TH, TL, and configuration registers).
//// Master transmits 3 data bytes to DS18B20.
//#define CMD_WRITE_SP	0x4E
//// Copies TH, TL, and configuration register data from the scratchpad to EEPROM.
//#define CMD_CPY_SP		0x48
//// Recalls TH, TL, and configuration register data from EEPROM to the scratchpad.
//#define	CMD_READCALL	0xB8
//// Signals DS18B20 power supply mode to the master
//#define	CMD_READ_PS		0xB4


////Speed 50MHz
//#define SPEED GPIOG->OSPEEDR = (GPIOG->OSPEEDR & ~(3 << (0 * 2))) | (GPIO_Speed_50MHz << (0 * 2))

////Output PG0
//#define Tx_DS18B20 GPIOG->MODER = (GPIOG->MODER & ~(3 << (0 * 2))) | (GPIO_Mode_OUT << (0 * 2))

////Input PG0
//#define Rx_DS18B20 GPIOG->MODER = (GPIOG->MODER & ~(3 << (0 * 2))) | (GPIO_Mode_IN << (0 * 2))

//// HIGH
//#define HIGH GPIOG->BSRRL = (1 << 0);

//// LOW
//#define LOW GPIOG->BSRRH = (1 << 0);


////Open drain PG0
////#define OPEN_DRAIN GPIOG->OTYPER |= (1 << 0);
//#define	OPEN_DRAIN GPIOG->OTYPER = (GPIOG->OTYPER & ~(1 << (0))) | (GPIO_OType_OD << (0))

////push-pull PG0
////#define PUSH_PULL GPIOG->OTYPER &= ~(1 << 0);
//#define PUSH_PULL GPIOG->OTYPER = (GPIOG->OTYPER & ~(1 << (0))) | (GPIO_OType_PP << (0))



#endif
