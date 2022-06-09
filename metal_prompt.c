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


M_P_CFG_FORCE_OPTIMIZATION
static void m_p_print_prompt(const char *cmd) {
    m_p_transport_out_ln();
#ifdef M_P_CFG_COLORS
    m_p_color_out_prompt();
#endif
    m_p_transport_out(M_P_CFG_COMMAND_PROMPT);
#ifdef M_P_CFG_COLORS
    m_p_color_out_default();
#endif

    if (NULL != cmd) {
        m_p_transport_out(cmd);
    }
}


M_P_CFG_FORCE_OPTIMIZATION
static bool m_p_execute_command(const char *cmd, char *buf, const m_p_command* selected_command) {
    // Temporary variables to hold argument inputs and the returned values too
#ifdef M_P_CFG_TYPE_CHARS
    char* ret_arg_char_ptr;
#endif
#ifdef M_P_CFG_TYPE_UINT
    unsigned int ret_arg_uint; // Intentionally leaving it's size to change depending on the target
#endif
#ifdef M_P_CFG_TYPE_UINT32
    uint32_t ret_arg_uint32;
#endif
#ifdef M_P_CFG_TYPE_UINT64
    uint64_t ret_arg_uint64;
#endif

    // TODO: Parse the argument first (if not using buf, then remove it from signature)

    // Execute the command
    switch (selected_command->type) {

        // -------------- Return type void -------------------------
        case M_P_CMD_TYPES(M_P_TYPE_VOID, M_P_TYPE_VOID):
            selected_command->void_void.action();
            break;

#if defined(M_P_CFG_TYPE_CHARS)
        case M_P_CMD_TYPES(M_P_TYPE_VOID, M_P_TYPE_CHARS):
            selected_command->void_chars.action(ret_arg_char_ptr);
            break;
#endif

#if defined(M_P_CFG_TYPE_UINT)
        case M_P_CMD_TYPES(M_P_TYPE_VOID, M_P_TYPE_UINT):
            selected_command->void_uint.action(ret_arg_uint);
            break;
#endif

#if defined(M_P_CFG_TYPE_UINT32)
        case M_P_CMD_TYPES(M_P_TYPE_VOID, M_P_TYPE_UINT32):
            selected_command->void_uint32.action(ret_arg_uint32);
            break;
#endif

#if defined(M_P_CFG_TYPE_UINT64)
        case M_P_CMD_TYPES(M_P_TYPE_VOID, M_P_TYPE_UINT64):
            selected_command->void_uint64.action(ret_arg_uint64);
            break;
#endif


        // -------------- Return type chars ---------------------
#if defined(M_P_CFG_TYPE_CHARS)
        case M_P_CMD_TYPES(M_P_TYPE_CHARS, M_P_TYPE_VOID):
            ret_arg_char_ptr = selected_command->chars_void.action();
            break;
#endif

#if defined(M_P_CFG_TYPE_CHARS) && defined(M_P_CFG_TYPE_CHARS)
        case M_P_CMD_TYPES(M_P_TYPE_CHARS, M_P_TYPE_CHARS):
            ret_arg_char_ptr = selected_command->chars_chars.action(ret_arg_char_ptr);
            break;
#endif

#if defined(M_P_CFG_TYPE_CHARS) && defined(M_P_CFG_TYPE_UINT)
        case M_P_CMD_TYPES(M_P_TYPE_CHARS, M_P_TYPE_UINT):
            ret_arg_char_ptr = selected_command->chars_uint.action(ret_arg_uint);
            break;
#endif

#if defined(M_P_CFG_TYPE_CHARS) && defined(M_P_CFG_TYPE_UINT32)
        case M_P_CMD_TYPES(M_P_TYPE_CHARS, M_P_TYPE_UINT32):
            ret_arg_char_ptr = selected_command->chars_uint32.action(ret_arg_uint32);
            break;
#endif

#if defined(M_P_CFG_TYPE_CHARS) && defined(M_P_CFG_TYPE_UINT64)
        case M_P_CMD_TYPES(M_P_TYPE_CHARS, M_P_TYPE_UINT64):
            ret_arg_char_ptr = selected_command->chars_uint64.action(ret_arg_uint64);
            break;
#endif


        // -------------- Return type unsigned int ---------------------
#if defined(M_P_CFG_TYPE_UINT)
        case M_P_CMD_TYPES(M_P_TYPE_UINT, M_P_TYPE_VOID):
            ret_arg_uint = selected_command->uint_void.action();
            break;
#endif

#if defined(M_P_CFG_TYPE_UINT) && defined(M_P_CFG_TYPE_CHARS)
        case M_P_CMD_TYPES(M_P_TYPE_UINT, M_P_TYPE_CHARS):
            ret_arg_uint = selected_command->uint_chars.action(ret_arg_char_ptr);
            break;
#endif

#if defined(M_P_CFG_TYPE_UINT) && defined(M_P_CFG_TYPE_UINT)
        case M_P_CMD_TYPES(M_P_TYPE_UINT, M_P_TYPE_UINT):
            ret_arg_uint = selected_command->uint_uint.action(ret_arg_uint);
            break;
#endif

#if defined(M_P_CFG_TYPE_UINT) && defined(M_P_CFG_TYPE_UINT32)
        case M_P_CMD_TYPES(M_P_TYPE_UINT, M_P_TYPE_UINT32):
            ret_arg_uint = selected_command->uint_uint32.action(ret_arg_uint32);
            break;
#endif

#if defined(M_P_CFG_TYPE_UINT) && defined(M_P_CFG_TYPE_UINT64)
        case M_P_CMD_TYPES(M_P_TYPE_UINT, M_P_TYPE_UINT64):
            ret_arg_uint = selected_command->uint_uint64.action(ret_arg_uint64);
            break;
#endif


    // -------------- Return type uint32 ---------------------
#if defined(M_P_CFG_TYPE_UINT32)
        case M_P_CMD_TYPES(M_P_TYPE_UINT32, M_P_TYPE_VOID):
            ret_arg_uint32 = selected_command->uint32_void.action();
            break;
#endif

#if defined(M_P_CFG_TYPE_UINT32) && defined(M_P_CFG_TYPE_CHARS)
        case M_P_CMD_TYPES(M_P_TYPE_UINT32, M_P_TYPE_CHARS):
            ret_arg_uint32 = selected_command->uint32_chars.action(ret_arg_char_ptr);
            break;
#endif

#if defined(M_P_CFG_TYPE_UINT32) && defined(M_P_CFG_TYPE_UINT)
        case M_P_CMD_TYPES(M_P_TYPE_UINT32, M_P_TYPE_UINT):
            ret_arg_uint32 = selected_command->uint32_uint.action(ret_arg_uint);
            break;
#endif

#if defined(M_P_CFG_TYPE_UINT32) && defined(M_P_CFG_TYPE_UINT32)
        case M_P_CMD_TYPES(M_P_TYPE_UINT32, M_P_TYPE_UINT32):
            ret_arg_uint32 = selected_command->uint32_uint32.action(ret_arg_uint32);
            break;
#endif

#if defined(M_P_CFG_TYPE_UINT32) && defined(M_P_CFG_TYPE_UINT64)
        case M_P_CMD_TYPES(M_P_TYPE_UINT32, M_P_TYPE_UINT64):
            ret_arg_uint32 = selected_command->uint32_uint64.action(ret_arg_uint64);
            break;
#endif


    // -------------- Return type uint64 ---------------------
#if defined(M_P_CFG_TYPE_UINT64)
        case M_P_CMD_TYPES(M_P_TYPE_UINT64, M_P_TYPE_VOID):
            ret_arg_uint64 = selected_command->uint64_void.action();
            break;
#endif

#if defined(M_P_CFG_TYPE_UINT64) && defined(M_P_CFG_TYPE_CHARS)
        case M_P_CMD_TYPES(M_P_TYPE_UINT64, M_P_TYPE_CHARS):
            ret_arg_uint64 = selected_command->uint64_chars.action(ret_arg_char_ptr);
            break;
#endif

#if defined(M_P_CFG_TYPE_UINT64) && defined(M_P_CFG_TYPE_UINT)
        case M_P_CMD_TYPES(M_P_TYPE_UINT64, M_P_TYPE_UINT):
            ret_arg_uint64 = selected_command->uint64_uint.action(ret_arg_uint);
            break;
#endif

#if defined(M_P_CFG_TYPE_UINT64) && defined(M_P_CFG_TYPE_UINT32)
        case M_P_CMD_TYPES(M_P_TYPE_UINT64, M_P_TYPE_UINT32):
            ret_arg_uint64 = selected_command->uint64_uint32.action(ret_arg_uint32);
            break;
#endif

#if defined(M_P_CFG_TYPE_UINT64) && defined(M_P_CFG_TYPE_UINT64)
        case M_P_CMD_TYPES(M_P_TYPE_UINT64, M_P_TYPE_UINT64):
            ret_arg_uint64 = selected_command->uint64_uint64.action(ret_arg_uint64);
            break;
#endif

        default:
            // Non valid command type found, aborting
            return false;
            break;
    }


    // Display the returned type
    switch (M_P_CMD_MASK_RET & selected_command->type) {

#ifdef M_P_CFG_TYPE_CHARS
        case M_P_CMD_GET_RET_TYPE(M_P_TYPE_CHARS):
            m_p_transport_out(ret_arg_char_ptr);
            break;
#endif

#ifdef M_P_CFG_TYPE_UINT
        case M_P_CMD_GET_RET_TYPE(M_P_TYPE_UINT):
            if (m_p_print_in_dec) {
                itoa(ret_arg_uint, buf, 10);
            } else {
                itoa(ret_arg_uint, buf, 16);
                m_p_transport_out("0x");
            }
            m_p_transport_out(buf);
            break;
#endif

#ifdef M_P_CFG_TYPE_UINT32
        case M_P_CMD_GET_RET_TYPE(M_P_TYPE_UINT32):
            if (m_p_print_in_dec) {
                itoa(ret_arg_uint32, buf, 10);
            } else {
                itoa(ret_arg_uint32, buf, 16);
                m_p_transport_out("0x");
            }
            m_p_transport_out(buf);
            break;
#endif

#ifdef M_P_CFG_TYPE_UINT64
        case M_P_CMD_GET_RET_TYPE(M_P_TYPE_UINT64):
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
            // void return type, nothing to display
            break;
    }

    return true;
}


