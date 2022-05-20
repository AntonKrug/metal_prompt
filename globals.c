/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 20/05/2022
 */

#include "globals.h"

#pragma mark Public variables
bool     metal_prompt_keep_runnning      = true;

#ifdef METAL_PROMPT_UPTIME
bool     metal_prompt_benchmark_commands = false;
#endif

#ifdef METAL_PROMPT_RETURN_AND_ARGUMENT_STRING_ENABLE
char     metal_prompt_return_and_argument_buf[METAL_PROMPT_RETURN_AND_ARGUMENT_STRING_BUFFER_SIZE];
#endif
