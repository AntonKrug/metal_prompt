/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 20/05/2022
 */


#ifndef METAL_PROMPT_GLOBALS_H_
#define METAL_PROMPT_GLOBALS_H_

#include "config.h"

#include <stdbool.h>

#pragma mark Public variables
extern bool m_p_keep_runnning;

#ifdef M_P_UPTIME
extern bool m_p_benchmark_commands;
#endif

#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
extern char m_p_return_and_argument_buf[];
#endif


#endif /* METAL_PROMPT_GLOBALS_H_ */
