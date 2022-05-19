/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#ifndef TEST_INTERFACE_TRANSPORT_PFSOC_MSS_UART_H_
#define TEST_INTERFACE_TRANSPORT_PFSOC_MSS_UART_H_

#include "drivers/mss/mss_mmuart/mss_uart.h"

extern void test_interface_cmd_line(mss_uart_instance_t* port_to_be_used, const char* application_name);

#endif /* TEST_INTERFACE_TRANSPORT_PFSOC_MSS_UART_H_ */
