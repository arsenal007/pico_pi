#include <FreeRTOS.h>
#include <task.h>
#include <tasks/usb_cdc_0.h>
#include <tusb.h>

#define STACK_SIZE configMINIMAL_STACK_SIZE

static StaticTask_t xTaskBuffer;
static StackType_t xStack[STACK_SIZE];
static TaskHandle_t xHandle;

#define USB_CDC_0_BUFFER_SIZE 256u

static uint8_t usb_cdc_0_buffer[USB_CDC_0_BUFFER_SIZE];

static void usb_cdc_0_task_connected(EventGroupHandle_t* p_event_group) {
  uint32_t len = tud_cdc_n_available(0);
  if (len > 0) {
    uint32_t count = tud_cdc_n_read(0, usb_cdc_0_buffer, USB_CDC_0_BUFFER_SIZE);
    tud_cdc_n_read_flush(0);
    printf("%u %s\n", count, usb_cdc_0_buffer);
    memset(usb_cdc_0_buffer, 0, USB_CDC_0_BUFFER_SIZE);
  }
}

static void usb_cdc_0_task(void* pvParameters) {
  EventGroupHandle_t* p_event_group = (EventGroupHandle_t*)pvParameters;
  vTaskDelay(pdMS_TO_TICKS(1000));
  while (1) {
    if (tud_cdc_n_connected(0)) usb_cdc_0_task_connected(p_event_group);
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void create_usb_cdc_0_task(EventGroupHandle_t* p_event_group) {
  xHandle =
      xTaskCreateStatic(usb_cdc_0_task, "t0002", STACK_SIZE, p_event_group,
                        configMAX_PRIORITIES - 2, xStack, &xTaskBuffer);
}

void bind_usb_cdc_0_task_to_core0(void) {
  UBaseType_t uxCoreAffinityMask = 1;
  vTaskCoreAffinitySet(xHandle, uxCoreAffinityMask);
}