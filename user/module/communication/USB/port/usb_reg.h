#ifndef __USB_REG_H__
#define __USB_REG_H__

#include "core_cm4.h"
#include <stdint.h>

typedef struct
{
    __IO uint32_t gctrlsts;
    __IO uint32_t gint;
    __IO uint32_t gahbcfg;
    __IO uint32_t gusbcfg;
    __IO uint32_t grstctrl;
    __IO uint32_t gcint;
    __IO uint32_t gintmask;
    __IO uint32_t grxsts;
    __IO uint32_t grxfifo;
    __IO uint32_t gtxfcfg;
    __IO uint32_t gnptxfqsts;
    __IO uint32_t ggccfg;
    __IO uint32_t gcid;
    __IO uint32_t ghptxfsize;
    __IO uint32_t dtxfifo1;
    __IO uint32_t dtxfifo2;
    __IO uint32_t dtxfifo3;
} usb_global_t;

typedef struct {
    __IO uint32_t dcfg;
    __IO uint32_t dctrl;
    __IO uint32_t dsts;
    uint32_t reserved0;
    __IO uint32_t dinimask;
    __IO uint32_t doutimask;
    __IO uint32_t daepint;
    __IO uint32_t daepimask;
    uint32_t reserved1[2];
    __IO uint32_t dvbusdtim;
    __IO uint32_t dvbusptim;
    uint32_t reserved2;
    __IO uint32_t dieimask;
} usb_device_reg_t;

typedef struct {
    __IO uint32_t diepctrl;
    uint32_t reserved0;
    __IO uint32_t diepint;
    uint32_t reserved1;
    __IO uint32_t dieptrs;
    uint32_t reserved2;
    __IO uint32_t ditxfsts;
    uint32_t reserved3;
} usb_in_ep_t;

typedef struct {
    __IO uint32_t doepctrl;
    uint32_t reserved0;
    __IO uint32_t doepint;
    uint32_t reserved1;
    __IO uint32_t doeptrs;
} usb_out_ep_t;

typedef struct {
    usb_device_reg_t *device_reg;
    usb_in_ep_t *in_ep[4];
    usb_out_ep_t *out_ep[4];
} usb_device_t;

#define USB_BASE (0x50000000UL)
#define USB_D_BASE (USB_BASE + 0x800)
#define USB_D_IN_EP0_BASE (USB_D_BASE + 0x100)
#define USB_D_OUT_EP0_BASE (USB_D_BASE + 0x300)

extern usb_device_t g_usb_device;

#endif
