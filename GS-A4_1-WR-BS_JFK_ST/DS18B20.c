#include "DS18B20.h"

void
CRCwrite (uint8_t CMD) {

}

uint8_t
CRCread () {
	uint8_t CMD;
	
	return CMD;
}

bool
isDS18B20 (uint8_t * ROM) {
	if (*ROM == FAMILY_ROM) {
		return true;
	}
	return false;
	
}
