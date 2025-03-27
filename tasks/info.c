#include <FreeRTOS.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <task.h>
#include <tasks/info.h>

#define STACK_SIZE configMINIMAL_STACK_SIZE

static StaticTask_t xTaskBuffer;
static StackType_t xStack[STACK_SIZE];
static TaskHandle_t xHandle;

static void info_task(void *pvParameters) {
  (void)pvParameters;
  stdio_uart_init();
  // gpio_init(LED_PIN);
  // gpio_set_dir(LED_PIN, GPIO_OUT);
  // bool con = false;
  while (1) {
    printf("Running FreeRTOS version: %s (on Core %u)\n",
           tskKERNEL_VERSION_NUMBER, get_core_num());
    UBaseType_t free_words = uxTaskGetStackHighWaterMark(NULL);
    printf("Stack usage: %u words free\n", free_words);
    // gpio_put(LED_PIN, con);
    // con = !con;
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void create_info_task(void) {
  xHandle = xTaskCreateStatic(info_task, "t0000", STACK_SIZE, NULL,
                              tskIDLE_PRIORITY, xStack, &xTaskBuffer);
}

void bind_info_task_to_core0(void) {
  UBaseType_t uxCoreAffinityMask = 1;
  vTaskCoreAffinitySet(xHandle, uxCoreAffinityMask);
}