/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */

/*
 * Global config to tweak, or enable/disable certain features of metal prompt
 */


#ifndef METAL_PROMPT_CONFIG_H_
#define METAL_PROMPT_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>


#pragma mark - Configuration of features


#define M_P_CFG_COMMAND_PROMPT "metal_prompt:/$ "
#define M_P_CFG_COMMAND_NAME_SIZE 128
#define M_P_CFG_FORCE_OPTIMIZATION __attribute__((optimize("-Os")))
//#define M_P_CFG_FORCE_OPTIMIZATION // uncomment this line instead for no optimizations
#define M_P_CFG_ALLOW_QUIT
#define M_P_CFG_PRINT_DEC_DEFAULT true

// Enable/disable the uptime and benchmark commands (see systick.c)
//#define M_P_CFG_UPTIME

#define M_P_CFG_COLOR_ENABLE


#pragma mark - Configuration of autocomplete


#define M_P_CFG_AUTOCOMPLETE_ENABLE
#ifdef M_P_CFG_AUTOCOMPLETE_ENABLE

// Enable autocomplete even on completely empty command prompt
#define M_P_CFG_AUTOCOMPLETE_ON_EMPTY_COMMANDS

// Uncomment this when you want the list and it's arguments be triggered
// even when the command is already written in full
#define M_P_CFG_AUTOCOMPLETE_ON_FULL_COMMANDS

#endif // M_P_CFG_AUTOCOMPLETE_ENABLE


#pragma mark - Supported return and argument types


#define M_P_CFG_TYPE_CHARS
//#define M_P_CFG_TYPE_UINT    // its size is platform specific (16-bit min)
//#define M_P_CFG_TYPE_UINT32
#define M_P_CFG_TYPE_UINT64

#ifdef M_P_CFG_TYPE_CHARS
#define M_P_CFG_TYPE_CHARS_BUFFER_SIZE 48
#endif


#ifdef __cplusplus
}
#endif

#endif /* METAL_PROMPT_CONFIG_H_ */
