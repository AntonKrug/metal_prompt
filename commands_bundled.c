/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#include "commands_bundled.h"

#include "metal_prompt.h"
#include "transport/interface.h"
#include "systick.h"

#include <stdlib.h>


void metal_prompt_bundled_list_all_tests() {
	char cmd[255] = "";
	uint32_t carret = 0;


	// Auto-complete now list commands on the when empty command is given
	metal_prompt_auto_complete(cmd, &carret);
}


void metal_prompt_bundled_configuration() {
	char optimization[5];
#ifdef NDEBUG
	MSS_UART_polled_tx_string(uart_test_main, "Configuration: Release\r\n");
#else
	metal_prompt_transport_out("Configuration: Debug\r\n");
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
	metal_prompt_transport_out("No speed nor size optimizations\r\n");
#endif
#endif

#ifdef __NO_INLINE__
	metal_prompt_transport_out("__NO_INLINE__\r\n");
#endif

	// TODO: Display Hard/Soft float depending on the project defines (don't
	// know how to detect these from the auto-generated defined
}


void metal_prompt_bundled_help() {
    metal_prompt_transport_out("Help: \r\n");
    metal_prompt_transport_out(" Ctrl+C exits the test interface\r\n");
    metal_prompt_transport_out(" Ctrl+S cleans the screen\r\n");
    metal_prompt_transport_out(" Up arrow repeats the last command\r\n");
    metal_prompt_transport_out(" Use TAB for auto-complete, or to list all possible commands\r\n");
}


#ifdef METAL_PROMPT_UPTIME
uint32_t metal_prompt_uptime() {
	return metal_prompt_systick_uptime_ticks;
}


void metal_prompt_set_command_benchmark() {
    metal_prompt_transport_out("Command benchmark mode enabled, now each command will print how long it took to execute.\r\n");
    metal_prompt_benchmark_commands = true;
}
#endif


void metal_prompt_bundled_quit() {
    metal_prompt_transport_out("Exiting...\r\n");
	metal_prompt_keep_runnning = false;
}


metal_prompt_command metal_prompt_bundled_list[] = {
        METAL_PROMPT_BUNDLED_CONFIG_META,
        METAL_PROMPT_BUNDLED_HELP_META,
        METAL_PROMPT_BUNDLED_LIST_META,
#ifdef METAL_PROMPT_UPTIME
        METAL_PROMPT_BUNDLED_BENCHMARK_META,
        METAL_PROMPT_BUNDLED_UPTIME_META,
#endif
        METAL_PROMPT_BUNLDED_QUIT_META
};
