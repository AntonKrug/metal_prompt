/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#include "metal_prompt.h"

#include "commands_enabled.h"
#include "transport/interface.h"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

uint32_t longest_command = 0;
bool     test_keep_runnning = true;
bool     test_benchmark_commands = false;

uint32_t test_group_index;
uint32_t test_command_index;


void test_print_prompt(char *cmd) {
    test_interface_transport_out_ln();
    test_interface_transport_out("\033[1;36m");
    test_interface_transport_out(TEST_COMMAND_PROMPT);
    test_interface_transport_out("\033[0;39m");

	if (cmd != NULL) {
	    test_interface_transport_out(cmd);
	}
}


uint32_t test_execute_cmd(char *cmd) {
	char buf[TEST_COMMAND_NAME_LIMIT];
	char* ret_char_ptr;
	uint64_t ret_uint64;
	list_commands_begin();
	while (list_commands_current_exist()) {
		list_command_get_current_string(buf, false);
		if (strcmp(cmd, buf)  == 0) {
//			uint64_t begin = 0;
//			begin = test_systick_uptime_ticks;

			// TODO: parse syntax of arguments. Only if syntax is OK return 0
			test_command selected_command = list_command_get_current_structure();

			// TODO: Depending on type call it differently / parse return etc.
			test_interface_transport_out("\r\n");

			switch (selected_command.type) {
				case TEST_INTERFACE_COMMAND_TYPE_RET_CHAR_ARG_VOID:
					ret_char_ptr = selected_command.char_no_args.action();
					test_interface_transport_out(ret_char_ptr);
					break;

				case TEST_INTERFACE_COMMAND_TYPE_RET_UINT64_ARG_VOID:
				    ret_uint64 = selected_command.uint64_no_args.action();
					itoa(ret_uint64, buf, 16);
					test_interface_transport_out("0x");
					test_interface_transport_out(buf);
					break;

				case TEST_INTERFACE_COMMAND_TYPE_RET_CHAR_ARG_UINT64:

					break;

				case TEST_INTERFACE_COMMAND_TYPE_RET_VOID_ARG_VOID:
				default:
					selected_command.void_no_args.action();
					break;
			}
#ifdef TEST_INTERFACE_UPTIME
			if (test_benchmark_commands) {
				itoa(test_systick_uptime_ticks - begin, buf, 16);
				test_interface_transport_out("\r\nCommand took 0x");
				test_interface_transport_out(buf);
				test_interface_transport_out(" ticks to execute");
			}
#endif

			return 0; // command executed, return success
		}
		list_commands_select_next();
	}
	return 1; // no command found, return error
}


