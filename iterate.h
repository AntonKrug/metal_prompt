/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 20/05/2022
 */

/*
 * Functions which can iterate through the whole command structure and get the
 * the specific command structures, command names and print the command's syntax
 */

#ifndef METAL_PROMPT_ITERATE_H_
#define METAL_PROMPT_ITERATE_H_

#include "structures.h"

#include "config.h"

#include <stdint.h>
#include <stdbool.h>


#pragma mark - Public functions - iterate


extern void         m_p_iterate_begin(void);
extern bool         m_p_iterate_is_first(void);
extern bool         m_p_iterate_current_exists(void);
extern void         m_p_iterate_next(void);

extern m_p_command  m_p_iterate_get_current_structure(void);
extern unsigned int m_p_iterate_get_current_string(char *buf, bool use_color);

#ifdef M_P_LIST_AND_AUTOCOMPLETE_ENABLE
extern void         m_p_iterate_get_current_string_arguments(char *buf);
extern void         m_p_iterate_set_size_of_longest_command(void);
extern void         m_p_iterate_align_with_longest_command(unsigned int current_size);
#endif

#endif /* METAL_PROMPT_ITERATE_H_ */
