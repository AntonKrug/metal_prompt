/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 29/05/2022
 */

/* Project which is using metal prompt should include this header file. While
 * having inside its makefile (or other build management) define for one of the
 * transport implementations.
 */

#ifndef METAL_PROMPT_TRANSPORT_IMPLEMENTATION_H_
#define METAL_PROMPT_TRANSPORT_IMPLEMENTATION_H_

// Include all possible implementations, while the ifdefs inside them will
// select one specific implementation

#include "pf_fabric_core_uart_apb/implementation.h"
#include "pfsoc_mss_uart/implementation.h"

#endif /* METAL_PROMPT_TRANSPORT_IMPLEMENTATION_H_ */
