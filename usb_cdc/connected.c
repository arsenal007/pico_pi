
#include <stdio.h>
#include <string.h>
#include <tusb.h>
#include <usb_cdc/connected.h>
#include <usb_cdc/parce_command.h>

void usb_cdc_connected(EventGroupHandle_t* p_event_group, uint8_t itf,
                       char* message_buffer, uint32_t message_buffer_size) {
  uint32_t received_size = tud_cdc_n_available(itf);

  if (received_size > 0) {
    uint32_t bytes_read =
        tud_cdc_n_read(itf, message_buffer, message_buffer_size);

    tud_cdc_n_read_flush(itf);

    uint32_t cdc_port = itf;

    usb_cdc_parce_command(message_buffer, bytes_read, cdc_port);
    printf("port: %u, received: %u bytes, message: %s\n", cdc_port, bytes_read,
           message_buffer);

    memset(message_buffer, 0, message_buffer_size);
  }
}