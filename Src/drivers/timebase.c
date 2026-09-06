#include "drivers/timebase.h"
#include <timer.h>
uint32_t millis(void) { return GetMilSecTick(); }
void Timebase_Poll(void) { processTimerScheduler(); }