M_P_CFG_FORCE_OPTIMIZATION
static bool m_p_find_match_and_execute(const char *cmd) {
    char buf[M_P_CFG_COMMAND_NAME_SIZE];

    m_p_iterate_begin();
    while (m_p_iterate_current_exists()) {
        m_p_iterate_get_current_string(buf, false);

        // TODO: check if the len of both is the same, overlapping commands
        // chould make wrong match
        if (0 == strcmp(cmd, buf)) {
#ifdef M_P_CFG_UPTIME
            const unsigned int begin = m_p_systick_uptime_ticks;
#endif

            // TODO: parse syntax of arguments. Only if syntax is OK return 0
            m_p_command selected_command = m_p_iterate_get_current_structure();

            // TODO: Depending on type call it differently / parse return etc.
            m_p_transport_out("\r\n");

            if (!m_p_execute_command(cmd, buf, &selected_command)) {
                // problem happened
                return false;
            }

#ifdef M_P_CFG_UPTIME
            if (m_p_benchmark_commands) {
                const unsigned int end = m_p_systick_uptime_ticks;
                unsigned int diff = end - begin;

#ifdef M_P_CFG_UPTIME_SMALL_OVERFLOW
                if (2 >= sizeof(unsigned int)) {
                    // small int on this platform, check for overflow
                    if (end < begin) {
                        // 0 ..... end ..... begin ..... MAX
                        diff = 0;
                        diff--; // turn all bits 1
                        diff -= begin;  // calculate the begin->MAX
                        diff += end;    // add the range 0->end
                        // so the diff is 0->end + begin->MAX
                    }
                }
#endif

                itoa(diff, buf, 16);
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


M_P_CFG_FORCE_OPTIMIZATION
static void m_p_evaluate_character(const char character) {
    static char cmd[M_P_CFG_WHOLE_PROMPT_SIZE];
    static unsigned int caret = 0;
    static unsigned int escape_sequence = 0;
    static unsigned int escape_sequence_next = 0;

#ifdef M_P_CFG_HISTORY
    static char cmd_old[M_P_CFG_WHOLE_PROMPT_SIZE]="";
#endif

    switch (character) {
        case 0x0a:
        case 0x0d:
            // \r line feed or carriage return \n  (enter)
            if (0 == strlen(cmd)) {
                m_p_print_prompt(NULL);
                break;
            }


            if (!m_p_find_match_and_execute(cmd)) {
#ifdef M_P_CFG_COLORS
                m_p_color_out_error();
#endif
                m_p_transport_out("\r\n[ERROR]");
#ifdef M_P_CFG_COLORS
                m_p_color_out_default();
#endif
                m_p_transport_out(" Missing/misconfigured command or wrong arguments...\r\n");

#ifdef M_P_CFG_BELL
                    m_p_transport_out("\07");
#endif

            } else {
                // Executed correctly the command, clean the command line
            }

#ifdef M_P_CFG_HISTORY
            // Store current command into history
            strcpy(cmd_old,cmd);
#endif

            // Clear current command
            cmd[0] = 0;
            caret = 0;

            m_p_print_prompt(NULL);
            break;

#ifdef M_P_CFG_AUTOCOMPLETE
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

#ifdef M_P_CFG_ALLOW_QUIT
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

        case 0x1B:
            // start of the escape sequence \033, but do not display anything yet
            if (0 == escape_sequence){
                escape_sequence_next = 1;
            }
            break;

        default:
            if (0 == escape_sequence) {
                // Regular character, write it down on the screen and to the buffer
                if (caret < (M_P_CFG_COMMAND_NAME_SIZE-2)) {
                    // Before adding any character, make sure you have at least
                    // 2 characters spare in the buffer. For the character you
                    // are going to assign right now and the 0 termination
                    // character.
                    cmd[caret++] = character;
                    cmd[caret]   = 0;
                    m_p_transport_out_characters(&character, 1);
                } else {
                    // You would get buffer overflow, optionally give feedback
                    // that a issue happened
#ifdef M_P_CFG_BELL
                    m_p_transport_out("\07");
#endif
                }
            }

            if ( (1 == escape_sequence) && ('[' == character) ) {
                // Parsing second part of the escape sequence
                escape_sequence_next = 2;
            }
            else {
                escape_sequence_next = 0;
            }

#ifdef M_P_CFG_HISTORY
            if ( (2 == escape_sequence) && ('A' == character) ) {
                // Parsing last 3rd part of the sequence
                // Up arrow was pressed, copy previous command
                if (0 == strlen(cmd)) {
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
#else
            if (2 == escape_sequence) {
                escape_sequence_next = 0;
            }
#endif


            break;
    }
    escape_sequence = escape_sequence_next;
}


#pragma mark - Public functions


M_P_CFG_FORCE_OPTIMIZATION
void m_p_prompt_generic() {
    m_p_transport_out("\r\nTest Interface ");
    m_p_transport_out(M_P_VERSION);
    m_p_transport_out("\r\n");
    m_p_print_prompt(NULL);
#ifdef M_P_CFG_ALLOW_QUIT
    m_p_keep_runnning = true;
#endif

#ifdef M_P_CFG_AUTOCOMPLETE
    // Find out what is the length of the longest command
    m_p_iterate_set_size_of_longest_command();
#endif

#ifdef M_P_CFG_ALLOW_QUIT
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


#ifdef M_P_CFG_AUTOCOMPLETE
M_P_CFG_FORCE_OPTIMIZATION
void m_p_auto_complete(char* cmd, unsigned int* caret) {
    char         first_command[M_P_CFG_COMMAND_NAME_SIZE];
    char         buf[M_P_CFG_COMMAND_NAME_SIZE];

    unsigned int common_location = M_P_CFG_COMMAND_NAME_SIZE;
    unsigned int cmd_len         = strlen(cmd);
    bool         first           = true;

    // Allow auto-complete to work even with empty commands
#ifndef M_P_CFG_AUTOCOMPLETE_ON_EMPTY_PROMPT
    if (0 == cmd_len) {
        // No auto complete with empty line
        return;
    }
#endif


    m_p_iterate_begin();

    while (m_p_iterate_current_exists()) {
        // Go through all commands, if the start of the command matches the
        // current cmd line, then find how many unique characters can be added
        m_p_iterate_get_current_string(buf, false);
        if (0 == strncmp(cmd, buf, cmd_len)) {
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

    if (M_P_CFG_COMMAND_NAME_SIZE == common_location) {
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
        // There is nothing to add with auto-complete as it's full command

#ifdef M_P_CFG_AUTOCOMPLETE_ON_FULL_COMMAND
        // Do the autocomplete anyway as it will display the arguments of the
        // current command
        m_p_iterate_begin();
        while (m_p_iterate_current_exists()) {
            // Search if there is full match
            m_p_iterate_get_current_string(buf, false);
            if (0 == strcmp(cmd, buf)) {
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
            if (0 == strncmp(cmd, buf, cmd_len)) {
                m_p_transport_out_ln();

                // Print the current command
                unsigned int cmd_len = m_p_iterate_get_current_string(buf, true);
                m_p_transport_out(buf);
#ifdef M_P_CFG_COLORS
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
