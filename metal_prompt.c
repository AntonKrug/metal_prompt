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

#pragma mark Public variables
bool     test_keep_runnning      = true;
bool     test_benchmark_commands = false;


#pragma mark Private variables
uint32_t list_group_index   = 0;
uint32_t list_command_index = 0;
uint32_t longest_command    = 0;


#pragma mark Public functions
void metal_prompt_print_prompt(char *cmd) {
    metal_prompt_transport_out_ln();
    metal_prompt_transport_out("\033[1;36m");
    metal_prompt_transport_out(METAL_PROMPT_COMMAND_PROMPT);
    metal_prompt_transport_out("\033[0;39m");

	if (cmd != NULL) {
	    metal_prompt_transport_out(cmd);
	}
}

#pragma mark Private functions
uint32_t metal_prompt_execute_cmd(char *cmd) {
	char buf[METAL_PROMPT_COMMAND_NAME_LIMIT];
	char* ret_char_ptr;
	uint64_t ret_uint64;
	metal_prompt_list_begin();
	while (metal_prompt_list_current_exist()) {
		metal_prompt_list_get_current_string(buf, false);
		if (strcmp(cmd, buf)  == 0) {
//			uint64_t begin = 0;
//			begin = test_systick_uptime_ticks;

			// TODO: parse syntax of arguments. Only if syntax is OK return 0
			metal_prompt_command selected_command = metal_prompt_list_get_current_structure();

			// TODO: Depending on type call it differently / parse return etc.
			metal_prompt_transport_out("\r\n");

			switch (selected_command.type) {
				case METAL_PROMPT_COMMAND_TYPE_RET_CHAR_ARG_VOID:
					ret_char_ptr = selected_command.char_no_args.action();
					metal_prompt_transport_out(ret_char_ptr);
					break;

				case METAL_PROMPT_COMMAND_TYPE_RET_UINT64_ARG_VOID:
				    ret_uint64 = selected_command.uint64_no_args.action();
					itoa(ret_uint64, buf, 16);
					metal_prompt_transport_out("0x");
					metal_prompt_transport_out(buf);
					break;

				case METAL_PROMPT_COMMAND_TYPE_RET_CHAR_ARG_UINT64:

					break;

				case METAL_PROMPT_COMMAND_TYPE_RET_VOID_ARG_VOID:
				default:
					selected_command.void_no_args.action();
					break;
			}
#ifdef TEST_INTERFACE_UPTIME
			if (test_benchmark_commands) {
				itoa(test_systick_uptime_ticks - begin, buf, 16);
				metal_prompt_transport_out("\r\nCommand took 0x");
				metal_prompt_transport_out(buf);
				metal_prompt_transport_out(" ticks to execute");
			}
#endif

			return 0; // command executed, return success
		}
		metal_prompt_list_select_next();
	}
	return 1; // no command found, return error
}


void metal_prompt_auto_complete(char* cmd, uint32_t* caret) {
	char     first_command[METAL_PROMPT_COMMAND_NAME_LIMIT];
	char     buf[METAL_PROMPT_COMMAND_NAME_LIMIT];
	uint32_t common_location = METAL_PROMPT_COMMAND_NAME_LIMIT;
	uint32_t cmd_len         = strlen(cmd);
	uint32_t first           = 1;

	// Allow auto-complete to work even with empty commands
#if 0
	if (cmd_len == 0) {
		// No auto complete with empty line
		return;
	}
#endif


	metal_prompt_list_begin();

	while (metal_prompt_list_current_exist()) {
		// Go through all commands, if the start of the command matches the
		// current cmd line, then find how many unique characters can be added
		metal_prompt_list_get_current_string(buf, false);
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
		metal_prompt_list_select_next();
	}

	if (common_location == METAL_PROMPT_COMMAND_NAME_LIMIT) {
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
		metal_prompt_transport_out_characters(cmd + cmd_len, common_location - cmd_len);
		*caret = strlen(cmd);
	} else {
		// There is nothing to add with auto-complete

		// If it's full unique command = do nothing
#ifdef TEST_DO_NOT_LIST_ON_FULL_COMMANDS
		metal_prompt_list_begin();
		while (metal_prompt_list_current_exist()) {
			// Search if there is full match
			metal_prompt_list_get_current_string(buf, false);
			if (strcmp(cmd, buf) == 0) {
				return;
			}
			metal_prompt_list_select_next();
		}
#endif

		// If not full command, then list all the options

		metal_prompt_transport_out_ln();
		metal_prompt_list_begin();
		while (metal_prompt_list_current_exist()) {
			// Go through all commands, if the start of the command matches the
			metal_prompt_list_get_current_string(buf, false);
			if (strncmp(cmd, buf, cmd_len) == 0) {
			    metal_prompt_transport_out_ln();

			    metal_prompt_transport_out("\033[1;33m");
				// Print the current command
				uint32_t cmd_len = metal_prompt_list_get_current_string(buf, true);
				metal_prompt_transport_out(buf);
				metal_prompt_transport_out("\033[0;39m");

				// Align it to the longest command
				metal_prompt_list_add_spaces(cmd_len, longest_command);

				// Print the arguments
				metal_prompt_list_get_current_string_arguments(buf);
				metal_prompt_transport_out(buf);

			}
			metal_prompt_list_select_next();
		}
		metal_prompt_print_prompt(cmd);
	}
}


