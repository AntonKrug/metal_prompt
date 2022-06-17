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

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"
#include "structures.h"

#include <stdbool.h>

#pragma mark - Public functions


extern void  m_p_prompt_generic(void);
#ifdef M_P_CFG_AUTOCOMPLETE
extern void  m_p_auto_complete(char* cmd, unsigned int* caret);
#endif

#ifdef __cplusplus
}
#endif

#endif /* METAL_PROMPT_METAL_PROMPT_H_ */
