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
static void m_p_print_prompt(const char *command) {
    m_p_transport_out_ln();
#ifdef M_P_CFG_COLORS
    m_p_color_out_prompt();
#endif
    m_p_transport_out(M_P_CFG_COMMAND_PROMPT);
#ifdef M_P_CFG_COLORS
    m_p_color_out_default();
#endif

    if (NULL != command) {
        m_p_transport_out(command);
    }
}


#ifndef M_P_CFG_TYPES_NONE
M_P_CFG_FORCE_OPTIMIZATION
static int m_p_find_first_space(const char *prompt) {
    for (unsigned int i=0; i<strlen(prompt); i++) {
        if (' ' == prompt[i]) return i;
    }
    return -1;
}
#endif


#ifdef M_P_CFG_TYPE_UINT
M_P_CFG_FORCE_OPTIMIZATION
static char * m_p_parse_hex_nibble_to_uint(unsigned int *val, char *hex_string) {
    char ch = *hex_string++;
    *val = (*val << 4) + ((ch <= '9') ? (ch-'0') : (ch <= 'F') ? (ch -'A' + 10) : (ch -'a' + 10));
    return hex_string;
}
#endif


#ifdef M_P_CFG_TYPE_UINT32
M_P_CFG_FORCE_OPTIMIZATION
static char * m_p_parse_hex_nibble_to_uint32(uint32_t *val, char *hex_string) {
    char ch = *hex_string++;
    *val = (*val << 4) + ((ch <= '9') ? (ch-'0') : (ch <= 'F') ? (ch -'A' + 10) : (ch -'a' + 10));
    return hex_string;
}
#endif


#ifdef M_P_CFG_TYPE_UINT64
M_P_CFG_FORCE_OPTIMIZATION
static char * m_p_parse_hex_nibble_to_uint64(uint64_t *val, char *hex_string) {
    char ch = *hex_string++;
    *val = (*val << 4) + ((ch <= '9') ? (ch-'0') : (ch <= 'F') ? (ch -'A' + 10) : (ch -'a' + 10));
    return hex_string;
}
#endif


