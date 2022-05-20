/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#include "metal_prompt.h"

#include "commands_enabled.h"
#include "transport/interface.h"
#include "systick.h"
#include "config.h"
#include "globals.h"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#pragma mark Private variables
uint32_t list_group_index   = 0;
uint32_t list_command_index = 0;
uint32_t longest_command    = 0;


#pragma mark Public functions
void m_p_cmd_line_generic() {
    m_p_transport_out("\r\nTest Interface ");
    m_p_transport_out(M_P_VERSION);
    m_p_transport_out("\r\n");
    m_p_print_prompt(NULL);
    m_p_keep_runnning = true;

    // Get what is the longest command length
    longest_command = m_p_list_get_longest_size();

    while (m_p_keep_runnning) {
        char character;
        if (m_p_transport_in(&character)) {
            m_p_evaluate_character(character);
        }
    }
}


#pragma mark Private functions
void m_p_print_prompt(char *cmd) {
    m_p_transport_out_ln();
    m_p_transport_out("\033[1;36m");
    m_p_transport_out(M_P_COMMAND_PROMPT);
    m_p_transport_out("\033[0;39m");

    if (cmd != NULL) {
        m_p_transport_out(cmd);
    }
}


uint32_t m_p_execute_cmd(char *cmd) {
	char buf[M_P_COMMAND_NAME_LIMIT];

	// Temporary variables to hold argument inputs and the returned values too
	char* ret_arg_char_ptr;
    uint32_t ret_arg_uint32;
	uint64_t ret_arg_uint64;

	m_p_list_begin();
	while (m_p_list_current_exist()) {
		m_p_list_get_current_string(buf, false);

		if (strcmp(cmd, buf)  == 0) {
#ifdef METAL_PROMPT_UPTIME
			uint64_t begin = metal_prompt_systick_uptime_ticks;
#endif

			// TODO: parse syntax of arguments. Only if syntax is OK return 0
			m_p_command selected_command = m_p_list_get_current_structure();

			// TODO: Depending on type call it differently / parse return etc.
			m_p_transport_out("\r\n");

			// TODO: Parse the argument first

			// Execute the command
			switch (selected_command.type) {

			    // Return type void
                case M_P_COMMAND_TYPE_RET_VOID_ARG_VOID:
                    selected_command.void_void.action();
                    break;

#ifdef METAL_PROMPT_RETURN_AND_ARGUMENT_STRING_ENABLE
                case METAL_PROMPT_COMMAND_TYPE_RET_VOID_ARG_CHARS:
                    selected_command.void_chars.action(ret_arg_char_ptr);
                    break;
#endif

                case M_P_COMMAND_TYPE_RET_VOID_ARG_UINT32:
                    selected_command.void_uint32.action(ret_arg_uint32);
                    break;

                case M_P_COMMAND_TYPE_RET_VOID_ARG_UINT64:
                    selected_command.void_uint64.action(ret_arg_uint64);
                    break;


#ifdef METAL_PROMPT_RETURN_AND_ARGUMENT_STRING_ENABLE
                // Return type char*
				case METAL_PROMPT_COMMAND_TYPE_RET_CHARS_ARG_VOID:
					ret_arg_char_ptr = selected_command.chars_void.action();
					break;

                case METAL_PROMPT_COMMAND_TYPE_RET_CHARS_ARG_CHARS:
                    ret_arg_char_ptr = selected_command.chars_chars.action(ret_arg_char_ptr);
                    break;

                case METAL_PROMPT_COMMAND_TYPE_RET_CHARS_ARG_UINT32:
                    ret_arg_char_ptr = selected_command.chars_uint32.action(ret_arg_uint32);
                    break;

                case METAL_PROMPT_COMMAND_TYPE_RET_CHARS_ARG_UINT64:
                    ret_arg_char_ptr = selected_command.chars_uint64.action(ret_arg_uint64);
                    break;
#endif


                // Return type uint32_t
                case M_P_COMMAND_TYPE_RET_UINT32_ARG_VOID:
                    ret_arg_uint32 = selected_command.uint32_void.action();
                    break;

#ifdef METAL_PROMPT_RETURN_AND_ARGUMENT_STRING_ENABLE
                case METAL_PROMPT_COMMAND_TYPE_RET_UINT32_ARG_CHARS:
                    ret_arg_uint32 = selected_command.uint32_chars.action(ret_arg_char_ptr);
                    break;
#endif

                case M_P_COMMAND_TYPE_RET_UINT32_ARG_UINT32:
                    ret_arg_uint32 = selected_command.uint32_uint32.action(ret_arg_uint32);
                    break;

                case M_P_COMMAND_TYPE_RET_UINT32_ARG_UINT64:
                    ret_arg_uint32 = selected_command.uint32_uint64.action(ret_arg_uint64);
                    break;


                // Return type uint64_t
                case M_P_COMMAND_TYPE_RET_UINT64_ARG_VOID:
                    ret_arg_uint64 = selected_command.uint64_void.action();
                    break;

#ifdef METAL_PROMPT_RETURN_AND_ARGUMENT_STRING_ENABLE
                case METAL_PROMPT_COMMAND_TYPE_RET_UINT64_ARG_CHARS:
                    ret_arg_uint64 = selected_command.uint64_chars.action(ret_arg_char_ptr);
                    break;
#endif

                case M_P_COMMAND_TYPE_RET_UINT64_ARG_UINT32:
                    ret_arg_uint64 = selected_command.uint64_uint32.action(ret_arg_uint32);
                    break;

                case M_P_COMMAND_TYPE_RET_UINT64_ARG_UINT64:
                    ret_arg_uint64 = selected_command.uint64_uint64.action(ret_arg_uint64);
                    break;

				default:
				    // Misconfiguration of the command structure detected.
				    // Abort the execution with a error!
					return 1;
					break;
			}

            // Display the returned type
            switch (selected_command.type) {
                case M_P_COMMAND_TYPE_RET_VOID_ARG_VOID:
#ifdef METAL_PROMPT_RETURN_AND_ARGUMENT_STRING_ENABLE
                case METAL_PROMPT_COMMAND_TYPE_RET_VOID_ARG_CHARS:
#endif
                case M_P_COMMAND_TYPE_RET_VOID_ARG_UINT32:
                case M_P_COMMAND_TYPE_RET_VOID_ARG_UINT64:
                    // Void returned, display nothing
                    break;

#ifdef METAL_PROMPT_RETURN_AND_ARGUMENT_STRING_ENABLE
                case METAL_PROMPT_COMMAND_TYPE_RET_CHARS_ARG_VOID:
                case METAL_PROMPT_COMMAND_TYPE_RET_CHARS_ARG_CHARS:
                case METAL_PROMPT_COMMAND_TYPE_RET_CHARS_ARG_UINT32:
                case METAL_PROMPT_COMMAND_TYPE_RET_CHARS_ARG_UINT64:
                    m_p_transport_out(ret_arg_char_ptr);
                    break;
#endif

                case M_P_COMMAND_TYPE_RET_UINT32_ARG_VOID:
#ifdef METAL_PROMPT_RETURN_AND_ARGUMENT_STRING_ENABLE
                case METAL_PROMPT_COMMAND_TYPE_RET_UINT32_ARG_CHARS:
#endif
                case M_P_COMMAND_TYPE_RET_UINT32_ARG_UINT32:
                case M_P_COMMAND_TYPE_RET_UINT32_ARG_UINT64:
                    itoa(ret_arg_uint32, buf, 16);
                    m_p_transport_out("0x");
                    m_p_transport_out(buf);
                    break;

                case M_P_COMMAND_TYPE_RET_UINT64_ARG_VOID:
#ifdef METAL_PROMPT_RETURN_AND_ARGUMENT_STRING_ENABLE
                case METAL_PROMPT_COMMAND_TYPE_RET_UINT64_ARG_CHARS:
#endif
                case M_P_COMMAND_TYPE_RET_UINT64_ARG_UINT32:
                case M_P_COMMAND_TYPE_RET_UINT64_ARG_UINT64:
                    itoa(ret_arg_uint64, buf, 16);
                    m_p_transport_out("0x");
                    m_p_transport_out(buf);
                    break;

                default:
                    // The misconfiguration should have been detected with the
                    // previous case/switch statement, but just in case there
                    // is a bug in it, we will catch it here too.
                    // Abort the execution with a error!
                    return 1;
                    break;
            }


#ifdef METAL_PROMPT_UPTIME
			if (m_p_benchmark_commands) {
				itoa(metal_prompt_systick_uptime_ticks - begin, buf, 16);
				m_p_transport_out("\r\nCommand took 0x");
				m_p_transport_out(buf);
				m_p_transport_out(" ticks to execute");
			}
#endif

			return 0; // command executed, return success
		}
		m_p_list_select_next();
	}

	return 1; // no command found, return error
}