void test_auto_complete(char* cmd, uint32_t* caret) {
	char     first_command[TEST_COMMAND_NAME_LIMIT];
	char     buf[TEST_COMMAND_NAME_LIMIT];
	uint32_t common_location = TEST_COMMAND_NAME_LIMIT;
	uint32_t cmd_len         = strlen(cmd);
	uint32_t first           = 1;

	// Allow auto-complete to work even with empty commands
#if 0
	if (cmd_len == 0) {
		// No auto complete with empty line
		return;
	}
#endif


	list_commands_begin();

	while (list_commands_current_exist()) {
		// Go through all commands, if the start of the command matches the
		// current cmd line, then find how many unique characters can be added
		list_command_get_current_string(buf, false);
		if (strncmp(cmd, buf, cmd_len) == 0) {
			if (first) {
				strcpy(first_command, buf);
				first = 0;
			}

			uint32_t overlap = strspn(first_command, buf);
			if (overlap < common_location) {
				common_location = overlap;
			}
		}
		list_commands_select_next();
	}

	if (common_location == TEST_COMMAND_NAME_LIMIT) {
		// No common overlap found, probably wrong command / typo
		// Do not auto complete anything.
		return;
	}

	// Calculated where is the overlap between the commands and the cmd line
	if (cmd_len < common_location) {

		// If there are some common characters to add, then add them
		strncpy(cmd + cmd_len, first_command + cmd_len, common_location - cmd_len);
		cmd[common_location] = 0;

		// Display only the added difference as the previous part of the command
		// is already on the UART prompt
		test_interface_transport_out_characters(cmd + cmd_len, common_location - cmd_len);
		*caret = strlen(cmd);
	} else {
		// There is nothing to add with auto-complete

		// If it's full unique command = do nothing
#ifdef TEST_DO_NOT_LIST_ON_FULL_COMMANDS
		list_commands_begin();
		while (list_commands_current_exist()) {
			// Search if there is full match
			list_command_get_current_string(buf, false);
			if (strcmp(cmd, buf) == 0) {
				return;
			}
			list_commands_select_next();
		}
#endif

		// If not full command, then list all the options

		test_interface_transport_out_ln();
		list_commands_begin();
		while (list_commands_current_exist()) {
			// Go through all commands, if the start of the command matches the
			list_command_get_current_string(buf, false);
			if (strncmp(cmd, buf, cmd_len) == 0) {
			    test_interface_transport_out_ln();

			    test_interface_transport_out("\033[1;33m");
				// Print the current command
				uint32_t cmd_len = list_command_get_current_string(buf, true);
				test_interface_transport_out(buf);
				test_interface_transport_out("\033[0;39m");

				// Align it to the longest command
				list_command_add_spaces(cmd_len, longest_command);

				// Print the arguments
				list_command_get_current_string_arguments(buf);
				test_interface_transport_out(buf);

			}
			list_commands_select_next();
		}
		test_print_prompt(cmd);
	}
}


void evaluate_character(char character) {
	static char cmd[255];
	static char cmd_old[255]="";
	static uint32_t caret = 0;
	static uint32_t escape_sequence = 0;
	static uint32_t escape_sequence_next = 0;

	switch (character) {
		case 0x0a:
		case 0x0d:
			// \r line feed or carriage return \n  (enter)
			if (strlen(cmd)==0) {
				test_print_prompt(NULL);
				break;
			}


			if (test_execute_cmd(cmd)) {
			    test_interface_transport_out("\033[1;31m");
			    test_interface_transport_out("\r\n[ERROR] ");
                test_interface_transport_out("\033[0;39m");
			    test_interface_transport_out("Missing command or wrong arguments...\r\n");
			} else {
				// Executed correctly the command, clean the command line
			}

			// Store current command into history
			strcpy(cmd_old,cmd);

			// Clear current command
			cmd[0] = 0;
			caret = 0;

			test_print_prompt(NULL);
			break;

		case 0x09:
			// TAB
			test_auto_complete(cmd, &caret);
			break;

		case 0x08:
		case 0x7f:
			// Backspace
			if (caret > 0) {
				// only remove character when there is something to delete
				cmd[--caret] = 0;
				// Go back 1 character, print space and go back 1 character again
				test_interface_transport_out("\033[1D \033[1D");
			}
			break;

		case 0x03:
			// Ctrl + C = quit
			test_keep_runnning = 0;
			test_interface_transport_out("\r\nCtrl + C, exiting ...\r\n");
			break;

		case 0x13:
			// Ctrl + S = clear screen
		    test_interface_transport_out("\033[2J\033[H");
			test_print_prompt(cmd);
			break;

		case 0x0C:
			// Ctrl + L = recall history
			if (strlen(cmd) == 0) {
				// When prompt empty, then print at the same prompt
			    test_interface_transport_out(cmd_old);
			} else{
				//  Print completely new prompt
				test_print_prompt(cmd_old);
			}

			strcpy(cmd, cmd_old);
			caret = strlen(cmd);
			break;

		case 0x1B:
			// start of the escape sequence \033, but do not display anything yet
			if (escape_sequence == 0){
				escape_sequence_next = 1;
			}
			break;

		default:
			if (escape_sequence == 0) {
				// Regular character, write it down on the screen and to the buffer
				cmd[caret++] = character;
				cmd[caret]   = 0;
				test_interface_transport_out_characters(&character, 1);
			}

			if (escape_sequence == 1 && character == '[') {
				// Parsing second part of the escape sequence
				escape_sequence_next = 2;
			}
			else {
				escape_sequence_next = 0;
			}

			if (escape_sequence == 2 && character == 'A') {
				// Parsing last 3rd part of the sequence
				// Up arrow was pressed, copy previous command
				if (strlen(cmd) == 0) {
					// When prompt empty, then print at the same prompt
				    test_interface_transport_out(cmd_old);
				} else{
					//  Print completely new prompt
					test_print_prompt(cmd_old);
				}

				strcpy(cmd, cmd_old);
				caret = strlen(cmd);
				escape_sequence_next = 0;
			}

			break;
	}
	escape_sequence = escape_sequence_next;
}


