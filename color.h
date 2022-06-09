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

#ifdef __cplusplus
extern "C" {
#endif


#include "config.h"

#include <stdbool.h>


#ifdef M_P_CFG_COLORS

#pragma mark - Public variables

extern bool M_P_CFG_COLORS_ENABLED;


#pragma mark - Public functions outputing into the string buffer


extern void m_p_color_buf_default(char* buf);

#ifdef M_P_CFG_AUTOCOMPLETE
extern void m_p_color_buf_default_bold(char* buf);
extern void m_p_color_buf_command_namespace(char* buf);
extern void m_p_color_buf_command_name(char* buf);
extern void m_p_color_buf_gray(char* buf);
#endif // M_P_CFG_AUTOCOMPLETE


#pragma mark - Public functions outputing directly to the transport interface


extern void m_p_color_out_default(void);
extern void m_p_color_out_prompt(void);
extern void m_p_color_out_error(void);


#endif // M_P_CFG_COLORS


#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_METAL_PROMPT_COLOR_H_ */
