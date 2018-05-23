#ifndef _DS18B20_H
#define _DS18B20_H
#include "TI_Lib.h"
#include <stdio.h>
#include <stdbool.h>

#define FAMILY_ROM	0x28

typedef struct
{
	uint8_t	sensor_id[8];
	float		temperature;
	bool		exist;
} TempSensor;


typedef enum {
	RESOLUTION_9BITS = 9,
	RESOLUTION_10BITS = 10,
	RESOLUTION_11BITS = 11,
	RESOLUTION_12BITS = 12
} Resolution_t;

#define RESOLUTION_R1	6
#define RESOLUTION_R0	5



#endif /* _DS18B20_H */

