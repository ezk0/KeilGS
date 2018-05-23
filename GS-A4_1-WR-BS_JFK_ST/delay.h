#include "timer.h"

void
delay (uint32_t usec) {
	timerinit();
	uint32_t start = usec * TICKS_PER_US;
	uint32_t ticks = getTimeStamp();
	while (getTimeStamp() < start + Ticks) {
	
	}
}