void m_p_auto_complete(char* cmd, uint32_t* caret) {
	char     first_command[M_P_COMMAND_NAME_LIMIT];
	char     buf[M_P_COMMAND_NAME_LIMIT];
	uint32_t common_location = M_P_COMMAND_NAME_LIMIT;
	uint32_t cmd_len         = strlen(cmd);
	uint32_t first           = 1;

	// Allow auto-complete to work even with empty commands
#if 0
	if (cmd_len == 0) {
		// No auto complete with empty line
		return;
	}
#endif


	m_p_list_begin();

	while (m_p_list_current_exist()) {
		// Go through all commands, if the start of the command matches the
		// current cmd line, then find how many unique characters can be added
		m_p_list_get_current_string(buf, false);
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
		m_p_list_select_next();
	}

	if (common_location == M_P_COMMAND_NAME_LIMIT) {
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
		m_p_transport_out_characters(cmd + cmd_len, common_location - cmd_len);
		*caret = strlen(cmd);
	} else {
		// There is nothing to add with auto-complete

		// If it's full unique command = do nothing
#ifdef TEST_DO_NOT_LIST_ON_FULL_COMMANDS
		m_p_list_begin();
		while (m_p_list_current_exist()) {
			// Search if there is full match
			m_p_list_get_current_string(buf, false);
			if (strcmp(cmd, buf) == 0) {
				return;
			}
			m_p_list_select_next();
		}
#endif

		// If not full command, then list all the options

		m_p_transport_out_ln();
		m_p_list_begin();
		while (m_p_list_current_exist()) {
			// Go through all commands, if the start of the command matches the
			m_p_list_get_current_string(buf, false);
			if (strncmp(cmd, buf, cmd_len) == 0) {
			    m_p_transport_out_ln();

			    m_p_transport_out("\033[1;33m");
				// Print the current command
				uint32_t cmd_len = m_p_list_get_current_string(buf, true);
				m_p_transport_out(buf);
				m_p_transport_out("\033[0;39m");

				// Align it to the longest command
				m_p_list_align_with_longest_command(cmd_len);

				// Print the arguments
				m_p_list_get_current_string_arguments(buf);
				m_p_transport_out(buf);

			}
			m_p_list_select_next();
		}
		m_p_print_prompt(cmd);
	}
}


