/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */

/*
 * Generic transport interface, each transport implementation (uart, telnet ...)
 * needs to implement these functions for the metal prompt to be able to use
 * that specific transport layer.
 */


#ifndef METAL_PROMPT_TRANSPORT_INTERFACE_H_
#define METAL_PROMPT_TRANSPORT_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#pragma mark - Public functions for generic transport interface
extern void m_p_transport_out(const char* text);
extern void m_p_transport_out_characters(const char* characters, const unsigned int count);
extern void m_p_transport_out_ln(void);
extern bool m_p_transport_in(char* out_buffer);

#ifdef __cplusplus
}
#endif

#endif /* METAL_PROMPT_TRANSPORT_INTERFACE_H_ */
