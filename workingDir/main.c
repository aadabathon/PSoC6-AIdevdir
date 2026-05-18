/**
 * @file main.c
 * @brief DPS368 barometer demo entry point
 * @author Adam Shebani (adamsheb414@gmail.com)
 */

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

#include "FreeRTOS.h"
#include "task.h"

#include "barometer_task.h"

#include <stdio.h>

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

    /* Hand control to FreeRTOS. Never returns unless it can't start
     * (usually means configTOTAL_HEAP_SIZE is too small). */
    vTaskStartScheduler();

    CY_ASSERT(0);
    for(;;) {}
}