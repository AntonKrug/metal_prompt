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

#define M_P_CFG_TYPE_CHARS // "chars" is pointer of char aka string
#define M_P_CFG_TYPE_UINT    // unsigned int size is platform specific (16-bit minimum size)
//#define M_P_CFG_TYPE_UINT32  // 32-bit unsigned int (uint32_t)
//#define M_P_CFG_TYPE_UINT64  // 64-bit unsigned int (uint64_t)

#ifdef M_P_CFG_TYPE_CHARS
#define M_P_CFG_TYPE_CHARS_BUFFER_SIZE 48 // Max buffer size for argument and return strings
#endif


// Configure corresponding size of the argument buffer depending on the size
// of supported types. Note this will affect the maximum size of the character
// buffer too. So in some cases you might want to override these
#ifdef M_P_CFG_TYPE_UINT64
#define M_P_CFG_COMMAND_ARG_SIZE  19   // 19 characters => 64-bit hex argument (0x8000000000000000)
#else

#ifdef M_P_CFG_TYPE_UINT32
#define M_P_CFG_COMMAND_ARG_SIZE  11   // 11 characters => 32-bit hex argument (0x80000000)
#else
#define M_P_CFG_COMMAND_ARG_SIZE  (3+(2*sizeof(unsigned int)))  // '0x' + termination + 2chars per byte
#endif // M_P_CFG_TYPE_UINT32


// -----------------------------------------------------------------------------
#pragma mark - Configuration of features

#define M_P_CFG_COMMAND_PROMPT "metal_prompt:/$ "
#define M_P_CFG_COMMAND_NAME_SIZE 48   // biggest length the command name can have (including namespace)
#define M_P_CFG_WHOLE_PROMPT_SIZE (M_P_CFG_COMMAND_NAME_SIZE + M_P_CFG_COMMAND_ARG_SIZE + 1) // Whole prompt buffer
#define M_P_CFG_FORCE_OPTIMIZATION __attribute__((optimize("-Os"))) // comment out to disable optimizations
#define M_P_CFG_ALLOW_QUIT             // run in infinite loop or be able to exit
#define M_P_CFG_PRINT_DEC_DEFAULT true // printing return values in HEX or DEC
//#define M_P_CFG_UPTIME                 // uptime and benchmark commands (see systick.c)
//#define M_P_CFG_UPTIME_SMALL_OVERFLOW  // do check of uptime overflowing on small platforms
#define M_P_CFG_COLORS                 // enable/disable vt100 colors
#define M_P_CFG_HISTORY                // remember previous command (currently only 1)
//#define M_P_CFG_BELL                   // ring vt100 BELL when issues happen
#define M_P_CFG_MEMORY_DUMP            // enable memory dump commands (needs M_P_CFG_TYPE_UINT)


// -----------------------------------------------------------------------------
#pragma mark - Configuration of autocomplete

#define M_P_CFG_AUTOCOMPLETE                    // completely enable/disable autocomplete support
#define M_P_CFG_AUTOCOMPLETE_TYPE_MAX_STRING 13 // 'unsigned int' is the longest string of a supported type
#define M_P_CFG_AUTOCOMPLETE_HELP_EXTRA_SIZE 40 // 40 chars extra to print command's help (based on 13 char type string)
// The two features below produce smaller footprint when they are enabled
#define M_P_CFG_AUTOCOMPLETE_ON_EMPTY_PROMPT    // will list all possible commands
#define M_P_CFG_AUTOCOMPLETE_ON_FULL_COMMAND    // will print 'help' for that command

#endif // M_P_CFG_TYPE_UINT64


#if !defined(M_P_CFG_TYPE_CHARS) && !defined(M_P_CFG_TYPE_UINT) && !defined(M_P_CFG_TYPE_UINT32) && !defined(M_P_CFG_TYPE_UINT64)
#define M_P_CFG_TYPES_NONE
#endif

#ifndef M_P_CFG_FORCE_OPTIMIZATION
#define M_P_CFG_FORCE_OPTIMIZATION // if nothing defined, then at least define empty one
#endif

#ifdef __cplusplus
}
#endif

#endif /* METAL_PROMPT_CONFIG_H_ */