void metal_prompt_evaluate_character(char character) {
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
				metal_prompt_print_prompt(NULL);
				break;
			}


			if (metal_prompt_execute_cmd(cmd)) {
			    metal_prompt_transport_out("\033[1;31m");
			    metal_prompt_transport_out("\r\n[ERROR] ");
                metal_prompt_transport_out("\033[0;39m");
			    metal_prompt_transport_out("Missing command or wrong arguments...\r\n");
			} else {
				// Executed correctly the command, clean the command line
			}

			// Store current command into history
			strcpy(cmd_old,cmd);

			// Clear current command
			cmd[0] = 0;
			caret = 0;

			metal_prompt_print_prompt(NULL);
			break;

		case 0x09:
			// TAB
			metal_prompt_auto_complete(cmd, &caret);
			break;

		case 0x08:
		case 0x7f:
			// Backspace
			if (caret > 0) {
				// only remove character when there is something to delete
				cmd[--caret] = 0;
				// Go back 1 character, print space and go back 1 character again
				metal_prompt_transport_out("\033[1D \033[1D");
			}
			break;

		case 0x03:
			// Ctrl + C = quit
			test_keep_runnning = 0;
			metal_prompt_transport_out("\r\nCtrl + C, exiting ...\r\n");
			break;

		case 0x13:
			// Ctrl + S = clear screen
		    metal_prompt_transport_out("\033[2J\033[H");
			metal_prompt_print_prompt(cmd);
			break;

		case 0x0C:
			// Ctrl + L = recall history
			if (strlen(cmd) == 0) {
				// When prompt empty, then print at the same prompt
			    metal_prompt_transport_out(cmd_old);
			} else{
				//  Print completely new prompt
				metal_prompt_print_prompt(cmd_old);
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
				metal_prompt_transport_out_characters(&character, 1);
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
				    metal_prompt_transport_out(cmd_old);
				} else{
					//  Print completely new prompt
					metal_prompt_print_prompt(cmd_old);
				}

				strcpy(cmd, cmd_old);
				caret = strlen(cmd);
				escape_sequence_next = 0;
			}

			break;
	}
	escape_sequence = escape_sequence_next;
}


void metal_prompt_cmd_line_generic() {
    metal_prompt_transport_out("\r\nTest Interface ");
    metal_prompt_transport_out(METAL_PROMPT_VERSION);
    metal_prompt_transport_out("\r\n");
	metal_prompt_print_prompt(NULL);
	test_keep_runnning = 1;

    // Get what is the longest command length
    longest_command = metal_prompt_list_get_longest_size();

	while (test_keep_runnning) {
		char character;
		if (metal_prompt_transport_in(&character)) {
			metal_prompt_evaluate_character(character);
		}
	}
}


void metal_prompt_list_begin() {
	list_group_index   = 0;
	list_command_index = 0;
}


uint32_t metal_prompt_list_is_first() {
	return (list_group_index == 0) && (list_command_index == 0);
}


uint32_t metal_prompt_list_current_exist() {
	if ( list_group_index < METAL_PROMPT_ENABLED_SIZE &&
			list_command_index < metal_prompt_commands_enabled[list_group_index].testsSize) {
		return 1;
	}
	return 0;
}


void metal_prompt_list_select_next() {
	list_command_index++;
	if (list_command_index >= metal_prompt_commands_enabled[list_group_index].testsSize) {
		// Finished with current group, select new group
		list_command_index = 0;
		list_group_index++;  // no need to check here if it's valid group
	}
}


metal_prompt_command metal_prompt_list_get_current_structure() {
	return metal_prompt_commands_enabled[list_group_index].tests[list_command_index];
}


uint32_t metal_prompt_list_get_current_string(char *buf, bool color) {
	uint32_t group_len = strlen(metal_prompt_commands_enabled[list_group_index].group_name);

	strcpy(buf,"");
	uint32_t ans = 0;

	if ( group_len != 0 ) {
        if (color) strcat(buf, "\033[1;33m");
		strcat(buf, metal_prompt_commands_enabled[list_group_index].group_name);
        if (color) strcat(buf, "\033[1;39m");
		strcat(buf, ".");
        ans += strlen(metal_prompt_commands_enabled[list_group_index].group_name) + 1;
	}
    if (color) strcat(buf, "\033[1;35m");
	strcat(buf, metal_prompt_commands_enabled[list_group_index].tests[list_command_index].command);
	ans += strlen(metal_prompt_commands_enabled[list_group_index].tests[list_command_index].command);
    if (color) strcat(buf, "\033[0;39m");

	return ans;
}


