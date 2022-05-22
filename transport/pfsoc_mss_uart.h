/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */

/* This is a transport implementation for Microchip - PolarFire SoC
 * devices which can use the microchip subsystem (mss) UART peripherals
 */

#ifndef METAL_PROMPT_TRANSPORT_PFSOC_MSS_UART_H_
#define METAL_PROMPT_TRANSPORT_PFSOC_MSS_UART_H_

#include "drivers/mss/mss_mmuart/mss_uart.h"

#pragma mark - Public functions
extern void m_p_cmd_line(mss_uart_instance_t* port_to_be_used);

#endif /* METAL_PROMPT_TRANSPORT_PFSOC_MSS_UART_H_ */
