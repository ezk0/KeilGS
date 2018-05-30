#ifndef OWIRE_H
#define OWIRE_H

#include "TI_Lib.h"
#define	DS18B20_GPIO GPIOG
#define	DS18B20_PIN (0x01U << 0)

#ifndef ONEWIRE_SEARCH
#define ONEWIRE_SEARCH 1
#endif

#define RSCRATCHPAD			0xBE
#define WSCRATCHPAD			0x4E
#define CPYSCRATCHPAD		0x48
#define RECEEPROM			0xB8
#define RPWRSUPPLY			0xB4
#define SEARCHROM			0xF0
#define READROM				0x33
#define MATCHROM			0x55
#define SKIPROM				0xCC

#define FALSE 0
#define TRUE  1

typedef struct{
  GPIO_TypeDef*         m_Port;        
  __IO uint32_t*        m_Register;
  uint32_t              m_RegMask;
  uint32_t              m_InputMask;
  uint32_t              m_OutputMask;
  uint16_t              m_BitMask;
#ifdef ONEWIRE_SEARCH
  // global search state
  unsigned char ROM_NO[8];
  uint8_t LastDiscrepancy;
  uint8_t LastFamilyDiscrepancy;
  uint8_t LastDeviceFlag;
#endif
}Owire;
#endif

extern void OWinit(Owire *ow);
extern 	uint8_t OWsearch(Owire *ow, uint8_t *newAddress);
