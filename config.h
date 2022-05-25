/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */

/*
 * Global config to tweak, or enable/disable certain features of metal prompt
 */


#ifndef METAL_PROMPT_CONFIG_H_
#define METAL_PROMPT_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#define M_P_COMMAND_PROMPT "metal_prompt:/$ "
#define M_P_COMMAND_NAME_LIMIT 128
#define M_P_FORCE_OPTIMIZATION __attribute__((optimize("-Os")))
//#define M_P_FORCE_OPTIMIZATION // uncomment this line instead for no optimizations
#define M_P_ALLOW_QUIT
#define M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
#define M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
//#define M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
#define M_P_COLOR_ENABLE
#define M_P_LIST_AND_AUTOCOMPLETE_ENABLE

#ifdef M_P_LIST_AND_AUTOCOMPLETE_ENABLE
#define M_P_AUTOCOMPLETE_ON_EMPTY_COMMANDS
#endif

#define M_P_PRINT_DEC_ON_STARTUP true

#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
#define M_P_RETURN_AND_ARGUMENT_STRING_BUFFER_SIZE 48
#endif

// Comment this out when you want the list and it's arguments be triggered
// even when the command is already written in full
//#define M_P_DO_NOT_LIST_ON_FULL_COMMANDS


// Enable/disable the uptime and benchmark commands (see systick.c)
#define M_P_UPTIME

#ifdef __cplusplus
}
#endif

#endif /* METAL_PROMPT_CONFIG_H_ */
