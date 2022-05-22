/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 20/05/2022
 */

#include "iterate.h"

#include "commands_enabled.h"
#include "config.h"
#include "transport/interface.h"

#include <string.h>


#pragma mark - Private variables


unsigned int group_idx       = 0;
unsigned int command_idx     = 0;

#ifdef M_P_LIST_AND_AUTOCOMPLETE_ENABLE
unsigned int longest_command = 0;
#endif


#pragma mark - Private functions - color handling inside a buffer


M_P_FORCE_OPTIMIZATION
static void m_p_color_buf_default(char* buf) {
#ifdef M_P_COLOR_ENABLE
    strcat(buf, "\033[0;39m");
#endif
}


#ifdef M_P_LIST_AND_AUTOCOMPLETE_ENABLE
M_P_FORCE_OPTIMIZATION
static void m_p_color_buf_default_bold(char* buf) {
#ifdef M_P_COLOR_ENABLE
    strcat(buf, "\033[1;39m");
#endif
}
#endif


M_P_FORCE_OPTIMIZATION
static inline void m_p_color_buf_command_namespace(char* buf) {
#ifdef M_P_LIST_AND_AUTOCOMPLETE_ENABLE
#ifdef M_P_COLOR_ENABLE
    strcat(buf, "\033[1;33m");
#endif
#endif
}


M_P_FORCE_OPTIMIZATION
static inline void m_p_color_buf_command_name(char* buf) {
#ifdef M_P_LIST_AND_AUTOCOMPLETE_ENABLE
#ifdef M_P_COLOR_ENABLE
    strcat(buf, "\033[1;35m");
#endif
#endif
}


#ifdef M_P_LIST_AND_AUTOCOMPLETE_ENABLE
M_P_FORCE_OPTIMIZATION
static void m_p_color_buf_gray(char* buf) {
#ifdef M_P_COLOR_ENABLE
    strcat(buf, "\033[1;30m");
#endif
}
#endif


#pragma mark - Public functions - iterate


M_P_FORCE_OPTIMIZATION
void m_p_iterate_begin() {
    group_idx   = 0;
    command_idx = 0;
}


M_P_FORCE_OPTIMIZATION
bool m_p_iterate_is_first() {
    return (0 == group_idx) && (0 == command_idx);
}


M_P_FORCE_OPTIMIZATION
bool m_p_iterate_current_exists() {
    return (NULL != m_p_commands_enabled[group_idx].namespace);
}


M_P_FORCE_OPTIMIZATION
void m_p_iterate_next() {
    command_idx++;
    if (NULL == m_p_commands_enabled[group_idx].commands[command_idx].command) {
        // Finished with current group, select new group
        command_idx = 0;
        group_idx++;
        // no need to check here if it's valid group as the loops later call the
        // m_p_iterate_current_exists which will check if it's end of the groups
    }
}


M_P_FORCE_OPTIMIZATION
m_p_command m_p_iterate_get_current_structure() {
    return m_p_commands_enabled[group_idx].commands[command_idx];
}


M_P_FORCE_OPTIMIZATION
unsigned int m_p_iterate_get_current_string(char *buf, bool use_color) {
    unsigned int group_len = strlen(m_p_commands_enabled[group_idx].namespace);

    strcpy(buf,"");
    unsigned int ans = 0;

    if ( group_len != 0 ) {
        // Print namespace of the commands
        if (use_color) m_p_color_buf_command_namespace(buf);
        strcat(buf, m_p_commands_enabled[group_idx].namespace);

        // Print in default color '.'
        if (use_color) m_p_color_buf_default(buf);
        strcat(buf, ".");
        ans += strlen(m_p_commands_enabled[group_idx].namespace) + 1;
    }

    // Print the name of the command
    if (use_color) m_p_color_buf_command_name(buf);
    strcat(buf, m_p_commands_enabled[group_idx].commands[command_idx].command);
    ans += strlen(m_p_commands_enabled[group_idx].commands[command_idx].command);
    if (use_color) m_p_color_buf_default(buf);

    return ans; // return count how many characters we added to the buffer
}


