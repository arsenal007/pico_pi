#include <stdio.h>
#include <pico/stdlib.h>
#include <FreeRTOS.h>
#include <task.h>

uint32_t counter = 0;

void hello_task(void *pvParameters)
{
    while (1)
    {
        counter++;
        printf("Hello world from FreeRTOS %u\n", counter);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main()
{
    stdio_init_all();

    xTaskCreate(hello_task, "HelloTask", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1)
    {
        tight_loop_contents();
    }

    return 0;
}

void vApplicationMallocFailedHook(void)
{
    printf("ERROR: Malloc failed!\n");
    while (1)
        ;
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    printf("ERROR: Stack overflow in task %s!\n", pcTaskName);
    while (1)
        ;
}

void vApplicationTickHook(void)
{
}