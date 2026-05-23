/**
 * @file main.h
 * @author Adam Shebani (adamsheb414@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2026-05-23
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef __MAIN_H__
#define __MAIN_H__

#include "cybsp.h"
#include "cyhal_hw_types.h"
#include "cyhal_gpio.h"
#include "cyhal_timer.h"
#include "cyhal_pwm.h"
#include "cyhal_adc.h"
#include "cyhal_uart.h"
#include "cyhal_i2c.h"
#include "cyhal_spi.h"
#include "cy_retarget_io.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/* Uncomment the line below to enable FreeRTOS in your applications*/

#if defined(USE_FREERTOS)
/* FreeRTOS Includes */
#include <FreeRTOS.h>
#include <event_groups.h>
#include <queue.h>
#include <semphr.h>
#include <task.h>
#endif

extern char NAME[];
extern char APP_DESCRIPTION[];

void app_init_hw(void);

void app_main(void);

#endif