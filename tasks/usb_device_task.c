#include <FreeRTOS.h>
#include <task.h>
#include <tasks/usb_device_task.h>
#include <tusb.h>

#define STACK_SIZE configMINIMAL_STACK_SIZE

static StaticTask_t xTaskBuffer;
static StackType_t xStack[STACK_SIZE];
static TaskHandle_t xHandle;

static void usb_device_task(void *param) {
  (void)param;
  vTaskDelay(pdMS_TO_TICKS(1000));
  tusb_init();
  while (1) {
    tud_task();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void create_usb_device_task(void) {
  xHandle = xTaskCreateStatic(usb_device_task, "t0001", STACK_SIZE, NULL,
                              configMAX_PRIORITIES - 1, xStack, &xTaskBuffer);
}

void bind_usb_device_task_to_core0(void) {
  UBaseType_t uxCoreAffinityMask = 1;
  vTaskCoreAffinitySet(xHandle, uxCoreAffinityMask);
}