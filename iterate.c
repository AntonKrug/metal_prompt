/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 20/05/2022
 */

/*
 * Functions which can iterate through the whole command structure and get the
 * the specific command structures, command names and print the command's syntax
 */

#include "iterate.h"

#include "commands_enabled.h"
#include "config.h"
#include "transport/interface.h"
#include "color.h"

#include <string.h>


#pragma mark - Private variables


unsigned int group_idx       = 0;
unsigned int command_idx     = 0;

#ifdef M_P_CFG_AUTOCOMPLETE_ENABLE
unsigned int longest_command = 0;
#endif


#pragma mark - Public functions


M_P_CFG_FORCE_OPTIMIZATION
void m_p_iterate_begin() {
    group_idx   = 0;
    command_idx = 0;
}


M_P_CFG_FORCE_OPTIMIZATION
bool m_p_iterate_is_first() {
    return (0 == group_idx) && (0 == command_idx);
}


M_P_CFG_FORCE_OPTIMIZATION
bool m_p_iterate_current_exists() {
    return (NULL != m_p_commands_enabled[group_idx].namespace);
}


M_P_CFG_FORCE_OPTIMIZATION
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


M_P_CFG_FORCE_OPTIMIZATION
m_p_command m_p_iterate_get_current_structure() {
    return m_p_commands_enabled[group_idx].commands[command_idx];
}


M_P_CFG_FORCE_OPTIMIZATION
unsigned int m_p_iterate_get_current_string(char *buf, bool use_color) {
    unsigned int group_len = strlen(m_p_commands_enabled[group_idx].namespace);

    strcpy(buf,"");
    unsigned int ans = 0;

    if ( group_len != 0 ) {
        // Print namespace of the commands
#if defined(M_P_CFG_COLOR_ENABLE) && defined(M_P_CFG_AUTOCOMPLETE_ENABLE)
        if (use_color) m_p_color_buf_command_namespace(buf);
#endif
        strcat(buf, m_p_commands_enabled[group_idx].namespace);

        // Print in default color '.'
#if defined(M_P_CFG_COLOR_ENABLE) && defined(M_P_CFG_AUTOCOMPLETE_ENABLE)
        if (use_color) m_p_color_buf_default(buf);
#endif
        strcat(buf, ".");
        ans += strlen(m_p_commands_enabled[group_idx].namespace) + 1;
    }

    // Print the name of the command
#if defined(M_P_CFG_COLOR_ENABLE) && defined(M_P_CFG_AUTOCOMPLETE_ENABLE)
    if (use_color) m_p_color_buf_command_name(buf);
#endif
    strcat(buf, m_p_commands_enabled[group_idx].commands[command_idx].command);
    ans += strlen(m_p_commands_enabled[group_idx].commands[command_idx].command);
#if defined(M_P_CFG_COLOR_ENABLE) && defined(M_P_CFG_AUTOCOMPLETE_ENABLE)
    if (use_color) m_p_color_buf_default(buf);
#endif

    return ans; // return count how many characters we added to the buffer
}


#ifdef M_P_CFG_AUTOCOMPLETE_ENABLE
M_P_CFG_FORCE_OPTIMIZATION
void m_p_iterate_get_current_string_arguments(char *buf) {
    // print 'return(' text

    strcpy(buf, "");
#ifdef M_P_CFG_COLOR_ENABLE
    m_p_color_buf_gray(buf);
#endif
    strcat(buf, "return(");
#ifdef M_P_CFG_COLOR_ENABLE
    m_p_color_buf_default_bold(buf);
#endif

    // print return type
    switch (M_P_CMD_MASK_RET & (m_p_commands_enabled[group_idx].commands[command_idx].type)) {
#ifdef M_P_CFG_TYPE_CHARS
        case M_P_CMD_GET_RET_TYPE(M_P_TYPE_CHARS):
            strcat(buf, "char*       ");
            break;
#endif

#ifdef M_P_CFG_TYPE_UINT
        case M_P_CMD_GET_RET_TYPE(M_P_TYPE_UINT):
            strcat(buf, "unsigned int");
        break;
#endif

#ifdef M_P_CFG_TYPE_UINT32
        case M_P_CMD_GET_RET_TYPE(M_P_TYPE_UINT32):
            strcat(buf, "uint32_t    ");
        break;
#endif

#ifdef M_P_CFG_TYPE_UINT64
        case M_P_CMD_GET_RET_TYPE(M_P_TYPE_UINT64):
            strcat(buf, "uint64_t    ");
            break;
#endif

        default:
            strcat(buf, "void        ");
            break;
    }

    // print ') arg('
#ifdef M_P_CFG_COLOR_ENABLE
    m_p_color_buf_gray(buf);
#endif
    strcat(buf, ") arg(");
#ifdef M_P_CFG_COLOR_ENABLE
    m_p_color_buf_default_bold(buf);
#endif

    // print argument's type
    switch (M_P_CMD_MASK_ARG & m_p_commands_enabled[group_idx].commands[command_idx].type) {
#ifdef M_P_CFG_TYPE_CHARS
        case M_P_CMD_GET_ARG_TYPE(M_P_TYPE_CHARS):
            strcat(buf, "char*");
            break;
#endif

#ifdef M_P_CFG_TYPE_UINT
        case M_P_CMD_GET_ARG_TYPE(M_P_TYPE_UINT):
            strcat(buf, "unsigned int");
            break;
#endif

#ifdef M_P_CFG_TYPE_UINT32
        case M_P_CMD_GET_ARG_TYPE(M_P_TYPE_UINT32):
            strcat(buf, "uint32_t");
            break;
#endif

#ifdef M_P_CFG_TYPE_UINT64
        case M_P_CMD_GET_ARG_TYPE(M_P_TYPE_UINT64):
            strcat(buf, "uint64_t");
            break;
#endif

        default:
            strcat(buf, "void");
            break;
    }

    // print close bracket
#ifdef M_P_CFG_COLOR_ENABLE
    m_p_color_buf_gray(buf);
#endif
    strcat(buf, ")");
#ifdef M_P_CFG_COLOR_ENABLE
    m_p_color_buf_default(buf);
#endif
}


M_P_CFG_FORCE_OPTIMIZATION
void m_p_iterate_set_size_of_longest_command(void) {
    char  buf[M_P_CFG_COMMAND_NAME_SIZE];

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
M_P_CFG_FORCE_OPTIMIZATION
void m_p_iterate_align_with_longest_command(unsigned int current_size) {
    for (unsigned int i = 0; i <= longest_command - current_size; ++i) {
        // <= on purpose, to add space even on the longest commands
        m_p_transport_out(" ");
    }
}
#endif // M_P_CFG_AUTOCOMPLETE_ENABLE