#ifdef M_P_LIST_AND_AUTOCOMPLETE_ENABLE
M_P_FORCE_OPTIMIZATION
void m_p_iterate_get_current_string_arguments(char *buf) {
    strcpy(buf, "");
    m_p_color_buf_gray(buf);
    strcat(buf, "return(");
    m_p_color_buf_default_bold(buf);

    switch (m_p_commands_enabled[group_idx].commands[command_idx].type) {
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
        case M_P_COMMAND_TYPE_RET_CHARS_ARG_VOID:
        case M_P_COMMAND_TYPE_RET_CHARS_ARG_CHARS:
        case M_P_COMMAND_TYPE_RET_CHARS_ARG_UINT32:
#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
            case M_P_COMMAND_TYPE_RET_CHARS_ARG_UINT64:
#endif
            strcat(buf, "char*   ");
            break;
#endif

        case M_P_COMMAND_TYPE_RET_UINT32_ARG_VOID:
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
        case M_P_COMMAND_TYPE_RET_UINT32_ARG_CHARS:
#endif
        case M_P_COMMAND_TYPE_RET_UINT32_ARG_UINT32:
#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
        case M_P_COMMAND_TYPE_RET_UINT32_ARG_UINT64:
#endif
            strcat(buf, "uint32_t");
        break;

#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
        case M_P_COMMAND_TYPE_RET_UINT64_ARG_VOID:
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
        case M_P_COMMAND_TYPE_RET_UINT64_ARG_CHARS:
#endif
        case M_P_COMMAND_TYPE_RET_UINT64_ARG_UINT32:
        case M_P_COMMAND_TYPE_RET_UINT64_ARG_UINT64:
            strcat(buf, "uint64_t");
            break;
#endif

        default:
            strcat(buf, "void    ");
            break;
    }

    m_p_color_buf_gray(buf);
    strcat(buf, ") arg(");
    m_p_color_buf_default_bold(buf);

    switch (m_p_commands_enabled[group_idx].commands[command_idx].type) {
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE

        case M_P_COMMAND_TYPE_RET_VOID_ARG_CHARS:
        case M_P_COMMAND_TYPE_RET_CHARS_ARG_CHARS:
        case M_P_COMMAND_TYPE_RET_UINT32_ARG_CHARS:
#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
        case M_P_COMMAND_TYPE_RET_UINT64_ARG_CHARS:
#endif
            strcat(buf, "char*");
            break;
#endif

        case M_P_COMMAND_TYPE_RET_VOID_ARG_UINT32:
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
        case M_P_COMMAND_TYPE_RET_CHARS_ARG_UINT32:
#endif
        case M_P_COMMAND_TYPE_RET_UINT32_ARG_UINT32:
#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
        case M_P_COMMAND_TYPE_RET_UINT64_ARG_UINT32:
#endif
            strcat(buf, "uint32_t");
            break;

#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
        case M_P_COMMAND_TYPE_RET_VOID_ARG_UINT64:
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
        case M_P_COMMAND_TYPE_RET_CHARS_ARG_UINT64:
#endif
        case M_P_COMMAND_TYPE_RET_UINT32_ARG_UINT64:
        case M_P_COMMAND_TYPE_RET_UINT64_ARG_UINT64:
            strcat(buf, "uint64_t");
            break;
#endif

        default:
            strcat(buf, "void");
            break;
    }

    m_p_color_buf_gray(buf);
    strcat(buf, ")");
    m_p_color_buf_default(buf);
}


M_P_FORCE_OPTIMIZATION
void m_p_iterate_set_size_of_longest_command(void) {
    char  buf[M_P_COMMAND_NAME_LIMIT];

    m_p_iterate_begin();
    while (m_p_iterate_current_exists()) {
        m_p_iterate_get_current_string(buf, false);
        unsigned int current = strlen(buf);
        if (longest_command < current) {
            longest_command = current;
        }
        m_p_iterate_next();
    }
}


// Find align the current string with the longest command
M_P_FORCE_OPTIMIZATION
void m_p_iterate_align_with_longest_command(unsigned int current_size) {
    for (unsigned int i = 0; i <= longest_command - current_size; ++i) {
        // <= on purpose, to add space even on the longest commands
        m_p_transport_out(" ");
    }
}
#endif // M_P_LIST_AND_AUTOCOMPLETE_ENABLE
