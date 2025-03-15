#include <stdio.h>
#include <pico/stdlib.h>
#include <FreeRTOS.h>
#include <task.h>

StaticTask_t xTaskBuffer;
StackType_t xStack[configMINIMAL_STACK_SIZE];

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

    xTaskCreateStatic(hello_task, "HelloTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, xStack, &xTaskBuffer);

    vTaskStartScheduler();

    while (1)
    {
        tight_loop_contents();
    }

    return 0;
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