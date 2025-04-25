#include <FreeRTOS.h>
#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <task.h>
#include <tasks/info.h>
#include <usb_cdc/cat_answer.h>
#include <libcat.h>

#define STACK_SIZE configMINIMAL_STACK_SIZE

static TaskHandle_t xHandle;

#define I2C_PORT i2c1
#define I2C_SDA_PIN 14
#define I2C_SCL_PIN 15

static void i2c1_init_custom(uint baudrate)
{
  i2c_init(I2C_PORT, baudrate);

  gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
}

static CAT_T cat_str;

static void set_frequency_vfo_a(uint32_t frequency)
{
  printf("Setting VFO A frequency to %u Hz\n", frequency);
}

static void info_task(void *pvParameters)
{
  (void)pvParameters;
  stdio_uart_init();
  i2c1_init_custom(100000); // 100kHz
  cat_str.answer_function = cat_answer;
  cat_str.set_frequency_vfo_a = set_frequency_vfo_a;
  cat_init(&cat_str);
  while (1)
  {
    vTaskDelay(pdMS_TO_TICKS(10000));
  }
}

void create_info_task(void)
{
  xTaskCreate(info_task, "info_task", STACK_SIZE, NULL, tskIDLE_PRIORITY,
              &xHandle);
}

void bind_info_task_to_core0(void)
{
  UBaseType_t uxCoreAffinityMask = 1;
  vTaskCoreAffinitySet(xHandle, uxCoreAffinityMask);
}