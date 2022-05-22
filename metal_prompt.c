/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */

/*
 * Prompt handlers, handling the user-input, parsing command's arguments,
 * invoking the commands and parsing command's return values.
 */


#include "metal_prompt.h"

#include "commands_enabled.h"
#include "transport/interface.h"
#include "systick.h"
#include "config.h"
#include "globals.h"
#include "iterate.h"
#include "color.h"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#pragma mark - Private functions - prompt handling


M_P_FORCE_OPTIMIZATION
static void m_p_print_prompt(char *cmd) {
    m_p_transport_out_ln();
#ifdef M_P_COLOR_ENABLE
    m_p_color_out_prompt();
#endif
    m_p_transport_out(M_P_COMMAND_PROMPT);
#ifdef M_P_COLOR_ENABLE
    m_p_color_out_default();
#endif

    if (cmd != NULL) {
        m_p_transport_out(cmd);
    }
}


M_P_FORCE_OPTIMIZATION
static bool m_p_execute_cmd(char *cmd) {
    char buf[M_P_COMMAND_NAME_LIMIT];

    // Temporary variables to hold argument inputs and the returned values too
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
    char* ret_arg_char_ptr;
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
    uint32_t ret_arg_uint32;
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
    uint64_t ret_arg_uint64;
#endif

    m_p_iterate_begin();
    while (m_p_iterate_current_exists()) {
        m_p_iterate_get_current_string(buf, false);

        if (strcmp(cmd, buf)  == 0) {
#ifdef M_P_UPTIME
            uint32_t begin = m_p_systick_uptime_ticks;
#endif

            // TODO: parse syntax of arguments. Only if syntax is OK return 0
            m_p_command selected_command = m_p_iterate_get_current_structure();

            // TODO: Depending on type call it differently / parse return etc.
            m_p_transport_out("\r\n");

            // TODO: Parse the argument first

            // Execute the command
            switch (selected_command.type) {

                // Return type void
                case M_P_CMD_RET_VOID_ARG_VOID:
                    selected_command.void_void.action();
                    break;

#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
                case M_P_CMD_RET_VOID_ARG_CHARS:
                    selected_command.void_chars.action(ret_arg_char_ptr);
                    break;
#endif

#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
                case M_P_CMD_RET_VOID_ARG_UINT32:
                    selected_command.void_uint32.action(ret_arg_uint32);
                    break;
#endif

#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
                case M_P_CMD_RET_VOID_ARG_UINT64:
                    selected_command.void_uint64.action(ret_arg_uint64);
                    break;
#endif


#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
                // Return type char*
                case M_P_CMD_RET_CHARS_ARG_VOID:
                    ret_arg_char_ptr = selected_command.chars_void.action();
                    break;

                case M_P_CMD_RET_CHARS_ARG_CHARS:
                    ret_arg_char_ptr = selected_command.chars_chars.action(ret_arg_char_ptr);
                    break;

#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
                case M_P_CMD_RET_CHARS_ARG_UINT32:
                    ret_arg_char_ptr = selected_command.chars_uint32.action(ret_arg_uint32);
                    break;
#endif

#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
                case M_P_CMD_RET_CHARS_ARG_UINT64:
                    ret_arg_char_ptr = selected_command.chars_uint64.action(ret_arg_uint64);
                    break;
#endif
#endif


#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
                // Return type uint32_t
                case M_P_CMD_RET_UINT32_ARG_VOID:
                    ret_arg_uint32 = selected_command.uint32_void.action();
                    break;

#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
                case M_P_CMD_RET_UINT32_ARG_CHARS:
                    ret_arg_uint32 = selected_command.uint32_chars.action(ret_arg_char_ptr);
                    break;
#endif

                case M_P_CMD_RET_UINT32_ARG_UINT32:
                    ret_arg_uint32 = selected_command.uint32_uint32.action(ret_arg_uint32);
                    break;

#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
                case M_P_CMD_RET_UINT32_ARG_UINT64:
                    ret_arg_uint32 = selected_command.uint32_uint64.action(ret_arg_uint64);
                    break;
#endif
#endif // M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE


#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
                // Return type uint64_t
                case M_P_CMD_RET_UINT64_ARG_VOID:
                    ret_arg_uint64 = selected_command.uint64_void.action();
                    break;

#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
                case M_P_CMD_RET_UINT64_ARG_CHARS:
                    ret_arg_uint64 = selected_command.uint64_chars.action(ret_arg_char_ptr);
                    break;
#endif

#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
                case M_P_CMD_RET_UINT64_ARG_UINT32:
                    ret_arg_uint64 = selected_command.uint64_uint32.action(ret_arg_uint32);
                    break;
#endif

                case M_P_CMD_RET_UINT64_ARG_UINT64:
                    ret_arg_uint64 = selected_command.uint64_uint64.action(ret_arg_uint64);
                    break;
#endif

                default:
                    // Misconfiguration of the command structure detected.
                    // Abort the execution with a error!
                    return false;
                    break;
            }

            // Display the returned type
            switch (selected_command.type) {
                case M_P_CMD_RET_VOID_ARG_VOID:
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
                case M_P_CMD_RET_VOID_ARG_CHARS:
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
                case M_P_CMD_RET_VOID_ARG_UINT32:
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
                case M_P_CMD_RET_VOID_ARG_UINT64:
#endif
                    // Void returned, display nothing
                    break;

#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
                case M_P_CMD_RET_CHARS_ARG_VOID:
                case M_P_CMD_RET_CHARS_ARG_CHARS:
#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
                case M_P_CMD_RET_CHARS_ARG_UINT32:
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
                case M_P_CMD_RET_CHARS_ARG_UINT64:
#endif
                    m_p_transport_out(ret_arg_char_ptr);
                    break;
#endif

#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
                case M_P_CMD_RET_UINT32_ARG_VOID:
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
                case M_P_CMD_RET_UINT32_ARG_CHARS:
#endif
                case M_P_CMD_RET_UINT32_ARG_UINT32:

#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
                    case M_P_CMD_RET_UINT32_ARG_UINT64:
#endif
                    if (m_p_print_in_dec) {
                        itoa(ret_arg_uint32, buf, 10);
                    } else {
                        itoa(ret_arg_uint32, buf, 16);
                        m_p_transport_out("0x");
                    }
                    m_p_transport_out(buf);
                    break;
#endif

#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
                case M_P_CMD_RET_UINT64_ARG_VOID:
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
                case M_P_CMD_RET_UINT64_ARG_CHARS:
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
                case M_P_CMD_RET_UINT64_ARG_UINT32:
#endif
                case M_P_CMD_RET_UINT64_ARG_UINT64:
                    if (m_p_print_in_dec) {
                        itoa(ret_arg_uint64, buf, 10);
                    } else {
                        itoa(ret_arg_uint64, buf, 16);
                        m_p_transport_out("0x");
                    }
                    m_p_transport_out(buf);
                    break;
#endif

                default:
                    // The misconfiguration should have been detected with the
                    // previous case/switch statement, but just in case there
                    // is a bug in it, we will catch it here too.
                    // Abort the execution with a error!
                    return false;
                    break;
            }


#ifdef M_P_UPTIME
            if (m_p_benchmark_commands) {
                itoa(m_p_systick_uptime_ticks - begin, buf, 16);
                m_p_transport_out("\r\nCommand took 0x");
                m_p_transport_out(buf);
                m_p_transport_out(" ticks to execute");
            }
#endif

            return true; // command executed, return success
        }
        m_p_iterate_next();
    }

    return false; // no command found, return error
}


