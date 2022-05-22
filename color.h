/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 21/05/2022
 */

/*
 * Terminal VT100 color functionality, can be completely disabled in
 * compile-time (config.h) or on runtime 'set_prompt_colors'
 */

#ifndef METAL_PROMPT_COLOR_H_
#define METAL_PROMPT_COLOR_H_

#include "config.h"

#include <stdbool.h>

#ifdef M_P_COLOR_ENABLE

#pragma mark - Public variables

extern bool m_p_color_enabled;

#pragma mark - Public functions outputing into the string buffer


extern void m_p_color_buf_default(char* buf);

#ifdef M_P_LIST_AND_AUTOCOMPLETE_ENABLE
extern void m_p_color_buf_default_bold(char* buf);
extern void m_p_color_buf_command_namespace(char* buf);
extern void m_p_color_buf_command_name(char* buf);
extern void m_p_color_buf_gray(char* buf);
#endif // M_P_LIST_AND_AUTOCOMPLETE_ENABLE


#pragma mark - Public functions outputing directly to the transport interface


extern void m_p_color_out_default(void);
extern void m_p_color_out_prompt(void);
extern void m_p_color_out_error(void);

#ifdef M_P_LIST_AND_AUTOCOMPLETE_ENABLE
extern void m_p_color_out_namespace(void);
#endif  // M_P_LIST_AND_AUTOCOMPLETE_ENABLE


#endif // M_P_COLOR_ENABLE


#endif /* PLATFORM_METAL_PROMPT_COLOR_H_ */
