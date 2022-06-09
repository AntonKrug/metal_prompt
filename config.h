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
#pragma mark - Configuration of features

#define M_P_CFG_COMMAND_PROMPT "metal_prompt:/$ "
#define M_P_CFG_COMMAND_NAME_SIZE 48   // biggest length the command name can have (including namespace)
//#define M_P_CFG_COMMAND_ARG_SIZE  19   // 19 characters => 64-bit hex argument (0x8000000000000000)
#define M_P_CFG_COMMAND_ARG_SIZE  11   // 11 characters => 32-bit hex argument (0x80000000)
#define M_P_CFG_FORCE_OPTIMIZATION __attribute__((optimize("-Os")))
//#define M_P_CFG_FORCE_OPTIMIZATION   // uncomment this line instead for no optimizations
#define M_P_CFG_ALLOW_QUIT             // run in infinite loop or be able to exit
#define M_P_CFG_PRINT_DEC_DEFAULT true // printing return values in HEX or DEC
#define M_P_CFG_UPTIME                 // uptime and benchmark commands (see systick.c)
#define M_P_CFG_COLORS                 // enable/disable vt100 colors


// -----------------------------------------------------------------------------
#pragma mark - Configuration of autocomplete

#define M_P_CFG_AUTOCOMPLETE                 // completely enable/disable autocomplete support
#define M_P_CFG_AUTOCOMPLETE_ON_EMPTY_PROMPT // will list all possible commands
#define M_P_CFG_AUTOCOMPLETE_ON_FULL_COMMAND // will print 'help' for that command


// -----------------------------------------------------------------------------
#pragma mark - Supported callback argument and return types

//#define M_P_CFG_TYPE_CHARS // "chars" is pointer of char aka string
#define M_P_CFG_TYPE_UINT    // unsigned int size is platform specific (16-bit min)
//#define M_P_CFG_TYPE_UINT32  // 32-bit unsigned int (uint32_t)
//#define M_P_CFG_TYPE_UINT64  // 64-bit unsigned int (uint64_t)

#ifdef M_P_CFG_TYPE_CHARS
#define M_P_CFG_TYPE_CHARS_BUFFER_SIZE 48 // Max buffer size for argument and return strings
#endif


#ifdef __cplusplus
}
#endif

#endif /* METAL_PROMPT_CONFIG_H_ */
