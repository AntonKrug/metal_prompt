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

#ifdef M_P_CFG_AUTOCOMPLETE
unsigned int longest_command = 0;

// This NEEDS to match with the structures.h!
const char m_p_command_type_string[][M_P_CFG_AUTOCOMPLETE_TYPE_MAX_STRING] = {
        "void        ",
#ifdef M_P_CFG_TYPE_CHARS
        "char *      ",
#endif
#ifdef M_P_CFG_TYPE_UINT
        "unsigned int",
#endif
#ifdef M_P_CFG_TYPE_UINT32
        "uint32_t    ",
#endif
#ifdef M_P_CFG_TYPE_UINT64
        "uint64_t    ",
#endif
};
#endif // M_P_CFG_AUTOCOMPLETE


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
unsigned int m_p_iterate_get_current_string(char *buf, const bool use_color) {
    unsigned int group_len = strlen(m_p_commands_enabled[group_idx].namespace);

    strcpy(buf,"");
    unsigned int ans = 0;

    if (0 != group_len) {
        // Print namespace of the commands
#if defined(M_P_CFG_COLORS) && defined(M_P_CFG_AUTOCOMPLETE)
        if (use_color) m_p_color_buf_command_namespace(buf);
#endif
        strcat(buf, m_p_commands_enabled[group_idx].namespace);

        // Print in default color '.'
#if defined(M_P_CFG_COLORS) && defined(M_P_CFG_AUTOCOMPLETE)
        if (use_color) m_p_color_buf_default(buf);
#endif
        strcat(buf, ".");
        ans += strlen(m_p_commands_enabled[group_idx].namespace) + 1;
    }

    // Print the name of the command
#if defined(M_P_CFG_COLORS) && defined(M_P_CFG_AUTOCOMPLETE)
    if (use_color) m_p_color_buf_command_name(buf);
#endif
    strcat(buf, m_p_commands_enabled[group_idx].commands[command_idx].command);
    ans += strlen(m_p_commands_enabled[group_idx].commands[command_idx].command);
#if defined(M_P_CFG_COLORS) && defined(M_P_CFG_AUTOCOMPLETE)
    if (use_color) m_p_color_buf_default(buf);
#endif

    return ans; // return count how many characters we added to the buffer
}


#ifdef M_P_CFG_AUTOCOMPLETE
M_P_CFG_FORCE_OPTIMIZATION
void m_p_iterate_get_current_string_arguments(char *buf) {
    // print 'return(' text

    strcpy(buf, "");
#ifdef M_P_CFG_COLORS
    m_p_color_buf_gray(buf);
#endif
    strcat(buf, "return(");
#ifdef M_P_CFG_COLORS
    m_p_color_buf_default_bold(buf);
#endif

    // print return type
    unsigned int returnOneHot =
            M_P_CMD_MASK_RET & m_p_commands_enabled[group_idx].commands[command_idx].type;
    unsigned int returnIndex = 0;
    while (returnOneHot > 1) {
        returnIndex++;
        returnOneHot >>= 1;
    }
    strcat(buf, m_p_command_type_string[returnIndex]);

    // print ') arg('
#ifdef M_P_CFG_COLORS
    m_p_color_buf_gray(buf);
#endif
    strcat(buf, ") arg(");
#ifdef M_P_CFG_COLORS
    m_p_color_buf_default_bold(buf);
#endif

    // print argument's type
    unsigned int argumentOneHot =
            (M_P_CMD_MASK_ARG & m_p_commands_enabled[group_idx].commands[command_idx].type) >> M_P_TYPE_LAST;
    unsigned int argumentIndex = 0;
    while (argumentOneHot > 1) {
        argumentIndex++;
        argumentOneHot >>= 1;
    }
    strcat(buf, m_p_command_type_string[argumentIndex]);

    // print close bracket
#ifdef M_P_CFG_COLORS
    m_p_color_buf_gray(buf);
#endif
    strcat(buf, ")");
#ifdef M_P_CFG_COLORS
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
void m_p_iterate_align_with_longest_command(const unsigned int current_size) {
    for (unsigned int i = 0; i <= longest_command - current_size; ++i) {
        // <= on purpose, to add space even on the longest commands
        m_p_transport_out(" ");
    }
}
#endif // M_P_CFG_AUTOCOMPLETE
