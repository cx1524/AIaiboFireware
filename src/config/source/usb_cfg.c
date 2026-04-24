#include "usb_cfg.h"
#include "apm32f402_403_rcm.h"
#include "apm32f402_403_gpio.h"
#include "apm32f402_403_usb_device.h"

#define RX_FIFO_SIZE 128

#define TX_CMD_FIFO_SIZE 64
#define TX_DATA_FIFO_SIZE 128

static void usb_msp_init(void);

void usb_init(void)
{
    usb_msp_init();
}

static void usb_msp_init(void)
{
    GPIO_Config_T gpio_init = {0};

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);
    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_OTGFS);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_AFIO);

    /* PA11: OTG_DM PA12: OTG_DP */
    gpio_init.pin  = GPIO_PIN_11 | GPIO_PIN_12;
    gpio_init.mode = GPIO_MODE_AF_PP;
    gpio_init.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOA, &gpio_init);

    NVIC_ConfigPriorityGroup(NVIC_PRIORITY_GROUP_4);
    NVIC_EnableIRQRequest(OTG_FS_IRQn, 1, 0);

    USB_OTG_DisableAllGlobalInterrupt(usbd_handle.usbGlobal);

    /* Link USBD Registers */
    usbd_handle.usbGlobal = USB_OTG_FS;
    usbd_handle.usbPower = USB_OTG_FS_PWR;
    usbd_handle.usbDevice = USB_OTG_FS_D;
    usbd_handle.usbFifo = USB_OTG_FS_FIFO;

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
