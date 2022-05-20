/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#ifndef METAL_PROMPT_TRANSPORT_INTERFACE_H_
#define METAL_PROMPT_TRANSPORT_INTERFACE_H_

#include <stdbool.h>

#pragma mark Public functions
extern void m_p_transport_out(const char* text);
extern void m_p_transport_out_characters(const char* characters, uint32_t count);
extern void m_p_transport_out_ln(void);
extern bool m_p_transport_in(char* out_buffer);

#endif /* METAL_PROMPT_TRANSPORT_INTERFACE_H_ */
