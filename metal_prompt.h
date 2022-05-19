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

#define METAL_PROMPT_VERSION "v0.1"


#pragma mark Public variables
extern bool test_keep_runnning;
extern bool test_benchmark_commands;


#pragma mark Public functions
extern void metal_prompt_cmd_line_generic(void);


#pragma mark Private functions
extern void metal_prompt_auto_complete(char* cmd, uint32_t* caret);


#pragma mark Private functions - command list handlers
extern void                 metal_prompt_list_begin(void);
extern bool                 metal_prompt_list_is_first(void);
extern bool                 metal_prompt_list_current_exist(void);
extern void                 metal_prompt_list_select_next(void);
extern metal_prompt_command metal_prompt_list_get_current_structure(void);
extern uint32_t             metal_prompt_list_get_current_string(char *buf, bool color);
extern void                 metal_prompt_list_get_current_string_arguments(char *buf);
extern void                 metal_prompt_list_align_command_for_args(char *cmd, uint32_t longest);
extern void                 metal_prompt_list_add_spaces(uint32_t current, uint32_t longest);
extern uint32_t             metal_prompt_list_get_longest_size(void);


#endif /* METAL_PROMPT_METAL_PROMPT_H_ */
