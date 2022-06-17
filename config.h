/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */

/*
 * Global config to tweak and enable/disable certain features of metal prompt
 */


#ifndef METAL_PROMPT_CONFIG_H_
#define METAL_PROMPT_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>


// -----------------------------------------------------------------------------
#pragma mark - Supported callback argument and return types

#define M_P_CFG_TYPE_CHARS   // "CHARS" is a pointer to char (aka string)
#define M_P_CFG_TYPE_UINT    // unsigned int size is platform specific (16-bit minimum size)
//#define M_P_CFG_TYPE_UINT32  // 32-bit unsigned int (uint32_t)
//#define M_P_CFG_TYPE_UINT64  // 64-bit unsigned int (uint64_t)

// Configured how much the command prompt buffer needs to enlarged to fit
// the biggest command name and required arguments at the same time.
// This is for the very worse case scenario and the absolute limit of
// the prompt length.
// Note this will have side effect on the maximum size of the CHARS argument too.
// In some cases you might want to override these defaults
#if defined(M_P_CFG_TYPE_UINT64) && !defined(M_P_CFG_TYPE_UINT32)
#define M_P_CFG_COMMAND_ARG_SIZE  19   // 19 characters => 64-bit hex argument (0x8000000000000000)
#endif

#if defined(M_P_CFG_TYPE_UINT32) && !defined(M_P_CFG_TYPE_UINT64)
#define M_P_CFG_COMMAND_ARG_SIZE  11   // 11 characters => 32-bit hex argument (0x80000000)
#endif

#if !defined(M_P_CFG_TYPE_UINT32) && !defined(M_P_CFG_TYPE_UINT64)
#define M_P_CFG_COMMAND_ARG_SIZE  (3+(2*sizeof(unsigned int)))  // '0x' + termination + 2chars per byte
#endif

#ifdef M_P_CFG_TYPE_CHARS
// Even the buffer for arguments might be limited by the maximum of the prompt
// buffer (affect by M_P_CFG_COMMAND_ARG_SIZE value), CHARS callbacks can still
// have own dedicated CHARS return buffer (which can be bigger than its
// arguments). This return buffer is shared with argument buffer, so if the
// prompt short (short command) then the argument can contain more characters
// than the value of M_P_CFG_COMMAND_ARG_SIZE.
// Max buffer size for return CHARS (and argument CHARS too):
#define M_P_CFG_TYPE_CHARS_BUFFER_SIZE 48
#endif



// -----------------------------------------------------------------------------
#pragma mark - Configuration of features

#define M_P_CFG_COMMAND_PROMPT "m_p:/$ "
#define M_P_CFG_VERSION "v0.1"
#define M_P_CFG_COMMAND_NAME_SIZE 48   // Biggest length the command name can have (including namespace)
#define M_P_CFG_WHOLE_PROMPT_SIZE (M_P_CFG_COMMAND_NAME_SIZE + M_P_CFG_COMMAND_ARG_SIZE + 1) // Whole prompt buffer
#define M_P_CFG_FORCE_OPTIMIZATION __attribute__((optimize("-Os"))) // Comment this out to disable optimizations
#define M_P_CFG_WELCOME_BANNER         // Print Welcome banner on startup
#define M_P_CFG_HELP                   // Enable the "help" command
#define M_P_CFG_PRINT_DEC_DEFAULT true // Printing return values in HEX or DEC
//#define M_P_CFG_UPTIME                 // Uptime and benchmark commands (see systick.c)
//#define M_P_CFG_UPTIME_SMALL_OVERFLOW  // Do check of uptime overflowing on small platforms
#define M_P_CFG_COLORS                 // Enable/disable vt100 colors
#define M_P_CFG_HISTORY                // Remember previous command (currently only 1)
//#define M_P_CFG_BELL                   // Ring vt100 BELL when issues happen
#define M_P_CFG_MEMORY_DUMP            // Enable memory dump commands (needs M_P_CFG_TYPE_UINT)
#define M_P_CFG_ALLOW_QUIT             // Run in infinite loop or be able to exit


// -----------------------------------------------------------------------------
#pragma mark - Configuration of autocomplete

#define M_P_CFG_AUTOCOMPLETE                    // completely enable/disable autocomplete support
#define M_P_CFG_AUTOCOMPLETE_TYPE_MAX_STRING 13 // 'unsigned int' is the longest string of a supported type
#define M_P_CFG_AUTOCOMPLETE_HELP_EXTRA_SIZE 40 // 40 chars extra to print command's help (based on 13 char type string)
// The two features below produce smaller footprint when they are enabled
#define M_P_CFG_AUTOCOMPLETE_ON_EMPTY_PROMPT    // will list all possible commands
#define M_P_CFG_AUTOCOMPLETE_ON_FULL_COMMAND    // will print 'help' for that command


#if !defined(M_P_CFG_TYPE_CHARS) && !defined(M_P_CFG_TYPE_UINT) && !defined(M_P_CFG_TYPE_UINT32) && !defined(M_P_CFG_TYPE_UINT64)
#define M_P_CFG_TYPES_NONE
#endif


#ifndef M_P_CFG_FORCE_OPTIMIZATION
// if no M_P_CFG_FORCE_OPTIMIZATION defined (optimizations are disabled), then
// at least define empty one so the project will compile without errors
#define M_P_CFG_FORCE_OPTIMIZATION
#endif



#ifdef __cplusplus
}
#endif

#endif /* METAL_PROMPT_CONFIG_H_ */
