#ifndef USB_CDC_PARCE_COMMAND_H
#define USB_CDC_PARCE_COMMAND_H

#include <stdint.h>

void usb_cdc_parce_command(const char *buffer, uint32_t size, uint8_t port);

#endif  // USB_CDC_CONNECTED_H