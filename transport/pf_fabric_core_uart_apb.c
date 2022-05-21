/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */

/* This is a transport implementation for Microchip - PolarFire
 * (and SmartFusion2) devices which can have instantiated CoreUART_apb
 * peripheral as a soft IP inside the FPGA fabric.
 */

#include "pf_fabric_core_uart_apb.h"

#include "interface.h"
#include "metal_prompt/config.h"
#include "metal_prompt/metal_prompt.h"

#include <stdint.h>


UART_instance_t* test_interface_uart_instance;

#pragma mark - Public functions

M_P_FORCE_OPTIMIZATION
void m_p_cmd_line(UART_instance_t* port_to_be_used, const char* application_name) {
    test_interface_uart_instance = port_to_be_used;

    m_p_cmd_line_generic();
}


#pragma mark - Public functions implementing the transport interface


M_P_FORCE_OPTIMIZATION
void m_p_transport_out(const char* text) {
    UART_polled_tx_string(test_interface_uart_instance, (const uint8_t*)text);
}


M_P_FORCE_OPTIMIZATION
void m_p_transport_out_characters(const char* characters, unsigned int count) {
    UART_fill_tx_fifo(test_interface_uart_instance, (const uint8_t*)characters, (uint32_t)count);
}


M_P_FORCE_OPTIMIZATION
void m_p_transport_out_ln(void) {
    m_p_transport_out("\r\n");
}


M_P_FORCE_OPTIMIZATION
bool m_p_transport_in(char* out_buffer) {
    uint32_t rx_count = UART_get_rx(test_interface_uart_instance, out_buffer, 1);

    return (rx_count > 0);
}
