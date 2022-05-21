/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#ifndef METAL_PROMPT_CONFIG_H_
#define METAL_PROMPT_CONFIG_H_

#define M_P_COMMAND_PROMPT "metal_prompt:/$ "
#define M_P_COMMAND_NAME_LIMIT 128
#define M_P_FORCE_OPTIMIZATION __attribute__((optimize("-Os")))
//#define M_P_FORCE_OPTIMIZATION // uncomment this line instead for no optimizations
#define M_P_ALLOW_QUIT
#define M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
#define M_P_COLOR_ENABLE

#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
#define M_P_RETURN_AND_ARGUMENT_STRING_BUFFER_SIZE 48
#endif

// Comment this out when you want the list and it's arguments be triggered
// even when the command is already written in full
//#define M_P_DO_NOT_LIST_ON_FULL_COMMANDS

#define M_P_AUTOCOMPLETE_ON_EMPTY_COMMANDS

// Enable/disable the uptime and benchmark commands (see systick.c)
#define M_P_UPTIME


#endif /* METAL_PROMPT_CONFIG_H_ */
