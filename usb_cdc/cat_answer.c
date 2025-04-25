
#include <usb_cdc/cat_answer.h>
#include <tusb.h>

void cat_answer(const char *buffer, size_t size)
{
    // Writes 'size' bytes from 'buffer' to the USB CDC interface 0 and returns the number of bytes successfully written.
    uint32_t bytes_written = tud_cdc_n_write(0, buffer, size);
    tud_cdc_n_write_flush(0);
}