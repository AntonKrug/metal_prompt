/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#include "commands_bundled.h"

#include "metal_prompt.h"
#include "transport/interface.h"
#include "systick.h"
#include "globals.h"

#include <stdlib.h>


void m_p_bundled_list_all_tests() {
	char cmd[255] = "";
	uint32_t carret = 0;


	// Auto-complete now list commands on the when empty command is given
	m_p_auto_complete(cmd, &carret);
}


void m_p_bundled_configuration() {
	char optimization[5];
#ifdef NDEBUG
	MSS_UART_polled_tx_string(uart_test_main, "Configuration: Release\r\n");
#else
	m_p_transport_out("Configuration: Debug\r\n");
#endif

#ifdef __OPTIMIZE__
	MSS_UART_polled_tx_string(uart_test_main, "__OPTIMIZE__\r\n");
#endif

#ifdef __OPTIMIZE_SIZE__
	MSS_UART_polled_tx_string(uart_test_main, "__OPTIMIZE_SIZE__\r\n");
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


void m_p_bundled_help() {
    m_p_transport_out("Help: \r\n\r\n");
    m_p_transport_out(" Ctrl+C exits the metal prompt interface\r\n\r\n");
    m_p_transport_out(" Ctrl+S cleans the screen\r\n\r\n");
    m_p_transport_out(" Up arrow repeats the last command\r\n\r\n");
    m_p_transport_out(" TAB auto-completes the command, if there are multiple options,\r\n");
    m_p_transport_out(" then it will list all options.\r\n");
}


#ifdef M_P_UPTIME
uint32_t m_p_bundled_uptime() {
	return m_p_systick_uptime_ticks;
}


void m_p_bundled_set_command_benchmark() {
    m_p_transport_out("Command benchmark mode enabled, now each command will print how long it took to execute.\r\n");
    m_p_benchmark_commands = true;
}
#endif


void m_p_bundled_quit() {
    m_p_transport_out("Exiting...\r\n");
	m_p_keep_runnning = false;
}


m_p_command m_p_bundled_list[] = {
        M_P_BUNDLED_CONFIG_META,
        M_P_BUNDLED_HELP_META,
        M_P_BUNDLED_LIST_META,
#ifdef M_P_UPTIME
        M_P_BUNDLED_BENCHMARK_META,
        M_P_BUNDLED_UPTIME_META,
#endif
        M_P_BUNLDED_QUIT_META
};
