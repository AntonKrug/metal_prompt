/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#include "commands_bundled.h"
#include "metal_prompt.h"
#include "transport/interface.h"
#include <stdlib.h>


void test_interface_bundled_list_all_tests() {
	char cmd[255] = "";
	uint32_t carret = 0;


	// Auto-complete now list commands on the when empty command is given
	test_auto_complete(cmd, &carret);

}


void test_bundled_configuration() {
	char optimization[5];
#ifdef NDEBUG
	MSS_UART_polled_tx_string(uart_test_main, "Configuration: Release\r\n");
#else
	test_interface_transport_out("Configuration: Debug\r\n");
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
	test_interface_transport_out("No speed nor size optimizations\r\n");
#endif
#endif

#ifdef __NO_INLINE__
	test_interface_transport_out("__NO_INLINE__\r\n");
#endif

	// TODO: Display Hard/Soft float depending on the project defines (don't
	// know how to detect these from the auto-generated defined
}


void test_bundled_help() {
    test_interface_transport_out("Help: \r\n");
    test_interface_transport_out(" Ctrl+C exits the test interface\r\n");
    test_interface_transport_out(" Ctrl+S cleans the screen\r\n");
    test_interface_transport_out(" Up arrow repeats the last command\r\n");
    test_interface_transport_out(" Use TAB for auto-complete, or to list all possible commands\r\n");
}


#ifdef TEST_INTERFACE_UPTIME
uint64_t test_uptime() {
	return test_systick_uptime_ticks;
}

void test_set_command_benchmark() {
    test_benchmark_commands = 1;
}
#endif


uint64_t test_uptime() {
    return 0;
}

void test_set_command_benchmark() {
    test_interface_transport_out("Command benchmark mode enabled, now each command will print how long it took to execute.\r\n");
}


void test_bundled_quit() {
    test_interface_transport_out("Exiting...\r\n");
	test_keep_runnning = 0;
}


test_command test_bundled_list[] = {
		TEST_BUNDLED_CONFIG_META,
		TEST_BUNDLED_HELP_META,
		TEST_BUNDLED_LIST_META,
#ifdef TEST_INTERFACE_UPTIME
        TEST_BUNDLED_BENCHMARK_META,
        TEST_BUNDLED_UPTIME_META,
#endif
		TEST_BUNLDED_QUIT_META
};
