/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#ifndef METAL_PROMPT_STRUCTURES_H_
#define METAL_PROMPT_STRUCTURES_H_

#include "config.h"

#include <stdint.h>

#pragma mark Public enums, structs

typedef enum {
    // The functions still can print to UART on their own while returning void
    METAL_PROMPT_COMMAND_TYPE_RET_VOID_ARG_VOID,
#ifdef METAL_PROMPT_RETURN_AND_ARGUMENT_STRING_ENABLE
    METAL_PROMPT_COMMAND_TYPE_RET_VOID_ARG_CHARS,
#endif
    METAL_PROMPT_COMMAND_TYPE_RET_VOID_ARG_UINT32,
    METAL_PROMPT_COMMAND_TYPE_RET_VOID_ARG_UINT64,

#ifdef METAL_PROMPT_RETURN_AND_ARGUMENT_STRING_ENABLE
    // Set the METAL_PROMPT_RETURN_AND_ARGUMENT_STRING_BUFFER_SIZE to
    // configure the return (and argument input) buffer capacity so content
    // will fit into it
    METAL_PROMPT_COMMAND_TYPE_RET_CHARS_ARG_VOID,
    METAL_PROMPT_COMMAND_TYPE_RET_CHARS_ARG_CHARS,
    METAL_PROMPT_COMMAND_TYPE_RET_CHARS_ARG_UINT32,
    METAL_PROMPT_COMMAND_TYPE_RET_CHARS_ARG_UINT64,
#endif

    METAL_PROMPT_COMMAND_TYPE_RET_UINT32_ARG_VOID,
#ifdef METAL_PROMPT_RETURN_AND_ARGUMENT_STRING_ENABLE
    METAL_PROMPT_COMMAND_TYPE_RET_UINT32_ARG_CHARS,
#endif
    METAL_PROMPT_COMMAND_TYPE_RET_UINT32_ARG_UINT32,
    METAL_PROMPT_COMMAND_TYPE_RET_UINT32_ARG_UINT64,

    METAL_PROMPT_COMMAND_TYPE_RET_UINT64_ARG_VOID,
#ifdef METAL_PROMPT_RETURN_AND_ARGUMENT_STRING_ENABLE
    METAL_PROMPT_COMMAND_TYPE_RET_UINT64_ARG_CHARS,
#endif
    METAL_PROMPT_COMMAND_TYPE_RET_UINT64_ARG_UINT32,
    METAL_PROMPT_COMMAND_TYPE_RET_UINT64_ARG_UINT64,
} metal_prompt_command_types;

#pragma mark Structs with void return type
typedef struct {
    void (*action)(void);
} metal_prompt_command_void_void;


typedef struct {
    void (*action)(char*);
} metal_prompt_command_void_chars;


typedef struct {
    void (*action)(uint32_t);
} metal_prompt_command_void_uint32;


typedef struct {
    void (*action)(uint64_t);
} metal_prompt_command_void_uint64;


#pragma mark Structs with char* return type
typedef struct {
	char* (*action)(void);
} metal_prompt_command_chars_void;


typedef struct {
    char* (*action)(char*);
} metal_prompt_command_chars_chars;


typedef struct {
    char* (*action)(uint32_t);
} metal_prompt_command_chars_uint32;


typedef struct {
    char* (*action)(uint64_t);
} metal_prompt_command_chars_uint64;


#pragma mark Structs with uint32_t return type
typedef struct {
    uint32_t (*action)(void);
} metal_prompt_command_uint32_void;


typedef struct {
    uint32_t (*action)(char*);
} metal_prompt_command_uint32_chars;


typedef struct {
    uint32_t (*action)(uint32_t);
} metal_prompt_command_uint32_uint32;


typedef struct {
    uint32_t (*action)(uint64_t);
} metal_prompt_command_uint32_uint64;


#pragma mark Structs with uint64_t return type
typedef struct {
    uint64_t (*action)(void);
} metal_prompt_command_uint64_void;


typedef struct {
    uint64_t (*action)(char*);
} metal_prompt_command_uint64_chars;


typedef struct {
    uint64_t (*action)(uint32_t);
} metal_prompt_command_uint64_uint32;


typedef struct {
    uint64_t (*action)(uint64_t);
} metal_prompt_command_uint64_uint64;




typedef struct {
	char* command;
	metal_prompt_command_types type;
	union {
        metal_prompt_command_void_void     void_void;
        metal_prompt_command_void_chars    void_chars;
        metal_prompt_command_void_uint32   void_uint32;
        metal_prompt_command_void_uint64   void_uint64;

        metal_prompt_command_chars_void    chars_void;
        metal_prompt_command_chars_chars   chars_chars;
        metal_prompt_command_chars_uint32  chars_uint32;
		metal_prompt_command_chars_uint64  chars_uint64;

        metal_prompt_command_uint32_void   uint32_void;
        metal_prompt_command_uint32_chars  uint32_chars;
        metal_prompt_command_uint32_uint32 uint32_uint32;
        metal_prompt_command_uint32_uint64 uint32_uint64;

		metal_prompt_command_uint64_void   uint64_void;
        metal_prompt_command_uint64_chars  uint64_chars;
        metal_prompt_command_uint64_uint32 uint64_uint32;
        metal_prompt_command_uint64_uint64 uint64_uint64;
	};
} metal_prompt_command;


typedef struct {
	char*                 group_name;
	uint32_t              testsSize;
	metal_prompt_command* tests;
} metal_prompt_group;


#endif /* METAL_PROMPT_STRUCTURES_H_ */