void m_p_evaluate_character(char character) {
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
				m_p_print_prompt(NULL);
				break;
			}


			if (m_p_execute_cmd(cmd)) {
			    m_p_transport_out("\033[1;31m");
			    m_p_transport_out("\r\n[ERROR] ");
                m_p_transport_out("\033[0;39m");
			    m_p_transport_out("Missing command or wrong arguments...\r\n");
			} else {
				// Executed correctly the command, clean the command line
			}

			// Store current command into history
			strcpy(cmd_old,cmd);

			// Clear current command
			cmd[0] = 0;
			caret = 0;

			m_p_print_prompt(NULL);
			break;

		case 0x09:
			// TAB
			m_p_auto_complete(cmd, &caret);
			break;

		case 0x08:
		case 0x7f:
			// Backspace
			if (caret > 0) {
				// only remove character when there is something to delete
				cmd[--caret] = 0;
				// Go back 1 character, print space and go back 1 character again
				m_p_transport_out("\033[1D \033[1D");
			}
			break;

		case 0x03:
			// Ctrl + C = quit
			m_p_keep_runnning = false;
			m_p_transport_out("\r\nCtrl + C, exiting ...\r\n");
			break;

		case 0x13:
			// Ctrl + S = clear screen
		    m_p_transport_out("\033[2J\033[H");
			m_p_print_prompt(cmd);
			break;

		case 0x0C:
			// Ctrl + L = recall history
			if (strlen(cmd) == 0) {
				// When prompt empty, then print at the same prompt
			    m_p_transport_out(cmd_old);
			} else{
				//  Print completely new prompt
				m_p_print_prompt(cmd_old);
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
				m_p_transport_out_characters(&character, 1);
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
				    m_p_transport_out(cmd_old);
				} else{
					//  Print completely new prompt
					m_p_print_prompt(cmd_old);
				}

				strcpy(cmd, cmd_old);
				caret = strlen(cmd);
				escape_sequence_next = 0;
			}

			break;
	}
	escape_sequence = escape_sequence_next;
}


void m_p_list_begin() {
	list_group_index   = 0;
	list_command_index = 0;
}


bool m_p_list_is_first() {
	return (list_group_index == 0) && (list_command_index == 0);
}


bool m_p_list_current_exist() {
	if ( list_group_index < M_P_ENABLED_SIZE &&
			list_command_index < m_p_commands_enabled[list_group_index].testsSize) {
		return true;
	}
	return false;
}


void m_p_list_select_next() {
	list_command_index++;
	if (list_command_index >= m_p_commands_enabled[list_group_index].testsSize) {
		// Finished with current group, select new group
		list_command_index = 0;
		list_group_index++;  // no need to check here if it's valid group
	}
}