M_P_FORCE_OPTIMIZATION
static void m_p_evaluate_character(char character) {
    static char cmd[255];
    static char cmd_old[255]="";
    static unsigned int caret = 0;
    static unsigned int escape_sequence = 0;
    static unsigned int escape_sequence_next = 0;

    switch (character) {
        case 0x0a:
        case 0x0d:
            // \r line feed or carriage return \n  (enter)
            if (strlen(cmd)==0) {
                m_p_print_prompt(NULL);
                break;
            }


            if (!m_p_execute_cmd(cmd)) {
#ifdef M_P_COLOR_ENABLE
                m_p_color_out_error();
#endif
                m_p_transport_out("\r\n[ERROR]");
#ifdef M_P_COLOR_ENABLE
                m_p_color_out_default();
#endif
                m_p_transport_out(" Missing command or wrong arguments...\r\n");
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

#ifdef M_P_LIST_AND_AUTOCOMPLETE_ENABLE
        case 0x09:
            // TAB
            m_p_auto_complete(cmd, &caret);
            break;
#endif

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

#ifdef M_P_ALLOW_QUIT
        case 0x03:
            // Ctrl + C = quit
            m_p_keep_runnning = false;
            m_p_transport_out("\r\nCtrl + C, exiting ...\r\n");
            break;
#endif

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


#pragma mark - Public functions


M_P_FORCE_OPTIMIZATION
void m_p_prompt_generic() {
    m_p_transport_out("\r\nTest Interface ");
    m_p_transport_out(M_P_VERSION);
    m_p_transport_out("\r\n");
    m_p_print_prompt(NULL);
#ifdef M_P_ALLOW_QUIT
    m_p_keep_runnning = true;
#endif

#ifdef M_P_LIST_AND_AUTOCOMPLETE_ENABLE
    // Find out what is the length of the longest command
    m_p_iterate_set_size_of_longest_command();
#endif

#ifdef M_P_ALLOW_QUIT
    while (m_p_keep_runnning) {
#else
    while (1) {
#endif
        char character;
        if (m_p_transport_in(&character)) {
            m_p_evaluate_character(character);
        }
    }

}


#ifdef M_P_LIST_AND_AUTOCOMPLETE_ENABLE
M_P_FORCE_OPTIMIZATION
void m_p_auto_complete(char* cmd, unsigned int* caret) {
    char         first_command[M_P_COMMAND_NAME_LIMIT];
    char         buf[M_P_COMMAND_NAME_LIMIT];

    unsigned int common_location = M_P_COMMAND_NAME_LIMIT;
    unsigned int cmd_len         = strlen(cmd);
    bool         first           = true;

    // Allow auto-complete to work even with empty commands
#ifndef M_P_AUTOCOMPLETE_ON_EMPTY_COMMANDS
    if (cmd_len == 0) {
        // No auto complete with empty line
        return;
    }
#endif


    m_p_iterate_begin();

    while (m_p_iterate_current_exists()) {
        // Go through all commands, if the start of the command matches the
        // current cmd line, then find how many unique characters can be added
        m_p_iterate_get_current_string(buf, false);
        if (strncmp(cmd, buf, cmd_len) == 0) {
            if (first) {
                strcpy(first_command, buf);
                first = false;
            }

            unsigned int overlap = strspn(first_command, buf);
            if (overlap < common_location) {
                common_location = overlap;
            }
        }
        m_p_iterate_next();
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
#ifdef M_P_DO_NOT_LIST_ON_FULL_COMMANDS
        m_p_iterate_begin();
        while (m_p_iterate_current_exists()) {
            // Search if there is full match
            m_p_iterate_get_current_string(buf, false);
            if (strcmp(cmd, buf) == 0) {
                return;
            }
            m_p_iterate_next();
        }
#endif

        // If not full command, then list all the options

        m_p_transport_out_ln();
        m_p_iterate_begin();
        while (m_p_iterate_current_exists()) {
            // Go through all commands, if the start of the command matches the
            m_p_iterate_get_current_string(buf, false);
            if (strncmp(cmd, buf, cmd_len) == 0) {
                m_p_transport_out_ln();

                // Print the current command
                unsigned int cmd_len = m_p_iterate_get_current_string(buf, true);
                m_p_transport_out(buf);
#ifdef M_P_COLOR_ENABLE
                m_p_color_out_default();
#endif

                // Align it to the longest command
                m_p_iterate_align_with_longest_command(cmd_len);

                // Print the arguments
                m_p_iterate_get_current_string_arguments(buf);
                m_p_transport_out(buf);

            }
            m_p_iterate_next();
        }
        m_p_print_prompt(cmd);
    }
}
#endif
