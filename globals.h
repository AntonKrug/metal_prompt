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
extern bool metal_prompt_keep_runnning;

#ifdef METAL_PROMPT_UPTIME
extern bool metal_prompt_benchmark_commands;
#endif

#ifdef METAL_PROMPT_RETURN_AND_ARGUMENT_STRING_ENABLE
extern char metal_prompt_return_and_argument_buf[];
#endif


#endif /* METAL_PROMPT_GLOBALS_H_ */
