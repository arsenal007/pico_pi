#include <FreeRTOS.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <task.h>
#include <tasks/info.h>
#include <hardware/gpio.h>

#define STACK_SIZE configMINIMAL_STACK_SIZE

static TaskHandle_t xHandle;

static void info_task(void *pvParameters)
{
  (void)pvParameters;
  stdio_uart_init();
  while (1)
  {
    vTaskDelay(pdMS_TO_TICKS(10000));
  }
}

void create_info_task(void)
{
  xTaskCreate(info_task, "info_task", STACK_SIZE, NULL,
              tskIDLE_PRIORITY, &xHandle);
}

void bind_info_task_to_core0(void)
{
  UBaseType_t uxCoreAffinityMask = 1;
  vTaskCoreAffinitySet(xHandle, uxCoreAffinityMask);
}