void test_interface_cmd_line_generic() {
    test_interface_transport_out("\r\nTest Interface ");
    test_interface_transport_out(TEST_INTERFACE_VERSION);
    test_interface_transport_out("\r\n");
	test_print_prompt(NULL);
	test_keep_runnning = 1;

    // Get what is the longest command length
    longest_command = list_command_get_longest_size();

	while (test_keep_runnning) {
		char character;
		if (test_interface_transport_in(&character)) {
			evaluate_character(character);
		}
	}
}


void list_commands_begin() {
	test_group_index   = 0;
	test_command_index = 0;
}


uint32_t list_commands_is_first() {
	return (test_group_index == 0) && (test_command_index == 0);
}


uint32_t list_commands_current_exist() {
	if ( test_group_index < TESTS_ENABLED_SIZE &&
			test_command_index < tests_enabled[test_group_index].testsSize) {
		return 1;
	}
	return 0;
}


void list_commands_select_next() {
	test_command_index++;
	if (test_command_index >= tests_enabled[test_group_index].testsSize) {
		// Finished with current group, select new group
		test_command_index = 0;
		test_group_index++;  // no need to check here if it's valid group
	}
}


test_command list_command_get_current_structure() {
	return tests_enabled[test_group_index].tests[test_command_index];
}


uint32_t list_command_get_current_string(char *buf, bool color) {
	uint32_t group_len = strlen(tests_enabled[test_group_index].group_name);

	strcpy(buf,"");
	uint32_t ans = 0;

	if ( group_len != 0 ) {
        if (color) strcat(buf, "\033[1;33m");
		strcat(buf, tests_enabled[test_group_index].group_name);
        if (color) strcat(buf, "\033[1;39m");
		strcat(buf, ".");
        ans += strlen(tests_enabled[test_group_index].group_name) + 1;
	}
    if (color) strcat(buf, "\033[1;35m");
	strcat(buf, tests_enabled[test_group_index].tests[test_command_index].command);
	ans += strlen(tests_enabled[test_group_index].tests[test_command_index].command);
    if (color) strcat(buf, "\033[0;39m");

	return ans;
}


