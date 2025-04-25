#include <FreeRTOS.h>
#include <hardware/i2c.h>
#include <pico/bootrom.h>
#include <stdint.h>
#include <stdio.h>
#include <task.h>
#include <usb_cdc/parce_command.h>
#include <hardware/eeprom_offsets.h>
#include <hardware/AT24C01.h>
#include <ctype.h>
#include <stdlib.h>

typedef void (*CommandFunc)(const char *args);

typedef struct
{
  const char *name;
  size_t command_string_length;
  CommandFunc func;
} Command;

static void cmd_reset_usb_boot(const char *args)
{ // Reset the USB boot
  (void)args;
  vTaskDelay(pdMS_TO_TICKS(1000));
  reset_usb_boot(0, 0);
}

static char buffer[1024];

static void show_tasks_stats(const char *args)
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

static void i2c1_scan_bus(const char *args)
{
  (void)args;

  printf("Scanning I2C bus...\n");
  uint8_t dummy = 0x00;
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

static void copy_hex_chars(char *dst, const char *src)
{
  size_t count = 0;

  while (*src != '\0' && count < 10)
  {

    if (isdigit(*src) || *src == 'x' || *src == 'X' ||
        (*src >= 'A' && *src <= 'F') || (*src >= 'a' && *src <= 'f'))
    {
      *dst = *src;
      dst++;
      count++;
    }
    src++;
  }

  *dst = '\0';
}

static int hex_to_binary(const char *hex_str, uint8_t *binary)
{

  if (hex_str[0] == '0' && (hex_str[1] == 'x' || hex_str[1] == 'X'))
  {

    char *endptr;
    unsigned long value = strtoul(hex_str + 2, &endptr, 16);

    if (*endptr == '\0' && value <= 0xFF)
    {
      *binary = (uint8_t)value;
      return 0;
    }
  }

  return -1;
}

static void set_si5351_address(const char *args)
{
  uint8_t new_address = 0x00;
  char str[16];
  copy_hex_chars(str, args);
  int conversion_result = hex_to_binary(str, &new_address);

  if (conversion_result == 0)
  {
    if ((new_address == 0x60) || (new_address == 0x6F))
    {
      at24c01_write_byte(SI5351_ADDRESS, new_address);
      at24c01_read_byte(SI5351_ADDRESS, &new_address);
      printf("SI5351 address set to 0x%02X\n", new_address);
    }
  }
  else
  {
    printf("Invalid command format. Use: si5351_address <address>\n");
    printf("Valid addresses: 0x60 or 0x6F\n");
  }
}

Command usb_cdc_port_1_commands[] = {
    {"reset_usb_boot", strlen("reset_usb_boot"), &cmd_reset_usb_boot},
    {"stats", strlen("stats"), &show_tasks_stats},
    {"i2c_scan", strlen("i2c_scan"), &i2c1_scan_bus},
    {"si5351_address", strlen("si5351_address"), &set_si5351_address}};

const int command_port_1_count =
    sizeof(usb_cdc_port_1_commands) / sizeof(usb_cdc_port_1_commands[0]);

static void parse_buffer(const char *buffer, Command *commands,
                         uint32_t count)
{
  for (uint32_t i = 0; i < count; i++)
  {
    const char *command = commands[i].name;
    CommandFunc func = commands[i].func;
    uint32_t command_length = commands[i].command_string_length;

    const char *found = strstr(buffer, command);

    if (found)
    {
      const char *arguments = found + command_length;
      func(arguments);
    }
  }
}

#include <libcat.h>

void usb_cdc_parce_command(const char *buffer, uint32_t size, uint8_t port)
{
  switch (port)
  {
  case 0:
  {
    // parse_buffer(buffer, usb_cdc_port_0_commands, command_count);
    for (uint32_t i = 0; i < size; i++)
    {
      cat_receive_cmd(buffer[i]);
    }
    break;
  }
  case 1:
    parse_buffer(buffer, usb_cdc_port_1_commands, command_port_1_count);
    break;
  default:
    break;
  }
}