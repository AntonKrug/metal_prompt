/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */

/*
 * The default set of prompt commands which are bundled with the metal prompt
 */

#include "commands_bundled.h"

#include "metal_prompt.h"
#include "transport/interface.h"
#include "systick.h"
#include "globals.h"
#include "config.h"
#include "color.h"

#include <stdlib.h>
#include <string.h>


#pragma mark - The default set prompt commands


#ifdef M_P_CFG_AUTOCOMPLETE
M_P_CFG_FORCE_OPTIMIZATION
void m_p_bundled_list_all_tests(void) {
	strcpy(m_p_command_prompt, "");
	unsigned int carret = 0;


	// Auto-complete now list commands on the when empty command is given
	m_p_auto_complete(m_p_command_prompt, &carret);
}
#endif


#ifdef M_P_CFG_CONFIGURATION
M_P_CFG_FORCE_OPTIMIZATION
void m_p_bundled_configuration(void) {
	char optimization[5];
#ifdef NDEBUG
	m_p_transport_out("Configuration: Release\r\n");
#else
	m_p_transport_out("Configuration: Debug\r\n");
#endif

#ifdef __OPTIMIZE__
	m_p_transport_out("__OPTIMIZE__\r\n");
#endif

#ifdef __OPTIMIZE_SIZE__
	m_p_transport_out("__OPTIMIZE_SIZE__\r\n");
#endif

#ifndef __OPTIMIZE__
#ifndef __OPTIMIZE_SIZE__
	// Debug configuration
	m_p_transport_out("No speed nor size optimizations\r\n");
#endif
#endif

#ifdef __NO_INLINE__
	m_p_transport_out("__NO_INLINE__\r\n");
#endif

}
#endif // M_P_CFG_CONFIGURATION


#ifdef M_P_CFG_HELP
M_P_CFG_FORCE_OPTIMIZATION
void m_p_bundled_help(void) {
    m_p_transport_out("Help: \r\n\r\n");

#ifdef M_P_CFG_ALLOW_QUIT
    m_p_transport_out(" Ctrl+C exits the metal prompt interface\r\n\r\n");
#endif

#ifdef M_P_CFG_HISTORY
    m_p_transport_out(" Up arrow repeats the last command\r\n\r\n");
#endif

#ifdef M_P_CFG_AUTOCOMPLETE
    m_p_transport_out(" TAB auto-completes the command, if there are multiple options,\r\n");
    m_p_transport_out(" then it will list all options.\r\n\r\n");

#ifdef M_P_CFG_AUTOCOMPLETE_ON_EMPTY_PROMPT
    m_p_transport_out(" Pressing TAB on empty prompt will list all possible commands (same as ls command).\r\n\r\n");
#endif

#ifdef M_P_CFG_AUTOCOMPLETE_ON_FULL_COMMAND
    m_p_transport_out(" Pressing TAB on a complete command will print its signature.\r\n\r\n");
#endif

#endif // M_P_CFG_AUTOCOMPLETE
}
#endif // M_P_CFG_HELP


#ifdef M_P_CFG_CLEAR_SCREEN
M_P_CFG_FORCE_OPTIMIZATION
void m_p_bundled_clear(void) {
    m_p_transport_out("\033[2J\033[H");
}
#endif


#ifdef M_P_CFG_UPTIME

M_P_CFG_FORCE_OPTIMIZATION
unsigned int m_p_bundled_uptime(void) {
	return m_p_systick_uptime_ticks;
}


M_P_CFG_FORCE_OPTIMIZATION
void m_p_bundled_set_command_benchmark(unsigned int value) {
    m_p_benchmark_commands = value;
}

#endif //M_P_CFG_UPTIME


#ifdef M_P_CFG_ALLOW_QUIT
M_P_CFG_FORCE_OPTIMIZATION
void m_p_bundled_quit(void) {
	m_p_keep_runnning = false;
}
#endif


#ifdef M_P_CFG_COLORS
M_P_CFG_FORCE_OPTIMIZATION
void m_p_bundled_set_prompt_color(unsigned int val) {
    if (val) {
        m_p_colors_enabled = true;
    } else {
        m_p_colors_enabled = false;
    }
}
#endif


#if defined(M_P_CFG_TYPE_UINT) && !defined(M_P_CFG_PRINT_ONLY_HEX)
M_P_CFG_FORCE_OPTIMIZATION
void m_p_bundled_print_in_dec(unsigned int val) {
    if (val) {
        m_p_print_in_dec = true;
    } else {
        m_p_print_in_dec = false;
    }
}
#endif


