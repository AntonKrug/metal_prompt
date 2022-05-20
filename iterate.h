/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 20/05/2022
 */

#ifndef METAL_PROMPT_ITERATE_H_
#define METAL_PROMPT_ITERATE_H_

#include "structures.h"

#include <stdint.h>
#include <stdbool.h>


#pragma mark Public functions

extern void        m_p_iterate_begin(void);
extern bool        m_p_iterate_is_first(void);
extern bool        m_p_iterate_current_exists(void);
extern void        m_p_iterate_next(void);
extern m_p_command m_p_iterate_get_current_structure(void);
extern uint32_t    m_p_iterate_get_current_string(char *buf, bool color);
extern void        m_p_iterate_get_current_string_arguments(char *buf);
extern void        m_p_iterate_set_size_of_longest_command(void);
extern void        m_p_iterate_align_with_longest_command(uint32_t current_size);

#endif /* METAL_PROMPT_ITERATE_H_ */
