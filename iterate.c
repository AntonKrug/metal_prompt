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


#pragma mark Private variables
uint32_t list_group_index   = 0;
uint32_t list_command_index = 0;
uint32_t longest_command    = 0;


#pragma mark Public functions
void m_p_iterate_begin() {
    list_group_index   = 0;
    list_command_index = 0;
}


bool m_p_iterate_is_first() {
    return (list_group_index == 0) && (list_command_index == 0);
}


bool m_p_iterate_current_exists() {
    if ( list_group_index < M_P_ENABLED_SIZE &&
            list_command_index < m_p_commands_enabled[list_group_index].testsSize) {
        return true;
    }
    return false;
}


void m_p_iterate_next() {
    list_command_index++;
    if (list_command_index >= m_p_commands_enabled[list_group_index].testsSize) {
        // Finished with current group, select new group
        list_command_index = 0;
        list_group_index++;  // no need to check here if it's valid group
    }
}


m_p_command m_p_iterate_get_current_structure() {
    return m_p_commands_enabled[list_group_index].tests[list_command_index];
}


uint32_t m_p_iterate_get_current_string(char *buf, bool use_color) {
    uint32_t group_len = strlen(m_p_commands_enabled[list_group_index].group_name);

    strcpy(buf,"");
    uint32_t ans = 0;

    if ( group_len != 0 ) {
        if (use_color) strcat(buf, "\033[1;33m");
        strcat(buf, m_p_commands_enabled[list_group_index].group_name);
        if (use_color) strcat(buf, "\033[1;39m");
        strcat(buf, ".");
        ans += strlen(m_p_commands_enabled[list_group_index].group_name) + 1;
    }
    if (use_color) strcat(buf, "\033[1;35m");
    strcat(buf, m_p_commands_enabled[list_group_index].tests[list_command_index].command);
    ans += strlen(m_p_commands_enabled[list_group_index].tests[list_command_index].command);
    if (use_color) strcat(buf, "\033[0;39m");

    return ans;
}


void m_p_iterate_get_current_string_arguments(char *buf) {
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


void m_p_iterate_set_size_of_longest_command(void) {
    char  buf[M_P_COMMAND_NAME_LIMIT];

    m_p_iterate_begin();
    while (m_p_iterate_current_exists()) {
        m_p_iterate_get_current_string(buf, false);
        uint32_t current = strlen(buf);
        if (longest_command < current) {
            longest_command = current;
        }
        m_p_iterate_next();
    }
}


// Find align the current string with the longest command
void m_p_iterate_align_with_longest_command(uint32_t current_size) {
    for (uint32_t i = 0; i <= longest_command - current_size; ++i) {
        // <= on purpose, to add space even on the longest commands
        m_p_transport_out(" ");
    }
}
