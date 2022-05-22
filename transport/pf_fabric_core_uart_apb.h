/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */

/*
 * This is a transport implementation for Microchip - PolarFire
 * (and SmartFusion2) devices which can have instantiated CoreUART_apb
 * peripheral as a soft IP inside the FPGA fabric.
 */

#ifndef METAL_PROMPT_TRANSPORT_PF_FABRIC_CODE_UART_APB_H_
#define METAL_PROMPT_TRANSPORT_PF_FABRIC_CODE_UART_APB_H_

#include "drivers/fabric_ip/CoreUARTapb/core_uart_apb.h"

#pragma mark - Public functions
extern void m_p_cmd_line(UART_instance_t* port_to_be_used);

#endif /* METAL_PROMPT_TRANSPORT_PF_FABRIC_CODE_UART_APB_H_ */
