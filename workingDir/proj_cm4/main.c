/**
 * @file main.c
 * @brief DPS368 barometer demo entry point
 * @author Adam Shebani (adamsheb414@gmail.com)
 */
#include "main.h"
#include "drivers.h"
#include "tasks.h"


static void reset_btn_isr(void *arg, cyhal_gpio_event_t event)
{
    (void)arg; (void)event;
    NVIC_SystemReset();   // doesn't return
}

int main(void)
{
    cy_rslt_t rslt;

    /* Board init: clocks, power, default pin muxing. Always first. */
    rslt = cybsp_init();
    if (rslt != CY_RSLT_SUCCESS) CY_ASSERT(0);

    __enable_irq();

    /* Hook printf() up to the KitProg3 virtual COM port at 115200 8N1. */
    rslt = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,
                               CY_RETARGET_IO_BAUDRATE);
    if (rslt != CY_RSLT_SUCCESS) CY_ASSERT(0);

    printf("\x1b[2J\x1b[H");   /* ANSI clear-screen + home cursor */
    printf("=== DPS368 barometer demo ===\r\n");

    /* Bring up the I2C bus, sensor, queue, and the owner task. */
    rslt = barometer_task_init();
    if (rslt != CY_RSLT_SUCCESS)
    {
        printf("barometer_task_init failed: 0x%08lx\r\n",
               (unsigned long)rslt);
        CY_ASSERT(0);
    }

    cyhal_gpio_init(CYBSP_USER_BTN, CYHAL_GPIO_DIR_INPUT,
                  CYHAL_GPIO_DRIVE_PULLUP, 1);
    cyhal_gpio_register_callback(CYBSP_USER_BTN,
      &(cyhal_gpio_callback_data_t){ .callback = reset_btn_isr });
    cyhal_gpio_enable_event(CYBSP_USER_BTN, CYHAL_GPIO_IRQ_FALL, 3, true);

    /* Hand control to FreeRTOS. Never returns unless it can't start
     * (usually means configTOTAL_HEAP_SIZE is too small). */
    vTaskStartScheduler();

    CY_ASSERT(0);
    for(;;) {} //shouldn't ever hit
}