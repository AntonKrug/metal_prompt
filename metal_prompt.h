/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#ifndef METAL_PROMPT_METAL_PROMPT_H_
#define METAL_PROMPT_METAL_PROMPT_H_

#include "config.h"
#include "structures.h"

#include <stdbool.h>

#define TEST_INTERFACE_VERSION "v0.1"

// Comment this out when you want the list and it's arguments be triggered
// even when the command is already written in full
//#define TEST_DO_NOT_LIST_ON_FULL_COMMANDS


extern bool test_keep_runnning;
extern bool test_benchmark_commands;



extern void metal_prompt_cmd_line_generic(void);



extern void metal_prompt_list_commands_begin(void);

extern uint32_t metal_prompt_list_commands_is_first(void);

extern uint32_t metal_prompt_list_commands_current_exist(void);

extern void metal_prompt_list_commands_select_next(void);

extern test_command metal_prompt_list_command_get_current_structure(void);

extern uint32_t metal_prompt_list_command_get_current_string(char *buf, bool color);

extern void metal_prompt_list_command_get_current_string_arguments(char *buf);

extern void metal_prompt_list_command_align_command_for_args(char *cmd, uint32_t longest);

extern void metal_prompt_list_command_add_spaces(uint32_t current, uint32_t longest);

extern uint32_t metal_prompt_list_command_get_longest_size(void);



void metal_prompt_auto_complete(char* cmd, uint32_t* caret);


#endif /* METAL_PROMPT_METAL_PROMPT_H_ */
