/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */

/*
 * The default set of prompt commands which are bundled with the metal prompt
 */

#include "commands_bundled.h"

#include "metal_prompt.h"
#include "transport/interface.h"
#include "systick.h"
#include "globals.h"
#include "config.h"
#include "color.h"

#include <stdlib.h>


#pragma mark - The default set prompt commands


#ifdef M_P_CFG_AUTOCOMPLETE_ENABLE
M_P_CFG_FORCE_OPTIMIZATION
void m_p_bundled_list_all_tests(void) {
	char cmd[255] = "";
	unsigned int carret = 0;


	// Auto-complete now list commands on the when empty command is given
	m_p_auto_complete(cmd, &carret);
}
#endif


M_P_CFG_FORCE_OPTIMIZATION
void m_p_bundled_configuration(void) {
	char optimization[5];
#ifdef NDEBUG
	m_p_transport_out("Configuration: Release\r\n");
#else
	m_p_transport_out("Configuration: Debug\r\n");
#endif

#ifdef __OPTIMIZE__
	m_p_transport_out("__OPTIMIZE__\r\n");
#endif

#ifdef __OPTIMIZE_SIZE__
	m_p_transport_out("__OPTIMIZE_SIZE__\r\n");
#endif

#ifndef __OPTIMIZE__
#ifndef __OPTIMIZE_SIZE__
	// Debug configuration
	m_p_transport_out("No speed nor size optimizations\r\n");
#endif
#endif

#ifdef __NO_INLINE__
	m_p_transport_out("__NO_INLINE__\r\n");
#endif

	// TODO: Display Hard/Soft float depending on the project defines (don't
	// know how to detect these from the auto-generated defined
}


M_P_CFG_FORCE_OPTIMIZATION
void m_p_bundled_help(void) {
    m_p_transport_out("Help: \r\n\r\n");
#ifdef M_P_CFG_ALLOW_QUIT
    m_p_transport_out(" Ctrl+C exits the metal prompt interface\r\n\r\n");
#endif
    m_p_transport_out(" Ctrl+S cleans the screen\r\n\r\n");
    m_p_transport_out(" Up arrow repeats the last command\r\n\r\n");
#ifdef M_P_CFG_AUTOCOMPLETE_ENABLE
    m_p_transport_out(" TAB auto-completes the command, if there are multiple options,\r\n");
    m_p_transport_out(" then it will list all options.\r\n");
#endif
}


#ifdef M_P_CFG_UPTIME

M_P_CFG_FORCE_OPTIMIZATION
uint32_t m_p_bundled_uptime(void) {
	return m_p_systick_uptime_ticks;
}


M_P_CFG_FORCE_OPTIMIZATION
void m_p_bundled_set_command_benchmark(void) {
    m_p_transport_out("Command benchmark mode enabled, now each command will print how long it took to execute.\r\n");
    m_p_benchmark_commands = true;
}

#endif


#ifdef M_P_CFG_ALLOW_QUIT
M_P_CFG_FORCE_OPTIMIZATION
void m_p_bundled_quit(void) {
    m_p_transport_out("Exiting...\r\n");
	m_p_keep_runnning = false;
}
#endif


#ifdef M_P_CFG_COLOR_ENABLE
M_P_CFG_FORCE_OPTIMIZATION
void m_p_bundled_set_prompt_color(uint32_t val) {
    if (val) {
        M_P_CFG_COLOR_ENABLEd = true;
    } else {
        M_P_CFG_COLOR_ENABLEd = false;
    }
}
#endif


#ifdef M_P_CFG_RET_AND_ARG_UINT
M_P_CFG_FORCE_OPTIMIZATION
void m_p_bundled_print_in_dec(unsigned int val) {
    if (val) {
        m_p_print_in_dec = true;
    } else {
        m_p_print_in_dec = false;
    }
}
#endif


const m_p_command m_p_bundled_list[] = {
        { "configuration",         M_P_CMD_RET_VOID_ARG_VOID,   .void_void   = { &m_p_bundled_configuration } },
        { "help",                  M_P_CMD_RET_VOID_ARG_VOID,   .void_void   = { &m_p_bundled_help } },

#ifdef M_P_CFG_AUTOCOMPLETE_ENABLE
        { "list",                  M_P_CMD_RET_VOID_ARG_VOID,   .void_void   = { &m_p_bundled_list_all_tests } },
#endif

#ifdef M_P_CFG_RET_AND_ARG_UINT
        { "print_in_dec"         , M_P_CMD_RET_VOID_ARG_UINT,   .void_uint   = { &m_p_bundled_print_in_dec } },
#endif

#ifdef M_P_CFG_UPTIME
        { "set_command_benchmark", M_P_CMD_RET_VOID_ARG_VOID,   .void_void   = { &m_p_bundled_set_command_benchmark } },
#ifdef M_P_CFG_RET_AND_ARG_UINT32
        { "uptime",                M_P_CMD_RET_UINT32_ARG_VOID, .uint32_void = { &m_p_bundled_uptime } },
#endif // M_P_CFG_RET_AND_ARG_UINT32
#endif // M_P_CFG_UPTIME

#if defined(M_P_CFG_COLOR_ENABLE) && defined(M_P_CFG_RET_AND_ARG_UINT32)
        { "set_prompt_color",      M_P_CMD_RET_VOID_ARG_UINT32, .void_uint32 = { &m_p_bundled_set_prompt_color } },
#endif

#ifdef M_P_CFG_ALLOW_QUIT
        { "quit",                  M_P_CMD_RET_VOID_ARG_VOID,   .void_void   = { &m_p_bundled_quit } },
#endif

        { NULL }  // Terminator of the list
};

