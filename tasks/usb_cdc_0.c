#include <FreeRTOS.h>
#include <task.h>
#include <tasks/usb_cdc_0.h>
#include <tusb.h>
#include <usb_cdc/connected.h>

#define STACK_SIZE configMINIMAL_STACK_SIZE

static TaskHandle_t xHandle;

const char *usb_cdc_0_device_name = "usb_cdc_0_task";

#define USB_CDC_0_BUFFER_SIZE 256u

static char usb_cdc_0_buffer[USB_CDC_0_BUFFER_SIZE];

static void usb_cdc_0_task(void *pvParameters)
{
  EventGroupHandle_t *p_event_group = (EventGroupHandle_t *)pvParameters;
  vTaskDelay(pdMS_TO_TICKS(1000));
  while (1)
  {
    if (tud_cdc_n_connected(0))
      usb_cdc_connected(p_event_group, 0, usb_cdc_0_buffer,
                        USB_CDC_0_BUFFER_SIZE);

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void create_usb_cdc_0_task(EventGroupHandle_t *p_event_group)
{
  xTaskCreate(usb_cdc_0_task, usb_cdc_0_device_name, STACK_SIZE,
              p_event_group, configMAX_PRIORITIES - 2,
              &xHandle);
}

void bind_usb_cdc_0_task_to_core0(void)
{
  UBaseType_t uxCoreAffinityMask = 1;
  vTaskCoreAffinitySet(xHandle, uxCoreAffinityMask);
}