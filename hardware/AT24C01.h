#ifndef HARDWARE_AT24C01_H
#define HARDWARE_AT24C01_H
#include <stdint.h>

int at24c01_write_byte(uint8_t mem_address, uint8_t data);
int at24c01_read_byte(uint8_t mem_address, uint8_t *data_out);

#endif