/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#ifndef METAL_PROMPT_METAL_PROMPT_H_
#define METAL_PROMPT_METAL_PROMPT_H_

#include "config.h"
#include "structures.h"

#include <stdbool.h>

#define M_P_VERSION "v0.1"


#pragma mark Public functions
extern void        m_p_cmd_line_generic(void);

#pragma mark Private functions
extern void        m_p_auto_complete(char* cmd, uint32_t* caret);
extern void        m_p_print_prompt(char *cmd);

#pragma mark Private functions - command list handlers
extern void        m_p_evaluate_character(char character);
extern void        m_p_iterate_begin(void);
extern bool        m_p_iterate_is_first(void);
extern bool        m_p_iterate_current_exists(void);
extern void        m_p_iterate_next(void);
extern m_p_command m_p_iterate_get_current_structure(void);
extern uint32_t    m_p_iterate_get_current_string(char *buf, bool color);
extern void        m_p_iterate_get_current_string_arguments(char *buf);
extern void        m_p_iterate_align_with_longest_command(uint32_t current_size);
extern uint32_t    m_p_iterate_get_size_of_longest_command(void);


#endif /* METAL_PROMPT_METAL_PROMPT_H_ */
