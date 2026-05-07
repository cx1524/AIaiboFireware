#include "comm_usb.h"

#include "usbd_core.h"
#include "usbd_cdc_acm.h"

#include "apm32f402_403_misc.h"
#include "apm32f402_403_rcm.h"
#include "apm32f402_403_gpio.h"
#include "apm32f402_403_usb_device.h"
#include "usb_reg.h"

#define RX_FIFO_SIZE 128

#define TX_CMD_FIFO_SIZE 64
#define TX_DATA_FIFO_SIZE 128

static void usb_msp_init(void);

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
    usb_msp_init();
}

static void usb_msp_init(void)
{
    GPIO_Config_T gpio_init = {0};

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);
    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_OTGFS);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_AFIO);

    NVIC_SetPriorityGrouping(NVIC_PRIORITY_GROUP_4);
    NVIC_SetPriority(PendSV_IRQn, 15);
    NVIC_SetPriority(SysTick_IRQn, 15);

    /* PA11: OTG_DM PA12: OTG_DP */
    gpio_init.pin  = GPIO_PIN_11 | GPIO_PIN_12;
    gpio_init.mode = GPIO_MODE_AF_PP;
    gpio_init.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOA, &gpio_init);

    /* Link USBD Registers */
    usbd_handle.usbGlobal = USB_OTG_FS;
    usbd_handle.usbPower = USB_OTG_FS_PWR;
    usbd_handle.usbDevice = USB_OTG_FS_D;
    usbd_handle.usbFifo = USB_OTG_FS_FIFO;

    NVIC_EnableIRQRequest(OTG_FS_IRQn, 1, 0);

    USB_OTG_DisableAllGlobalInterrupt(usbd_handle.usbGlobal);

    usbd_handle.usbCfg.vbusSense = DISABLE;
    usbd_handle.usbCfg.lowPowerStatus = DISABLE;
    usbd_handle.usbCfg.powerManageStatus = DISABLE;
    usbd_handle.usbCfg.sofStatus = DISABLE;
    usbd_handle.usbCfg.phyType = USB_OTG_PHY_EMB;
    usbd_handle.usbCfg.devEndpointNum = 4;
    usbd_handle.usbCfg.dmaStatus = DISABLE;
    usbd_handle.usbCfg.speed = USB_OTG_SPEED_FSLS;
    usbd_handle.usbCfg.speedChannel = USBD_SPEED_CH_FS;
    usbd_handle.usbCfg.mode = USB_OTG_MODE_DEVICE;
    usbd_handle.usbCfg.ep1Status = DISABLE;

    USBD_Config(&usbd_handle);

    USB_OTG_ConfigRxFifoSize(usbd_handle.usbGlobal, RX_FIFO_SIZE);

    USBD_OTG_ConfigDeviceTxFifo(&usbd_handle, 0, TX_CMD_FIFO_SIZE);
    USBD_OTG_ConfigDeviceTxFifo(&usbd_handle, 1, TX_DATA_FIFO_SIZE);

    USBD_Start(&usbd_handle);
}