void metal_prompt_list_get_current_string_arguments(char *buf) {
    strcpy(buf, "\033[1;30m");
	strcat(buf, "return(");
    strcat(buf, "\033[1;39m");

	switch (metal_prompt_commands_enabled[list_group_index].tests[list_command_index].type) {
	    case METAL_PROMPT_COMMAND_TYPE_RET_CHAR_ARG_VOID:
		case METAL_PROMPT_COMMAND_TYPE_RET_CHAR_ARG_CHAR:
        case METAL_PROMPT_COMMAND_TYPE_RET_CHAR_ARG_UINT32:
        case METAL_PROMPT_COMMAND_TYPE_RET_CHAR_ARG_UINT64:
			strcat(buf, "char*   ");
			break;

        case METAL_PROMPT_COMMAND_TYPE_RET_UINT32_ARG_VOID:
        case METAL_PROMPT_COMMAND_TYPE_RET_UINT32_ARG_CHAR:
        case METAL_PROMPT_COMMAND_TYPE_RET_UINT32_ARG_UINT32:
        case METAL_PROMPT_COMMAND_TYPE_RET_UINT32_ARG_UINT64:
            strcat(buf, "uint32_t");
        break;

        case METAL_PROMPT_COMMAND_TYPE_RET_UINT64_ARG_VOID:
        case METAL_PROMPT_COMMAND_TYPE_RET_UINT64_ARG_CHAR:
        case METAL_PROMPT_COMMAND_TYPE_RET_UINT64_ARG_UINT32:
        case METAL_PROMPT_COMMAND_TYPE_RET_UINT64_ARG_UINT64:
			strcat(buf, "uint64_t");
			break;

		default:
			strcat(buf, "void    ");
			break;
	}

    strcat(buf, "\033[1;30m");
	strcat(buf, ") arg(");
    strcat(buf, "\033[1;39m");

	switch (metal_prompt_commands_enabled[list_group_index].tests[list_command_index].type) {
	    case METAL_PROMPT_COMMAND_TYPE_RET_VOID_ARG_CHAR:
	    case METAL_PROMPT_COMMAND_TYPE_RET_CHAR_ARG_CHAR:
	    case METAL_PROMPT_COMMAND_TYPE_RET_UINT32_ARG_CHAR:
	    case METAL_PROMPT_COMMAND_TYPE_RET_UINT64_ARG_CHAR:
			strcat(buf, "char");
			break;

        case METAL_PROMPT_COMMAND_TYPE_RET_VOID_ARG_UINT32:
        case METAL_PROMPT_COMMAND_TYPE_RET_CHAR_ARG_UINT32:
        case METAL_PROMPT_COMMAND_TYPE_RET_UINT32_ARG_UINT32:
        case METAL_PROMPT_COMMAND_TYPE_RET_UINT64_ARG_UINT32:
            strcat(buf, "uint32_t");
            break;

        case METAL_PROMPT_COMMAND_TYPE_RET_VOID_ARG_UINT64:
        case METAL_PROMPT_COMMAND_TYPE_RET_CHAR_ARG_UINT64:
        case METAL_PROMPT_COMMAND_TYPE_RET_UINT32_ARG_UINT64:
        case METAL_PROMPT_COMMAND_TYPE_RET_UINT64_ARG_UINT64:
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
uint32_t metal_prompt_list_get_longest_size(void) {
    char  buf[METAL_PROMPT_COMMAND_NAME_LIMIT];
	uint32_t longest = 0;

	metal_prompt_list_begin();
	while (metal_prompt_list_current_exist()) {
	    metal_prompt_list_get_current_string(buf, false);
		uint32_t current = strlen(buf);
		if (longest < current) {
			longest = current;
		}
		metal_prompt_list_select_next();
	}
	return longest;
}


// Find length of current string and align it with longest
void metal_prompt_list_align_command_for_args(char *cmd, uint32_t longest) {
    if (strlen(cmd)>longest) return; // do not align if the cmd is too long

	for (uint32_t i = 0; i <= longest - strlen(cmd); ++i) {
		// <= on purpose, to add space even on the longest commands
	    metal_prompt_transport_out(" ");
	}
}


void metal_prompt_list_add_spaces(uint32_t current, uint32_t longest) {
    for (uint32_t i = 0; i <= longest - current; ++i) {
        // <= on purpose, to add space even on the longest commands
        metal_prompt_transport_out(" ");
    }
}
