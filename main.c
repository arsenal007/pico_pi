#include <FreeRTOS.h>
#include <pico/bootrom.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <task.h>
#include <tasks/info.h>
#include <tasks/usb_cdc_0.h>
#include <tasks/usb_cdc_1.h>
#include <tasks/usb_device.h>

// #define LED_PIN 25

static EventGroupHandle_t event_group;

int main() {
  create_info_task();
  create_usb_device_task();
  create_usb_cdc_0_task(&event_group);
  create_usb_cdc_1_task(&event_group);

  bind_info_task_to_core0();
  bind_usb_device_task_to_core0();
  bind_usb_cdc_0_task_to_core0();
  bind_usb_cdc_1_task_to_core0();

  vTaskStartScheduler();

  while (1) {
    tight_loop_contents();
  }

  return 0;
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
  printf("ERROR: Stack overflow in task %s!\n", pcTaskName);
  while (1) {
    ;
  };
}

void vApplicationTickHook(void) {}