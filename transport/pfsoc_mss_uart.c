/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */

/* This is a transport implementation for Microchip - PolarFire SoC
 * devices which can use the microchip subsystem (mss) UART peripherals
 */

#include "pfsoc_mss_uart.h"

#include "interface.h"
#include "metal_prompt/config.h"
#include "metal_prompt/metal_prompt.h"

#include <stdint.h>


mss_uart_instance_t* test_interface_uart_instance;

#pragma mark - Public functions

M_P_FORCE_OPTIMIZATION
void m_p_cmd_line(mss_uart_instance_t* port_to_be_used, const char* application_name) {
    test_interface_uart_instance = port_to_be_used;

    m_p_cmd_line_generic();
}


#pragma mark - Public functions implementing the transport interface


M_P_FORCE_OPTIMIZATION
void m_p_transport_out(const char* text) {
    MSS_UART_polled_tx_string(test_interface_uart_instance, (const uint8_t*)text);
}


M_P_FORCE_OPTIMIZATION
void m_p_transport_out_characters(const char* characters, unsigned int count) {
    MSS_UART_polled_tx(test_interface_uart_instance, (const uint8_t*)characters, (uint32_t)count);
}


M_P_FORCE_OPTIMIZATION
void m_p_transport_out_ln(void) {
    m_p_transport_out("\r\n");
}


M_P_FORCE_OPTIMIZATION
bool m_p_transport_in(char* out_buffer) {
    uint32_t rx_count = MSS_UART_get_rx(test_interface_uart_instance, out_buffer, 1);

    return (rx_count > 0);
}
