#define USE_USB
#if defined(USE_USB)
#include "usbd_core.h"
#include "usb_reg.h"

usb_device_t g_usb_device;

/* Endpoint state */
struct usb_dc_ep_state {
    uint16_t ep_mps;    /* Endpoint max packet size */
    uint8_t ep_type;    /* Endpoint type */
    uint8_t ep_stalled; /* Endpoint stall flag */
    uint8_t *xfer_buf;
    uint32_t xfer_len;
    uint32_t actual_xfer_len;
    uint8_t ep_enable;
};

/* Driver state */
struct myusb_udc {
    volatile uint8_t dev_addr;
    struct usb_dc_ep_state in_ep[16];  /*!< IN endpoint parameters*/
    struct usb_dc_ep_state out_ep[16]; /*!< OUT endpoint parameters */
} g_myusb_udc;

__WEAK void usb_dc_low_level_init(void)
{
}

__WEAK void usb_dc_low_level_deinit(void)
{
}

int usb_dc_init(uint8_t busid)
{
    memset(&g_myusb_udc, 0, sizeof(struct myusb_udc));

    g_usb_device.device_reg = (usb_device_reg_t *)USB_D_BASE;
    for (int i = 0; i < 4; i++) {
        g_usb_device.in_ep[i] = ((usb_in_ep_t *)(USB_D_IN_EP0_BASE + (i * 0x20)));
        g_usb_device.out_ep[i] = ((usb_out_ep_t *)(USB_D_OUT_EP0_BASE + (i * 0x20)));
    }

    usb_dc_low_level_init();
    return 0;
}

int usb_dc_deinit(uint8_t busid)
{
    return 0;
}

int usbd_set_address(uint8_t busid, const uint8_t addr)
{
    return 0;
}

int usbd_set_remote_wakeup(uint8_t busid)
{
    return -1;
}

uint8_t usbd_get_port_speed(uint8_t busid)
{
    return USB_SPEED_FULL;
}

int usbd_ep_open(uint8_t busid, const struct usb_endpoint_descriptor *ep)
{
    uint8_t ep_idx = USB_EP_GET_IDX(ep->bEndpointAddress);

    if (USB_EP_DIR_IS_OUT(ep->bEndpointAddress)) {
        g_myusb_udc.out_ep[ep_idx].ep_mps = USB_GET_MAXPACKETSIZE(ep->wMaxPacketSize);
        g_myusb_udc.out_ep[ep_idx].ep_type = USB_GET_ENDPOINT_TYPE(ep->bmAttributes);
        g_myusb_udc.out_ep[ep_idx].ep_enable = true;
    } else {
        g_myusb_udc.in_ep[ep_idx].ep_mps = USB_GET_MAXPACKETSIZE(ep->wMaxPacketSize);
        g_myusb_udc.in_ep[ep_idx].ep_type = USB_GET_ENDPOINT_TYPE(ep->bmAttributes);
        g_myusb_udc.in_ep[ep_idx].ep_enable = true;
    }
    return 0;
}

int usbd_ep_close(uint8_t busid, const uint8_t ep)
{
    return 0;
}

int usbd_ep_set_stall(uint8_t busid, const uint8_t ep)
{
    return 0;
}

int usbd_ep_clear_stall(uint8_t busid, const uint8_t ep)
{
    return 0;
}

int usbd_ep_is_stalled(uint8_t busid, const uint8_t ep, uint8_t *stalled)
{
    return 0;
}

int usbd_ep_start_write(uint8_t busid, const uint8_t ep, const uint8_t *data, uint32_t data_len)
{
    uint8_t ep_idx = USB_EP_GET_IDX(ep);
    uint32_t tmp;

    if (!data && data_len) {
        return -1;
    }

    g_myusb_udc.in_ep[ep_idx].xfer_buf = (uint8_t *)data;
    g_myusb_udc.in_ep[ep_idx].xfer_len = data_len;
    g_myusb_udc.in_ep[ep_idx].actual_xfer_len = 0;

    return 0;
}

int usbd_ep_start_read(uint8_t busid, const uint8_t ep, uint8_t *data, uint32_t data_len)
{
    uint8_t ep_idx = USB_EP_GET_IDX(ep);

    if (!data && data_len) {
        return -1;
    }

    g_myusb_udc.out_ep[ep_idx].xfer_buf = (uint8_t *)data;
    g_myusb_udc.out_ep[ep_idx].xfer_len = data_len;
    g_myusb_udc.out_ep[ep_idx].actual_xfer_len = 0;

    return 0;
}

void USBD_IRQHandler(uint8_t busid)
{
}
#endif
