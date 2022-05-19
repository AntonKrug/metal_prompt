/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#ifndef METAL_PROMPT_COMMANDS_BUNDLED_H_
#define METAL_PROMPT_COMMANDS_BUNDLED_H_

#include "config.h"
#include "structures.h"

extern void     metal_prompt_bundled_configuration(void);
extern void     metal_prompt_bundled_help(void);
extern void     metal_prompt_bundled_list_all_tests(void);
extern void     metal_prompt_bundled_quit(void);

#ifdef METAL_PROMPT_UPTIME
extern void     metal_prompt_set_command_benchmark(void);
extern uint64_t metal_prompt_uptime(void);
#endif


#define METAL_PROMPT_BUNDLED_CONFIG_META    { "configuration",         METAL_PROMPT_COMMAND_TYPE_RET_VOID_ARG_VOID,   .void_no_args   = { &metal_prompt_bundled_configuration } }
#define METAL_PROMPT_BUNDLED_HELP_META      { "help",                  METAL_PROMPT_COMMAND_TYPE_RET_VOID_ARG_VOID,   .void_no_args   = { &metal_prompt_bundled_help } }
#define METAL_PROMPT_BUNDLED_LIST_META      { "list",                  METAL_PROMPT_COMMAND_TYPE_RET_VOID_ARG_VOID,   .void_no_args   = { &metal_prompt_bundled_list_all_tests } }
#define METAL_PROMPT_BUNDLED_BENCHMARK_META { "set_command_benchmark", METAL_PROMPT_COMMAND_TYPE_RET_VOID_ARG_VOID,   .void_no_args   = { &metal_prompt_set_command_benchmark } }
#define METAL_PROMPT_BUNDLED_UPTIME_META    { "uptime",                METAL_PROMPT_COMMAND_TYPE_RET_UINT64_ARG_VOID, .uint64_no_args = { &metal_prompt_uptime } }
#define METAL_PROMPT_BUNLDED_QUIT_META      { "quit",                  METAL_PROMPT_COMMAND_TYPE_RET_VOID_ARG_VOID,   .void_no_args   = { &metal_prompt_bundled_quit } }

extern metal_prompt_command metal_prompt_bundled_list[];

#define METAL_PROMPT_BUNDLED_GROUP { "", 6, metal_prompt_bundled_list }

#endif /* METAL_PROMPT_COMMANDS_BUNDLED_H_ */
