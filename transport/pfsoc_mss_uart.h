/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#ifndef METAL_PROMPT_TRANSPORT_PFSOC_MSS_UART_H_
#define METAL_PROMPT_TRANSPORT_PFSOC_MSS_UART_H_

#include "drivers/mss/mss_mmuart/mss_uart.h"

#pragma mark Public functions
extern void metal_prompt_cmd_line(mss_uart_instance_t* port_to_be_used, const char* application_name);

#endif /* METAL_PROMPT_TRANSPORT_PFSOC_MSS_UART_H_ */
