#include <FreeRTOS.h>
#include <hardware/AT24C01.h>
#include <hardware/i2c.h>
#include <stdint.h>
#include <task.h>

#define AT24C01_ADDRESS 0x51

#define I2C_PORT i2c1

int at24c01_write_byte(uint8_t mem_address, uint8_t data) {
  uint8_t buf[2] = {mem_address, data};
  int result = i2c_write_blocking(I2C_PORT, AT24C01_ADDRESS, buf, 2, false);
  vTaskDelay(pdMS_TO_TICKS(10));
  return result;
}

int at24c01_read_byte(uint8_t mem_address, uint8_t *data_out) {
  int r = i2c_write_blocking(I2C_PORT, AT24C01_ADDRESS, &mem_address, 1, true);
  if (r < 0) return 0;

  return i2c_read_blocking(I2C_PORT, AT24C01_ADDRESS, data_out, 1, false);
}
