/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */

/*
 * Prompt handlers, handling the user-input, parsing command's arguments,
 * invoking the commands and parsing command's return values.
 */


#ifndef METAL_PROMPT_METAL_PROMPT_H_
#define METAL_PROMPT_METAL_PROMPT_H_

#include "config.h"
#include "structures.h"

#include <stdbool.h>

#define M_P_VERSION "v0.1"


#pragma mark - Public functions


extern void  m_p_prompt_generic(void);
#ifdef M_P_LIST_AND_AUTOCOMPLETE_ENABLE
extern void  m_p_auto_complete(char* cmd, unsigned int* caret);
#endif


#endif /* METAL_PROMPT_METAL_PROMPT_H_ */
