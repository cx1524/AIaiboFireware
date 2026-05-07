/**
 * @file comm_interface.h
 * @brief Abstract communication interface definitions
 *
 * This header file provides abstracted communication interfaces that decouple
 * upper-layer applications from underlying communication implementations.
 * It defines a unified interface for various communication mechanisms,
 * enabling seamless switching between different transport layers.
 */

#ifndef __COMM_INTERFACE_H__
#define __COMM_INTERFACE_H__

#include <stdint.h>
#include <stdbool.h>

bool comm_interface_init(void);
bool comm_interface_deinit(void);
bool comm_interface_connect(void);
bool comm_interface_disconnect(void);
bool comm_interface_is_connected(void);
bool comm_interface_send(const uint8_t* buf, uint32_t len);
bool comm_interface_recv(uint8_t* buf, uint32_t len);
uint8_t comm_interface_get_status(void);

#endif
