/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#ifndef METAL_PROMPT_CONFIG_H_
#define METAL_PROMPT_CONFIG_H_

#define TEST_COMMAND_PROMPT "metal_prompt:/$ "

#define TEST_COMMAND_NAME_LIMIT 255

// Comment this out when you want the list and it's arguments be triggered
// even when the command is already written in full
//#define TEST_DO_NOT_LIST_ON_FULL_COMMANDS


#define TEST_INTERFACE_ALLOW_QUIT

// Enable/disable the uptime and benchmark commands
//#define TEST_INTERFACE_UPTIME
#define SYSTICK_FREQUENCY_1MS 1000

#define TEST_INTERFACE_RETURN_AND_ARGUMENT_STRING_BUFFER_SIZE 100

#endif /* METAL_PROMPT_CONFIG_H_ */
