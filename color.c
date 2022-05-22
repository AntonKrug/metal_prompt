/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 21/05/2022
 */

/*
 * Terminal VT100 color functionality, can be completely disabled in
 * compile-time (config.h) or on runtime 'set_prompt_colors'
 */

#include "color.h"

#include "transport/interface.h"

#include <string.h>

#ifdef M_P_COLOR_ENABLE

#pragma mark - Public variables

bool m_p_color_enabled = true;

#pragma mark - Public functions outputing into the string buffer


M_P_FORCE_OPTIMIZATION
void m_p_color_buf_default(char* buf) {
    if (m_p_color_enabled) strcat(buf, "\033[0;39m");
}


#ifdef M_P_LIST_AND_AUTOCOMPLETE_ENABLE

M_P_FORCE_OPTIMIZATION
void m_p_color_buf_default_bold(char* buf) {
    if (m_p_color_enabled) strcat(buf, "\033[1;39m");
}


M_P_FORCE_OPTIMIZATION
void m_p_color_buf_command_namespace(char* buf) {
    if (m_p_color_enabled) strcat(buf, "\033[1;35m");
}


M_P_FORCE_OPTIMIZATION
void m_p_color_buf_command_name(char* buf) {
    if (m_p_color_enabled) strcat(buf, "\033[1;33m");
}


M_P_FORCE_OPTIMIZATION
void m_p_color_buf_gray(char* buf) {
    if (m_p_color_enabled) strcat(buf, "\033[1;30m");
}

#endif // M_P_LIST_AND_AUTOCOMPLETE_ENABLE


#pragma mark - Public functions outputing directly to the transport interface


M_P_FORCE_OPTIMIZATION
void m_p_color_out_default(void) {
    if (m_p_color_enabled) m_p_transport_out("\033[0;39m");
}


M_P_FORCE_OPTIMIZATION
void m_p_color_out_prompt(void) {
    if (m_p_color_enabled) m_p_transport_out("\033[1;36m");
}


M_P_FORCE_OPTIMIZATION
void m_p_color_out_error(void) {
    if (m_p_color_enabled) m_p_transport_out("\033[1;31m");
}


#ifdef M_P_LIST_AND_AUTOCOMPLETE_ENABLE
M_P_FORCE_OPTIMIZATION
void m_p_color_out_namespace(void) {
    if (m_p_color_enabled) m_p_transport_out("\033[1;35m");
}
#endif // M_P_LIST_AND_AUTOCOMPLETE_ENABLE


#endif // M_P_COLOR_ENABLE

