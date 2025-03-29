#include <FreeRTOS.h>
#include <task.h>
#include <tasks/usb_device.h>
#include <tusb.h>

#define STACK_SIZE configMINIMAL_STACK_SIZE

static TaskHandle_t xHandle;

static const char *usb_device_task_name = "usb_device_task";

static void usb_device_task(void *param)
{
  (void)param;
  vTaskDelay(pdMS_TO_TICKS(1000));
  tusb_init();
  while (1)
  {
    tud_task();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void create_usb_device_task(void)
{
  xTaskCreate(usb_device_task, usb_device_task_name, STACK_SIZE, NULL,
              configMAX_PRIORITIES - 1, &xHandle);
}

void bind_usb_device_task_to_core0(void)
{
  UBaseType_t uxCoreAffinityMask = 1;
  vTaskCoreAffinitySet(xHandle, uxCoreAffinityMask);
}