void list_command_get_current_string_arguments(char *buf) {
    strcpy(buf, "\033[1;30m");
	strcat(buf, "return(");
    strcat(buf, "\033[1;39m");

	switch (tests_enabled[test_group_index].tests[test_command_index].type) {
	    case TEST_INTERFACE_COMMAND_TYPE_RET_CHAR_ARG_VOID:
		case TEST_INTERFACE_COMMAND_TYPE_RET_CHAR_ARG_CHAR:
        case TEST_INTERFACE_COMMAND_TYPE_RET_CHAR_ARG_UINT32:
        case TEST_INTERFACE_COMMAND_TYPE_RET_CHAR_ARG_UINT64:
			strcat(buf, "char*   ");
			break;

        case TEST_INTERFACE_COMMAND_TYPE_RET_UINT32_ARG_VOID:
        case TEST_INTERFACE_COMMAND_TYPE_RET_UINT32_ARG_CHAR:
        case TEST_INTERFACE_COMMAND_TYPE_RET_UINT32_ARG_UINT32:
        case TEST_INTERFACE_COMMAND_TYPE_RET_UINT32_ARG_UINT64:
            strcat(buf, "uint32_t");
        break;

        case TEST_INTERFACE_COMMAND_TYPE_RET_UINT64_ARG_VOID:
        case TEST_INTERFACE_COMMAND_TYPE_RET_UINT64_ARG_CHAR:
        case TEST_INTERFACE_COMMAND_TYPE_RET_UINT64_ARG_UINT32:
        case TEST_INTERFACE_COMMAND_TYPE_RET_UINT64_ARG_UINT64:
			strcat(buf, "uint64_t");
			break;

		default:
			strcat(buf, "void    ");
			break;
	}

    strcat(buf, "\033[1;30m");
	strcat(buf, ") arg(");
    strcat(buf, "\033[1;39m");

	switch (tests_enabled[test_group_index].tests[test_command_index].type) {
	    case TEST_INTERFACE_COMMAND_TYPE_RET_VOID_ARG_CHAR:
	    case TEST_INTERFACE_COMMAND_TYPE_RET_CHAR_ARG_CHAR:
	    case TEST_INTERFACE_COMMAND_TYPE_RET_UINT32_ARG_CHAR:
	    case TEST_INTERFACE_COMMAND_TYPE_RET_UINT64_ARG_CHAR:
			strcat(buf, "char");
			break;

        case TEST_INTERFACE_COMMAND_TYPE_RET_VOID_ARG_UINT32:
        case TEST_INTERFACE_COMMAND_TYPE_RET_CHAR_ARG_UINT32:
        case TEST_INTERFACE_COMMAND_TYPE_RET_UINT32_ARG_UINT32:
        case TEST_INTERFACE_COMMAND_TYPE_RET_UINT64_ARG_UINT32:
            strcat(buf, "uint32_t");
            break;

        case TEST_INTERFACE_COMMAND_TYPE_RET_VOID_ARG_UINT64:
        case TEST_INTERFACE_COMMAND_TYPE_RET_CHAR_ARG_UINT64:
        case TEST_INTERFACE_COMMAND_TYPE_RET_UINT32_ARG_UINT64:
        case TEST_INTERFACE_COMMAND_TYPE_RET_UINT64_ARG_UINT64:
            strcat(buf, "uint64_t");
            break;

		default:
			strcat(buf, "void");
			break;
	}

    strcat(buf, "\033[1;30m");
	strcat(buf, ")");
    strcat(buf, "\033[0;39m");

}


// Find longest command in the set and return it's length
uint32_t list_command_get_longest_size(void) {
    char  buf[TEST_COMMAND_NAME_LIMIT];
	uint32_t longest = 0;

	list_commands_begin();
	while (list_commands_current_exist()) {
	    list_command_get_current_string(buf, false);
		uint32_t current = strlen(buf);
		if (longest < current) {
			longest = current;
		}
		list_commands_select_next();
	}
	return longest;
}


// Find length of current string and align it with longest
void list_command_align_command_for_args(char *cmd, uint32_t longest) {
    if (strlen(cmd)>longest) return; // do not align if the cmd is too long

	for (uint32_t i = 0; i <= longest - strlen(cmd); ++i) {
		// <= on purpose, to add space even on the longest commands
	    test_interface_transport_out(" ");
	}
}


void list_command_add_spaces(uint32_t current, uint32_t longest) {
    for (uint32_t i = 0; i <= longest - current; ++i) {
        // <= on purpose, to add space even on the longest commands
        test_interface_transport_out(" ");
    }
}
