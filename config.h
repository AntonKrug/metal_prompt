/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#ifndef METAL_PROMPT_CONFIG_H_
#define METAL_PROMPT_CONFIG_H_

#define M_P_COMMAND_PROMPT "metal_prompt:/$ "
#define M_P_COMMAND_NAME_LIMIT 255
#define M_P_INTERFACE_ALLOW_QUIT
#define M_P_RETURN_AND_ARGUMENT_STRING_ENABLE

#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
#define M_P_RETURN_AND_ARGUMENT_STRING_BUFFER_SIZE 64
#endif

// Comment this out when you want the list and it's arguments be triggered
// even when the command is already written in full
#define M_P_DO_NOT_LIST_ON_FULL_COMMANDS

// Enable/disable the uptime and benchmark commands
#define M_P_UPTIME
#define M_P_SYSTICK_FREQUENCY_1MS 1000


#endif /* METAL_PROMPT_CONFIG_H_ */
