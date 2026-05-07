#include "comm_interface.h"
#include "comm_usb.h"

bool comm_interface_init(void)
{
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
    }
#endif
    return true;
}

bool comm_interface_deinit(void)
{
    return true;
}

bool comm_interface_connect(void)
{
    return true;
}

bool comm_interface_disconnect(void)
{
    return true;
}

bool comm_interface_is_connected(void)
{
    return true;
}

bool comm_interface_send(const uint8_t* buf, uint32_t len)
{
    return true;
}

bool comm_interface_recv(uint8_t* buf, uint32_t len)
{
    return true;
}

uint8_t comm_interface_get_status(void)
{
    return 0;
}