M_P_CFG_FORCE_OPTIMIZATION
static bool m_p_execute_command(
        const unsigned int index_of_first_space,
        char *buf,
        const m_p_command* selected_command) {

    char *cmd = m_p_command_prompt;

    // Temporary variables to hold argument inputs and the returned values too
#ifdef M_P_CFG_TYPE_CHARS
    char* ret_arg_char_ptr = m_p_return_and_argument_buf;
#endif
#ifdef M_P_CFG_TYPE_UINT
    unsigned int ret_arg_uint = 0; // Intentionally leaving it's size to change depending on the target
#endif
#ifdef M_P_CFG_TYPE_UINT32
    uint32_t ret_arg_uint32 = 0;
#endif
#ifdef M_P_CFG_TYPE_UINT64
    uint64_t ret_arg_uint64 = 0;
#endif

#ifndef M_P_CFG_TYPES_NONE
    if (-1 != index_of_first_space && M_P_TYPE_VOID != (M_P_CMD_MASK_ARG & selected_command->type)) {
        // There was some space in the command prompt which indicates some
        // argument[s]. And the selected command and has some non-void argument
        // type

        // Move the buffer to start the argument segment after the ' ' space
        cmd += index_of_first_space + 1;

        // Find another space indicating there are multiple arguments,
        // clip the string to only contain the first argument and nothing else
        const int space_after_argument = m_p_find_first_space(cmd);
        if (-1 != space_after_argument) {
            cmd[space_after_argument] = 0;
        }

#ifdef M_P_CFG_TYPE_CHARS
        // If it's non-string argument then check the size of it
        if ( M_P_CMD_GET_ARG_TYPE(M_P_TYPE_CHARS) != (M_P_CMD_MASK_ARG & selected_command->type) ) {
            if ( 3 > strlen(cmd)) {
                // it is not long enough for even the 0x0 string, abort execution
                return false;
            }
        } else {
            // It's non-string argument then expect hex number as argument
            cmd += 2; // Assume the argument has '0x' prefix and jump over it
        }
#else
        if ( 3 > strlen(cmd) ) {
            // it is not long enough for even the 0x0 argument, abort execution
            return false;
        }
        cmd += 2; // Assume the argument has '0x' prefix and jump over it
#endif  // M_P_CFG_TYPE_CHARS


        switch (M_P_CMD_MASK_ARG & selected_command->type) {
#ifdef M_P_CFG_TYPE_CHARS
            case M_P_CMD_GET_ARG_TYPE(M_P_TYPE_CHARS):
                if (-1 != space_after_argument) {
                    // Strings can contain spaces, so put the removed space back
                    cmd[space_after_argument] = ' ';
                }

                if (M_P_CFG_TYPE_CHARS_BUFFER_SIZE <= strlen(cmd)) {
                    // The string buffer is too big for our dedicated buffer,
                    // therefore abort the execution
                    return false;
                }

                strcpy(m_p_return_and_argument_buf, cmd);
                break;
#endif // M_P_CFG_TYPE_CHARS

#ifdef M_P_CFG_TYPE_UINT
            case M_P_CMD_GET_ARG_TYPE(M_P_TYPE_UINT):
                for (unsigned int i=0; i<(sizeof(unsigned int)*2) && (0 != *cmd); i++) {
                    cmd = m_p_parse_hex_nibble_to_uint(&ret_arg_uint, cmd);
                }
                break;
#endif // M_P_CFG_TYPE_UINT

#ifdef M_P_CFG_TYPE_UINT32
            case M_P_CMD_GET_ARG_TYPE(M_P_TYPE_UINT32):
                for (unsigned int i=0; i<(sizeof(uint32_t)*2) && (0 != *cmd); i++) {
                    cmd = m_p_parse_hex_nibble_to_uint32(&ret_arg_uint32, cmd);
                }
                break;
#endif // M_P_CFG_TYPE_UINT32

#ifdef M_P_CFG_TYPE_UINT64
            case M_P_CMD_GET_ARG_TYPE(M_P_TYPE_UINT64):
                for (unsigned int i=0; i<(sizeof(uint64_t)*2) && (0 != *cmd); i++) {
                    cmd = m_p_parse_hex_nibble_to_uint64(&ret_arg_uint64, cmd);
                }
                break;
#endif // M_P_CFG_TYPE_UINT64

            default:
                // Whole condition before this switch statement checks for
                // non-void arguments, therefore getting here can't be because
                // a void argument type, but because we might have some novel
                // type which we didn't implemented the 'case' statement for.
                // Therefore abort execution.
                return false;
                break; // Not really needed to break as we return anyway
        }

        // We finished parsing up-to the maximum number bytes for each type.
        // If we are not at the end of the string, then the argument is bigger
        // than its type and therefore abort execution
        if (0 != *cmd) return false;
    }
#endif // M_P_CFG_TYPES_NONE


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
static bool m_p_find_match_and_execute() {
    char buf[M_P_CFG_COMMAND_NAME_SIZE];

#ifndef M_P_CFG_TYPES_NONE
    const int index_of_first_space = m_p_find_first_space(m_p_command_prompt);
    if (-1 != index_of_first_space) {
        // the command contains a space, and most likely argument
        // let cut it short so we can consider only the command
        // name without arguments
        m_p_command_prompt[index_of_first_space] = 0;
    }
#else
    const int index_of_first_space = -1;
#endif

    m_p_iterate_begin();
    while (m_p_iterate_current_exists()) {
        m_p_iterate_get_current_string(buf, false);

        if (0 == strcmp(m_p_command_prompt, buf)) {
            // Found command exactly matching the prompt's command
#ifdef M_P_CFG_UPTIME
            const unsigned int begin = m_p_systick_uptime_ticks;
#endif
            m_p_command selected_command = m_p_iterate_get_current_structure();
            m_p_transport_out("\r\n");

            if (!m_p_execute_command(index_of_first_space, buf, &selected_command)) {
                // problem happened when executing, report failure
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
            if (0 == strlen(m_p_command_prompt)) {
                m_p_print_prompt(NULL);
                break;
            }

#ifdef M_P_CFG_HISTORY
            // Store current command into history
            strcpy(cmd_old, m_p_command_prompt);
#endif

            if (!m_p_find_match_and_execute()) {
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

            // Clear current command
            m_p_command_prompt[0] = 0;
            caret = 0;

            m_p_print_prompt(NULL);
            break;

#ifdef M_P_CFG_AUTOCOMPLETE
        case 0x09:
            // TAB
            m_p_auto_complete(m_p_command_prompt, &caret);
            break;
#endif

        case 0x08:
        case 0x7f:
            // Backspace
            if (caret > 0) {
                // only remove character when there is something to delete
                m_p_command_prompt[--caret] = 0;
                // Go back 1 character, print space and go back 1 character again
                m_p_transport_out("\033[1D \033[1D");
            }
            break;

#ifdef M_P_CFG_ALLOW_QUIT
        case 0x03:
            // Ctrl + C = quit
            m_p_keep_runnning = false;
            break;
#endif

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
                    m_p_command_prompt[caret++] = character;
                    m_p_command_prompt[caret]   = 0;
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
                if (0 == strlen(m_p_command_prompt)) {
                    // When prompt empty, then print at the same prompt
                    m_p_transport_out(cmd_old);
                } else{
                    //  Print completely new prompt
                    m_p_print_prompt(cmd_old);
                }

                strcpy(m_p_command_prompt, cmd_old);
                caret = strlen(m_p_command_prompt);
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


#ifdef M_P_CFG_AUTOCOMPLETE
M_P_CFG_FORCE_OPTIMIZATION
unsigned int m_p_string_differ_at(char *a, char *b) {
    // go through both strings and find first position where they differ
    unsigned int differ_at = 0;

    while ((0 != *a) && (0 != *b) && (*a == *b)) {
        // We are not at the end yet, and the characters still match
        differ_at++;
        a++;
        b++;
    }

    return differ_at;
}
#endif


#pragma mark - Public functions


M_P_CFG_FORCE_OPTIMIZATION
void m_p_prompt_generic() {
#ifdef M_P_CFG_WELCOME_BANNER
    m_p_transport_out("\r\nTest Interface ");
    m_p_transport_out(M_P_VERSION);
    m_p_transport_out("\r\n");
#endif

    m_p_print_prompt(NULL);

#ifdef M_P_CFG_AUTOCOMPLETE
    // Find out what is the length of the longest command, it will be used
    // later to vertically align the commands so their help printout will
    // look consistent
    m_p_iterate_set_size_of_longest_command();
#endif

#ifdef M_P_CFG_ALLOW_QUIT
    m_p_keep_runnning = true;
    while (m_p_keep_runnning) {
#else
    while (1) {
#endif
        char character;
        if (m_p_transport_in(&character)) {
            m_p_evaluate_character(character);
        }
    }

#ifdef M_P_CFG_ALLOW_QUIT
    m_p_transport_out("\r\nExiting ...\r\n");
#endif
}


#ifdef M_P_CFG_AUTOCOMPLETE
M_P_CFG_FORCE_OPTIMIZATION
void m_p_auto_complete(char* command, unsigned int* caret) {
    char         first_command[M_P_CFG_COMMAND_NAME_SIZE];
    char         buf[M_P_CFG_COMMAND_NAME_SIZE + M_P_CFG_AUTOCOMPLETE_HELP_EXTRA_SIZE];
    // TODO: do checks if the size is not too small

    unsigned int common_location = M_P_CFG_COMMAND_NAME_SIZE;
    unsigned int cmd_len         = strlen(command);
    bool         first           = true;

    // Allow auto-complete to work even with empty commands
#ifndef M_P_CFG_AUTOCOMPLETE_ON_EMPTY_PROMPT
    if (0 == cmd_len) {
        // No auto complete with empty line
        return;
    }
#endif


    m_p_iterate_begin();

    // Figure out how far the auto-complete could complete the prompt
    while (m_p_iterate_current_exists()) {
        // Go through all commands, if the start of the command matches the
        // current cmd line, then find how many unique characters can be added
        m_p_iterate_get_current_string(buf, false);
        if (0 == strncmp(command, buf, cmd_len)) {
            // Only consider commands that match with already typed prompt,
            // and compare them to each other to find which is the smallest
            // index where they still are identical
            if (first) {
                strcpy(first_command, buf);
                first = false;
            }

            unsigned int overlap = m_p_string_differ_at(first_command, buf);
            if (overlap < common_location) {
                common_location = overlap;
            }
        }
        m_p_iterate_next();
    }

    if (M_P_CFG_COMMAND_NAME_SIZE == common_location) {
        // The common_location didn't got updated at all, that means
        // no common overlap found, probably wrong command / typo
        // Do not auto complete anything.
        return;
    }

    // Calculated where is the overlap between the commands and the cmd line
    if (cmd_len < common_location) {
        // There are some common characters to add, so then add them

        strncpy(command + cmd_len, first_command + cmd_len, common_location - cmd_len);
        command[common_location] = 0;

        // Display only the added difference as the previous part of the command
        // is already on the UART prompt
        m_p_transport_out_characters(command + cmd_len, common_location - cmd_len);
        *caret = strlen(command);

    } else {
        // The prompt is equal/longer than we could auto-complete. It might be
        // full command

#ifndef M_P_CFG_AUTOCOMPLETE_ON_FULL_COMMAND
        // Detect if it's a full command and abort the auto-complete including
        // the printing of 'help'.
        m_p_iterate_begin();
        while (m_p_iterate_current_exists()) {
            // Search if there is full match
            m_p_iterate_get_current_string(buf, false);
            if (0 == strcmp(command, buf)) {
                // It's exactly the command we wanted to find, so the prompt
                // is fully matching command, abort the autocomplete
                return;
            }
            m_p_iterate_next();
        }
#endif

        // List all the options with their help (partial or full prompts)
        m_p_transport_out_ln();
        m_p_iterate_begin();
        while (m_p_iterate_current_exists()) {
            // Go through all commands, if the start of the command matches the
            m_p_iterate_get_current_string(buf, false);
            if (0 == strncmp(command, buf, cmd_len)) {
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
        m_p_print_prompt(command);
    }
}
#endif
