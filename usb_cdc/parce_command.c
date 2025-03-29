#include <FreeRTOS.h>
#include <pico/bootrom.h>
#include <stdint.h>
#include <task.h>
#include <usb_cdc/parce_command.h>
#include <stdio.h>
#include <hardware/i2c.h>

typedef void (*CommandFunc)(void *args);

typedef struct
{
  const char *name;
  CommandFunc func;
  void *args;
} Command;

static void cmd_reset_usb_boot(void *args)
{ // Reset the USB boot
  (void)args;
  vTaskDelay(pdMS_TO_TICKS(1000));
  reset_usb_boot(0, 0);
}

static char buffer[1024];

static void show_tasks_stats(void *args)
{
  (void)args;
  printf("Running FreeRTOS version: %s (on Core %u)\n",
         tskKERNEL_VERSION_NUMBER, get_core_num());
  memset(buffer, 0, sizeof(buffer));
  vTaskList(buffer);
  printf("Task List:\n%s\n", buffer);
  memset(buffer, 0, sizeof(buffer));
  vTaskGetRunTimeStats(buffer);
  printf("Task Runtime Stats:\n%s\n", buffer);
}

#define I2C_PORT i2c1

static void i2c1_scan_bus(void *args)
{
  (void)args;
  printf("Scanning I2C bus...\n");
  uint8_t dummy;
  for (uint8_t addr = 0x08; addr <= 0x77; addr++)
  {
    int result = i2c_read_blocking(I2C_PORT, addr, &dummy, 1, false);
    if (result >= 0)
    {
      printf("Device found at address 0x%02X\n", addr);
    }
  }
  printf("I2C scan complete.\n");
}

Command usb_cdc_port_1_commands[] = {
    {"reset_usb_boot", &cmd_reset_usb_boot, NULL},
    {"stats", &show_tasks_stats, NULL},
    {"i2c_scan", &i2c1_scan_bus, NULL}};

const int command_port_1_count =
    sizeof(usb_cdc_port_1_commands) / sizeof(usb_cdc_port_1_commands[0]);

static void parse_buffer(const char *buffer, Command *commands,
                         uint32_t count)
{
  for (uint32_t i = 0; i < count; i++)
  {
    const char *command = commands[i].name;
    CommandFunc func = commands[i].func;
    void *args = commands[i].args;
    if (strstr(buffer, command))
    {
      func(args);
    }
  }
}

void usb_cdc_parce_command(const char *buffer, uint32_t size, uint8_t port)
{
  switch (port)
  {
  case 0:
    // parse_buffer(buffer, usb_cdc_port_0_commands, command_count);
    break;
  case 1:
    parse_buffer(buffer, usb_cdc_port_1_commands, command_port_1_count);
    break;
  default:
    break;
  }
}