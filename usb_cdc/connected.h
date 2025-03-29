#ifndef USB_CDC_CONNECTED_H
#define USB_CDC_CONNECTED_H

#include <FreeRTOS.h>
#include <event_groups.h>
#include <stdint.h>

void usb_cdc_connected(EventGroupHandle_t* p_event_group, uint8_t itf,
                       char* message_buffer, uint32_t message_buffer_size);

#endif  // USB_CDC_CONNECTED_H