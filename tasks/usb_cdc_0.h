#ifndef TASKS_USB_CDC_0_H
#define TASKS_USB_CDC_0_H
#include <event_groups.h>

void create_usb_cdc_0_task(EventGroupHandle_t* p_event_group);
void bind_usb_cdc_0_task_to_core0(void);

#endif  // TASKS_USB_DEVICE_H
