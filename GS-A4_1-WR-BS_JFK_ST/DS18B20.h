#ifndef DS18B20_H
#define DS18B20_H
#include <stdio.h>
#include <stdbool.h>
#include "TI_Lib.h"
#include "onewire.h"

/* Family Code of DS18B20 */
#define FAMILY_ROM	0x28

/* DS18B20 Temperature Command */
#define DS18B20_CONVERTTEMP 0x44
#define DS18B20_DECIMAL_STEPS_12BIT		0.0625
#define DS18B20_DECIMAL_STEPS_11BIT		0.125
#define DS18B20_DECIMAL_STEPS_10BIT		0.25
#define DS18B20_DECIMAL_STEPS_9BIT		0.5

/* Resolution Bits */
#define DS18B20_RESOLUTION_R1 6
#define DS18B20_RESOLUTION_R0 5

/* Config of DS18B20 Sensor */
#define DS18B20_MAX_SENSORS 7
#define	DS18B20_GPIO GPIOG
#define	DS18B20_PIN (0x01U << 0)
#define	DS18B20_UPDATE_INTERVAL_MS 10000
#define	DS18B20_CONVERT_TIMEOUT_MS 5000


typedef struct{
	uint8_t Adress[8];
	float Temperatur;
	bool dataExists;
}DS18B20_Sensor;

typedef enum {
	DS18B20_Resolution_9bits = 9,   /*!< DS18B20 9 bits resolution */
	DS18B20_Resolution_10bits = 10, /*!< DS18B20 10 bits resolution */
	DS18B20_Resolution_11bits = 11, /*!< DS18B20 11 bits resolution */
	DS18B20_Resolution_12bits = 12  /*!< DS18B20 12 bits resolution */
} DS18B20_Resolution_t;
	

extern DS18B20_Sensor DS18B20[DS18B20_MAX_SENSORS];
extern void DS18B20_STARTALL(OW_t *ow_struct);


#endif /* _DS18B20_H */
