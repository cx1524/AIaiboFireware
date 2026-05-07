#ifndef _DELAY_H_
#define _DELAY_H_

#include "apm32f402_403_tmr.h"
#include "apm32f402_403_rcm.h"

void delay_init_ms(void);
void delay_ms(uint32_t ms);

#endif
