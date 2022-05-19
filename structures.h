/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#ifndef METAL_PROMPT_STRUCTURES_H_
#define METAL_PROMPT_STRUCTURES_H_

#include <stdint.h>

#pragma mark Public enums, structs

typedef enum {
    // The functions still can print to UART on their own while returning void
    METAL_PROMPT_COMMAND_TYPE_RET_VOID_ARG_VOID,
    METAL_PROMPT_COMMAND_TYPE_RET_VOID_ARG_CHAR,
    METAL_PROMPT_COMMAND_TYPE_RET_VOID_ARG_UINT32,
    METAL_PROMPT_COMMAND_TYPE_RET_VOID_ARG_UINT64,

    // Set the TEST_INTERFACE_RETURN_AND_ARGUMENT_STRING_BUFFER_SIZE to return
    // buffer capacity so the return content will fit into it
    METAL_PROMPT_COMMAND_TYPE_RET_CHAR_ARG_VOID,
    METAL_PROMPT_COMMAND_TYPE_RET_CHAR_ARG_CHAR,
    METAL_PROMPT_COMMAND_TYPE_RET_CHAR_ARG_UINT32,
    METAL_PROMPT_COMMAND_TYPE_RET_CHAR_ARG_UINT64,

    METAL_PROMPT_COMMAND_TYPE_RET_UINT32_ARG_VOID,
    METAL_PROMPT_COMMAND_TYPE_RET_UINT32_ARG_CHAR,
    METAL_PROMPT_COMMAND_TYPE_RET_UINT32_ARG_UINT32,
    METAL_PROMPT_COMMAND_TYPE_RET_UINT32_ARG_UINT64,

    METAL_PROMPT_COMMAND_TYPE_RET_UINT64_ARG_VOID,
    METAL_PROMPT_COMMAND_TYPE_RET_UINT64_ARG_CHAR,
    METAL_PROMPT_COMMAND_TYPE_RET_UINT64_ARG_UINT32,
    METAL_PROMPT_COMMAND_TYPE_RET_UINT64_ARG_UINT64,
} metal_prompt_command_types;


typedef struct {
	char* (*action)(int64_t);
} metal_prompt_command_char_int64;



typedef struct {
	char* (*action)(void);
} metal_prompt_command_char_no_args;


typedef struct {
	uint64_t (*action)(void);
} metal_prompt_command_uint64_no_args;


typedef struct {
	int64_t (*action)(int64_t);
} metal_prompt_command_int64_int64;


typedef struct {
	void (*action)(void);
} metal_prompt_command_void_no_args;


typedef struct {
	char* command;
	metal_prompt_command_types type;
	union {
		metal_prompt_command_char_int64     char_int64;
		metal_prompt_command_char_no_args   char_no_args;
		metal_prompt_command_uint64_no_args uint64_no_args;
		metal_prompt_command_void_no_args   void_no_args;
	};
} metal_prompt_command;


typedef struct {
	char* group_name;
	uint32_t testsSize;
	metal_prompt_command* tests;
} metal_prompt_group;


#endif /* METAL_PROMPT_STRUCTURES_H_ */
