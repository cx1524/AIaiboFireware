#ifndef __COMM_USB_H__
#define __COMM_USB_H__

#include <stdint.h>

void cdc_acm_init(uint8_t busid, uintptr_t reg_base);
void usb_dc_low_level_init(void);

#endif
