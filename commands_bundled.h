/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#ifndef METAL_PROMPT_COMMANDS_BUNDLED_H_
#define METAL_PROMPT_COMMANDS_BUNDLED_H_

#include "config.h"
#include "structures.h"

extern void     m_p_bundled_configuration(void);
extern void     m_p_bundled_help(void);
extern void     m_p_bundled_list_all_tests(void);
extern void     m_p_bundled_quit(void);

#ifdef M_P_UPTIME
extern void     m_p_bundled_set_command_benchmark(void);
extern uint32_t m_p_bundled_uptime(void);
#endif


#define M_P_BUNDLED_CONFIG_META    { "configuration",         M_P_COMMAND_TYPE_RET_VOID_ARG_VOID,   .void_void   = { &m_p_bundled_configuration } }
#define M_P_BUNDLED_HELP_META      { "help",                  M_P_COMMAND_TYPE_RET_VOID_ARG_VOID,   .void_void   = { &m_p_bundled_help } }
#define M_P_BUNDLED_LIST_META      { "list",                  M_P_COMMAND_TYPE_RET_VOID_ARG_VOID,   .void_void   = { &m_p_bundled_list_all_tests } }
#define M_P_BUNDLED_BENCHMARK_META { "set_command_benchmark", M_P_COMMAND_TYPE_RET_VOID_ARG_VOID,   .void_void   = { &m_p_bundled_set_command_benchmark } }
#define M_P_BUNDLED_UPTIME_META    { "uptime",                M_P_COMMAND_TYPE_RET_UINT32_ARG_VOID, .uint32_void = { &m_p_bundled_uptime } }
#define M_P_BUNLDED_QUIT_META      { "quit",                  M_P_COMMAND_TYPE_RET_VOID_ARG_VOID,   .void_void   = { &m_p_bundled_quit } }

extern m_p_command m_p_bundled_list[];

#define M_P_BUNDLED_GROUP { "", 6, m_p_bundled_list }

#endif /* METAL_PROMPT_COMMANDS_BUNDLED_H_ */
