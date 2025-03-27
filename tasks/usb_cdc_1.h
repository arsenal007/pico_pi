#ifndef TASKS_USB_CDC_1_H
#define TASKS_USB_CDC_1_H
#include <event_groups.h>

void create_usb_cdc_1_task(EventGroupHandle_t* p_event_group);
void bind_usb_cdc_1_task_to_core0(void);

#endif  // TASKS_USB_DEVICE_H