#if defined(M_P_CFG_MEMORY_DUMP) && defined(M_P_CFG_TYPE_UINT)
M_P_CFG_FORCE_OPTIMIZATION
void m_p_bundled_memory_dump_word(unsigned int addr) {
    unsigned int *p = (void *)addr;
    char ch[2];
    for (unsigned int y=0; y<10; y++) {
        for (unsigned int x=0; x<16; x += sizeof(unsigned int), p++) {
            unsigned int val = *p;
            for (int i=0; i< (sizeof(unsigned int)*2); i++) {
                ch[0] = ((val & 0xf)>9) ? ((val & 0xf) - 10 + 'a') : ((val & 0xf) + '0');
                m_p_transport_out_characters(ch, 1);
                val = val >> 4;
            }
            ch[0] = ' ';
            m_p_transport_out_characters(ch, 1);
        }
        ch[0]='\r';
        ch[1]='\n';
        m_p_transport_out_characters(ch, 2);
    }
}


void m_p_bundled_memory_dump_byte(unsigned int addr) {
    uint8_t *p = (void *)addr;
    char ch[3];
    ch[2] = ' '; // hard-code the 3rd character to be a space

    for (unsigned int y=0; y<10; y++) {
        for (unsigned int x=0; x<16; x ++, p++) {
            unsigned int val = *p;
            ch[0] = ((val & 0xf)>9) ? ((val & 0xf) - 10 + 'a') : ((val & 0xf) + '0');
            val = val >> 4;
            ch[1] = ((val & 0xf)>9) ? ((val & 0xf) - 10 + 'a') : ((val & 0xf) + '0');

            m_p_transport_out_characters(ch, 3); // 3rd character is the space
        }
        ch[0]='\r';
        ch[1]='\n';
        m_p_transport_out_characters(ch, 2);
    }
}
#endif // defined(M_P_CFG_MEMORY_DUMP) && defined(M_P_CFG_TYPE_UINT)


const m_p_command m_p_bundled_list[] = {


#ifdef M_P_CFG_CLEAR_SCREEN
        { "clear",                 M_P_CMD_TYPES(M_P_TYPE_VOID, M_P_TYPE_VOID),   .void_void   = { &m_p_bundled_clear} },
#endif


#ifdef M_P_CFG_CONFIGURATION
        { "configuration",         M_P_CMD_TYPES(M_P_TYPE_VOID, M_P_TYPE_VOID),   .void_void   = { &m_p_bundled_configuration } },
#endif


#if defined(M_P_CFG_MEMORY_DUMP) && defined(M_P_CFG_TYPE_UINT)
        { "dump_byte",             M_P_CMD_TYPES(M_P_TYPE_VOID, M_P_TYPE_UINT),   .void_uint   = { &m_p_bundled_memory_dump_byte } },
        { "dump_word",             M_P_CMD_TYPES(M_P_TYPE_VOID, M_P_TYPE_UINT),   .void_uint   = { &m_p_bundled_memory_dump_word } },
#endif


#ifdef M_P_CFG_HELP
        { "help",                  M_P_CMD_TYPES(M_P_TYPE_VOID, M_P_TYPE_VOID),   .void_void   = { &m_p_bundled_help } },
#endif


#ifdef M_P_CFG_AUTOCOMPLETE
        { "ls",                    M_P_CMD_TYPES(M_P_TYPE_VOID, M_P_TYPE_VOID),   .void_void   = { &m_p_bundled_list_all_tests } },
#endif


#if defined(M_P_CFG_TYPE_UINT) && !defined(M_P_CFG_PRINT_ONLY_HEX)
        { "print_in_dec"         , M_P_CMD_TYPES(M_P_TYPE_VOID, M_P_TYPE_UINT),   .void_uint   = { &m_p_bundled_print_in_dec } },
#endif


#if defined(M_P_CFG_COLORS) && defined(M_P_CFG_TYPE_UINT)
        { "set_prompt_color",      M_P_CMD_TYPES(M_P_TYPE_VOID, M_P_TYPE_UINT),   .void_uint = { &m_p_bundled_set_prompt_color } },
#endif


#ifdef M_P_CFG_UPTIME
        { "set_command_benchmark", M_P_CMD_TYPES(M_P_TYPE_VOID, M_P_TYPE_UINT),   .void_uint   = { &m_p_bundled_set_command_benchmark } },
#ifdef M_P_CFG_TYPE_UINT
        { "uptime",                M_P_CMD_TYPES(M_P_TYPE_UINT, M_P_TYPE_VOID),   .uint_void   = { &m_p_bundled_uptime } },
#endif // M_P_CFG_TYPE_UINT32
#endif // M_P_CFG_UPTIME


#ifdef M_P_CFG_ALLOW_QUIT
        { "quit",                  M_P_CMD_TYPES(M_P_TYPE_VOID, M_P_TYPE_VOID),   .void_void   = { &m_p_bundled_quit } },
#endif


        { NULL }  // Terminator of the list
};

