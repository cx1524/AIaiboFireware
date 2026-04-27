#include "device_cfg.h"
#include <stdio.h>
#include <string.h>

#if defined (USE_USB)
#include "usbd_core.h"
#include "usbd_cdc_acm.h"

volatile bool ep_tx_busy_flag = false;

/* endpoint address */
#define CDC_IN_EP  0x81
#define CDC_OUT_EP 0x02
#define CDC_INT_EP 0x83

#define USBD_VID           0xABCD
#define USBD_PID           0xDCBA
#define USBD_MAX_POWER     100

#ifdef CONFIG_USB_HS
#define CDC_MAX_MPS 512
#else
#define CDC_MAX_MPS 64
#endif

/* config descriptor size */
#define USB_CONFIG_SIZE (9 + CDC_ACM_DESCRIPTOR_LEN)

/* descriptor */
static const uint8_t device_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0xEF, 0x02, 0x01, USBD_VID, USBD_PID, 0x0100, 0x01)
};

static const uint8_t config_descriptor[] = {
    USB_CONFIG_DESCRIPTOR_INIT(USB_CONFIG_SIZE, 0x02, 0x01, USB_CONFIG_BUS_POWERED, USBD_MAX_POWER),
    CDC_ACM_DESCRIPTOR_INIT(0x00, CDC_INT_EP, CDC_OUT_EP, CDC_IN_EP, CDC_MAX_MPS, 0x02)
};

static const char *string_descriptors[] = {
    (const char[]){ 0x09, 0x04 }, /* Langid */
    "CherryUSB",                  /* Manufacturer */
    "CherryUSB CDC DEMO",         /* Product */
    "2022123456",                 /* Serial Number */
};

static const uint8_t device_quality_descriptor[] = {
    ///////////////////////////////////////
    /// device qualifier descriptor
    ///////////////////////////////////////
    0x0a,
    USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x00,
    0x00,
};

USBD_HANDLE_T usbd_handle = {0};

static const uint8_t *device_descriptor_callback(uint8_t speed)
{
    return device_descriptor;
}

static const uint8_t *config_descriptor_callback(uint8_t speed)
{
    return config_descriptor;
}

static const uint8_t *device_quality_descriptor_callback(uint8_t speed)
{
    return device_quality_descriptor;
}

static const char *string_descriptor_callback(uint8_t speed, uint8_t index)
{
    if (index > 3) {
        return NULL;
    }
    return string_descriptors[index];
}

const struct usb_descriptor cdc_descriptor = {
    .device_descriptor_callback = device_descriptor_callback,
    .config_descriptor_callback = config_descriptor_callback,
    .device_quality_descriptor_callback = device_quality_descriptor_callback,
    .string_descriptor_callback = string_descriptor_callback
};

/**** buffer ****/
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t read_buffer[CDC_MAX_MPS];
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t write_buffer[CDC_MAX_MPS];

/*** endpoint callback functino */
void usbd_cdc_acm_bulk_out(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    // USB_LOG_RAW("actual out len:%d\r\n", (unsigned int)nbytes);
    /* setup next out ep read transfer */
    usbd_ep_start_read(busid, CDC_OUT_EP, read_buffer, 2048);
}

void usbd_cdc_acm_bulk_in(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    // USB_LOG_RAW("actual in len:%d\r\n", (unsigned int)nbytes);

    if ((nbytes % usbd_get_ep_mps(busid, ep)) == 0 && nbytes) {
        /* send zlp */
        usbd_ep_start_write(busid, CDC_IN_EP, NULL, 0);
    } else {
        ep_tx_busy_flag = false;
    }
}

/* endpoint */
struct usbd_endpoint cdc_out_ep = {
    .ep_addr = CDC_OUT_EP,
    .ep_cb = usbd_cdc_acm_bulk_out
};

struct usbd_endpoint cdc_in_ep = {
    .ep_addr = CDC_IN_EP,
    .ep_cb = usbd_cdc_acm_bulk_in
};

/* interface */
static struct usbd_interface intf0;
static struct usbd_interface intf1;

/* event callback function */
static void usbd_event_handler(uint8_t busid, uint8_t event)
{
    switch (event) {
        case USBD_EVENT_RESET:
            break;
        case USBD_EVENT_CONNECTED:
            break;
        case USBD_EVENT_DISCONNECTED:
            break;
        case USBD_EVENT_RESUME:
            break;
        case USBD_EVENT_SUSPEND:
            break;
        case USBD_EVENT_CONFIGURED:
            ep_tx_busy_flag = false;
            /* setup first out ep read transfer */
            usbd_ep_start_read(busid, CDC_OUT_EP, read_buffer, CDC_MAX_MPS);
            break;
        case USBD_EVENT_SET_REMOTE_WAKEUP:
            break;
        case USBD_EVENT_CLR_REMOTE_WAKEUP:
            break;

        default:
            break;
    }
}

void cdc_acm_init(uint8_t busid, uintptr_t reg_base)
{
    /* add descriptor */
    usbd_desc_register(busid, &cdc_descriptor);
    /* add interface */
    usbd_add_interface(busid, usbd_cdc_acm_init_intf(busid, &intf0));
    usbd_add_interface(busid, usbd_cdc_acm_init_intf(busid, &intf1));
    /* add endpoint */
    usbd_add_endpoint(busid, &cdc_out_ep);
    usbd_add_endpoint(busid, &cdc_in_ep);
    /* initialize */
    usbd_initialize(busid, reg_base, usbd_event_handler);
}

void usb_dc_low_level_init(void)
{
    device_config();
}
#endif

char buffer[] = "Hello World!\r\n";

int main(void) {
#if defined (USE_USB)
    cdc_acm_init(0, 0);

    while (!usb_device_is_configured(0));

    while (1)
    {
        if (ep_tx_busy_flag)
        {
            ep_tx_busy_flag = true;
            usbd_ep_start_write(0, CDC_OUT_EP, buffer, strlen(buffer));
        }
        GPIO_SetBit(GPIOB, GPIO_PIN_6);
        GPIO_SetBit(GPIOB, GPIO_PIN_7);
        delay_ms(500);
        GPIO_ResetBit(GPIOB, GPIO_PIN_6);
        GPIO_ResetBit(GPIOB, GPIO_PIN_7);
        delay_ms(500);
    }
#endif
}
