/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#include "pfsoc_mss_uart.h"

#include "interface.h"
#include "metal_prompt/metal_prompt.h"

#include <stdint.h>


mss_uart_instance_t* test_interface_uart_instance;


void test_interface_cmd_line(mss_uart_instance_t* port_to_be_used, const char* application_name) {
    test_interface_uart_instance = port_to_be_used;

    test_interface_cmd_line_generic();
}


void test_interface_transport_out(const char* text) {
    MSS_UART_polled_tx_string(test_interface_uart_instance, (const uint8_t*)text);
}


void test_interface_transport_out_characters(const char* characters, uint32_t count) {
    MSS_UART_polled_tx(test_interface_uart_instance, (const uint8_t*)characters, (uint32_t)count);
}


void test_interface_transport_out_ln(void) {
    test_interface_transport_out("\r\n");
}


bool test_interface_transport_in(char* out_buffer) {
    uint32_t rx_count = MSS_UART_get_rx(test_interface_uart_instance, out_buffer, 1);

    return (rx_count > 0);
}
