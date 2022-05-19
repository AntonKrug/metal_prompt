/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#ifndef TEST_BUNDLED_H_
#define TEST_BUNDLED_H_

#include <metal_prompt/structures.h>

extern void     test_bundled_configuration(void);
extern void     test_bundled_help(void);
extern void     test_interface_bundled_list_all_tests(void);

#ifdef TEST_INTERFACE_UPTIME
extern void     test_set_command_benchmark(void);
extern uint64_t test_uptime(void);
#endif

extern void     test_set_command_benchmark(void);
extern uint64_t test_uptime(void);

extern void    test_bundled_quit(void);

#define TEST_BUNDLED_CONFIG_META    { "configuration",         TEST_INTERFACE_COMMAND_TYPE_RET_VOID_ARG_VOID,   .void_no_args  = { &test_bundled_configuration } }
#define TEST_BUNDLED_HELP_META      { "help",                  TEST_INTERFACE_COMMAND_TYPE_RET_VOID_ARG_VOID,   .void_no_args  = { &test_bundled_help } }
#define TEST_BUNDLED_LIST_META      { "list",                  TEST_INTERFACE_COMMAND_TYPE_RET_VOID_ARG_VOID,   .void_no_args  = { &test_interface_bundled_list_all_tests } }

#define TEST_BUNDLED_BENCHMARK_META { "set_command_benchmark", TEST_INTERFACE_COMMAND_TYPE_RET_VOID_ARG_VOID,   .void_no_args  = { &test_set_command_benchmark } }
// TODO: switch to 32bit uptime
#define TEST_BUNDLED_UPTIME_META    { "uptime",                TEST_INTERFACE_COMMAND_TYPE_RET_UINT64_ARG_VOID, .uint64_no_args = { &test_uptime } }

#define TEST_BUNLDED_QUIT_META      { "quit",                  TEST_INTERFACE_COMMAND_TYPE_RET_VOID_ARG_VOID,   .void_no_args  = { &test_bundled_quit } }

extern test_command test_bundled_list[];

#define TEST_BUNDLED_GROUP { "", 6, test_bundled_list }

#endif /* TEST_BUNDLED_H_ */
