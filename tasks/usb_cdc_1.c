#include <FreeRTOS.h>
#include <task.h>
#include <tasks/usb_cdc_0.h>
#include <tusb.h>
#include <usb_cdc/connected.h>

#define STACK_SIZE 512

static TaskHandle_t xHandle;

const char *usb_cdc_1_device_name = "usb_cdc_1_task";

#define USB_CDC_1_BUFFER_SIZE 256u

static char usb_cdc_1_buffer[USB_CDC_1_BUFFER_SIZE];

static void usb_cdc_1_task(void *pvParameters)
{
  EventGroupHandle_t *p_event_group = (EventGroupHandle_t *)pvParameters;
  vTaskDelay(pdMS_TO_TICKS(1000));
  while (1)
  {
    if (tud_cdc_n_connected(1))
      usb_cdc_connected(p_event_group, 1, usb_cdc_1_buffer,
                        USB_CDC_1_BUFFER_SIZE);
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void create_usb_cdc_1_task(EventGroupHandle_t *p_event_group)
{
  xTaskCreate(&usb_cdc_1_task, usb_cdc_1_device_name,
              STACK_SIZE, p_event_group,
              configMAX_PRIORITIES - 2, &xHandle);
}

void bind_usb_cdc_1_task_to_core0(void)
{
  UBaseType_t uxCoreAffinityMask = 1;
  vTaskCoreAffinitySet(xHandle, uxCoreAffinityMask);
}