m_p_command m_p_list_get_current_structure() {
	return m_p_commands_enabled[list_group_index].tests[list_command_index];
}


uint32_t m_p_list_get_current_string(char *buf, bool color) {
	uint32_t group_len = strlen(m_p_commands_enabled[list_group_index].group_name);

	strcpy(buf,"");
	uint32_t ans = 0;

	if ( group_len != 0 ) {
        if (color) strcat(buf, "\033[1;33m");
		strcat(buf, m_p_commands_enabled[list_group_index].group_name);
        if (color) strcat(buf, "\033[1;39m");
		strcat(buf, ".");
        ans += strlen(m_p_commands_enabled[list_group_index].group_name) + 1;
	}
    if (color) strcat(buf, "\033[1;35m");
	strcat(buf, m_p_commands_enabled[list_group_index].tests[list_command_index].command);
	ans += strlen(m_p_commands_enabled[list_group_index].tests[list_command_index].command);
    if (color) strcat(buf, "\033[0;39m");

	return ans;
}


void m_p_list_get_current_string_arguments(char *buf) {
    strcpy(buf, "\033[1;30m");
	strcat(buf, "return(");
    strcat(buf, "\033[1;39m");

	switch (m_p_commands_enabled[list_group_index].tests[list_command_index].type) {
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
	    case M_P_COMMAND_TYPE_RET_CHARS_ARG_VOID:
		case M_P_COMMAND_TYPE_RET_CHARS_ARG_CHARS:
        case M_P_COMMAND_TYPE_RET_CHARS_ARG_UINT32:
        case M_P_COMMAND_TYPE_RET_CHARS_ARG_UINT64:
			strcat(buf, "char*   ");
			break;
#endif

        case M_P_COMMAND_TYPE_RET_UINT32_ARG_VOID:
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
        case M_P_COMMAND_TYPE_RET_UINT32_ARG_CHARS:
#endif
        case M_P_COMMAND_TYPE_RET_UINT32_ARG_UINT32:
        case M_P_COMMAND_TYPE_RET_UINT32_ARG_UINT64:
            strcat(buf, "uint32_t");
        break;

        case M_P_COMMAND_TYPE_RET_UINT64_ARG_VOID:
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
        case M_P_COMMAND_TYPE_RET_UINT64_ARG_CHARS:
#endif
        case M_P_COMMAND_TYPE_RET_UINT64_ARG_UINT32:
        case M_P_COMMAND_TYPE_RET_UINT64_ARG_UINT64:
			strcat(buf, "uint64_t");
			break;

		default:
			strcat(buf, "void    ");
			break;
	}

    strcat(buf, "\033[1;30m");
	strcat(buf, ") arg(");
    strcat(buf, "\033[1;39m");

	switch (m_p_commands_enabled[list_group_index].tests[list_command_index].type) {
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE

	    case M_P_COMMAND_TYPE_RET_VOID_ARG_CHARS:
	    case M_P_COMMAND_TYPE_RET_CHARS_ARG_CHARS:
	    case M_P_COMMAND_TYPE_RET_UINT32_ARG_CHARS:
	    case M_P_COMMAND_TYPE_RET_UINT64_ARG_CHARS:
			strcat(buf, "char*");
			break;
#endif

        case M_P_COMMAND_TYPE_RET_VOID_ARG_UINT32:
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
        case M_P_COMMAND_TYPE_RET_CHARS_ARG_UINT32:
#endif
        case M_P_COMMAND_TYPE_RET_UINT32_ARG_UINT32:
        case M_P_COMMAND_TYPE_RET_UINT64_ARG_UINT32:
            strcat(buf, "uint32_t");
            break;

        case M_P_COMMAND_TYPE_RET_VOID_ARG_UINT64:
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
        case M_P_COMMAND_TYPE_RET_CHARS_ARG_UINT64:
#endif
        case M_P_COMMAND_TYPE_RET_UINT32_ARG_UINT64:
        case M_P_COMMAND_TYPE_RET_UINT64_ARG_UINT64:
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
uint32_t m_p_list_get_longest_size(void) {
    char  buf[M_P_COMMAND_NAME_LIMIT];
	uint32_t longest = 0;

	m_p_list_begin();
	while (m_p_list_current_exist()) {
	    m_p_list_get_current_string(buf, false);
		uint32_t current = strlen(buf);
		if (longest < current) {
			longest = current;
		}
		m_p_list_select_next();
	}
	return longest;
}


// Find align the current string with the longest command
void m_p_list_align_with_longest_command(uint32_t current_size) {
    for (uint32_t i = 0; i <= longest_command - current_size; ++i) {
        // <= on purpose, to add space even on the longest commands
        m_p_transport_out(" ");
    }
}
