/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 20/05/2022
 */

/*
 * Set of variables which is meant to be accessible globally
 */

#ifndef METAL_PROMPT_GLOBALS_H_
#define METAL_PROMPT_GLOBALS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"

#include <stdbool.h>


#pragma mark - Public variables

extern char m_p_command_prompt[M_P_CFG_WHOLE_PROMPT_SIZE];

extern bool m_p_print_in_dec;

#ifdef M_P_CFG_ALLOW_QUIT
extern bool m_p_keep_runnning;
#endif

#ifdef M_P_CFG_UPTIME
extern bool m_p_benchmark_commands;
#endif

#ifdef M_P_CFG_TYPE_CHARS
extern char m_p_return_and_argument_buf[];
#endif

#ifdef __cplusplus
}
#endif

#endif /* METAL_PROMPT_GLOBALS_H_ */
