/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#ifndef METAL_PROMPT_STRUCTURES_H_
#define METAL_PROMPT_STRUCTURES_H_

#include "config.h"

#include <stdint.h>


#pragma mark - Structs with void return type


typedef struct {
    void (*action)(void);
} m_p_command_void_void;


typedef struct {
    void (*action)(char*);
} m_p_command_void_chars;


typedef struct {
    void (*action)(uint32_t);
} m_p_command_void_uint32;


typedef struct {
    void (*action)(uint64_t);
} m_p_command_void_uint64;


#pragma mark - Structs with char* return type


typedef struct {
	char* (*action)(void);
} m_p_command_chars_void;


typedef struct {
    char* (*action)(char*);
} m_p_command_chars_chars;


typedef struct {
    char* (*action)(uint32_t);
} m_p_command_chars_uint32;


typedef struct {
    char* (*action)(uint64_t);
} m_p_command_chars_uint64;


#pragma mark - Structs with uint32_t return type


typedef struct {
    uint32_t (*action)(void);
} m_p_command_uint32_void;


typedef struct {
    uint32_t (*action)(char*);
} m_p_command_uint32_chars;


typedef struct {
    uint32_t (*action)(uint32_t);
} m_p_command_uint32_uint32;


typedef struct {
    uint32_t (*action)(uint64_t);
} m_p_command_uint32_uint64;


#pragma mark - Structs with uint64_t return type


typedef struct {
    uint64_t (*action)(void);
} m_p_command_uint64_void;


typedef struct {
    uint64_t (*action)(char*);
} m_p_command_uint64_chars;


typedef struct {
    uint64_t (*action)(uint32_t);
} m_p_command_uint64_uint32;


typedef struct {
    uint64_t (*action)(uint64_t);
} m_p_command_uint64_uint64;


#pragma mark - Command enum and structs


typedef enum {
    // The functions still can print to UART on their own while returning void
    M_P_COMMAND_TYPE_RET_VOID_ARG_VOID,
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
    M_P_COMMAND_TYPE_RET_VOID_ARG_CHARS,
#endif
    M_P_COMMAND_TYPE_RET_VOID_ARG_UINT32,
    M_P_COMMAND_TYPE_RET_VOID_ARG_UINT64,

#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
    // Set the METAL_PROMPT_RETURN_AND_ARGUMENT_STRING_BUFFER_SIZE to
    // configure the return (and argument input) buffer capacity so content
    // will fit into it
    M_P_COMMAND_TYPE_RET_CHARS_ARG_VOID,
    M_P_COMMAND_TYPE_RET_CHARS_ARG_CHARS,
    M_P_COMMAND_TYPE_RET_CHARS_ARG_UINT32,
    M_P_COMMAND_TYPE_RET_CHARS_ARG_UINT64,
#endif

    M_P_COMMAND_TYPE_RET_UINT32_ARG_VOID,
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
    M_P_COMMAND_TYPE_RET_UINT32_ARG_CHARS,
#endif
    M_P_COMMAND_TYPE_RET_UINT32_ARG_UINT32,
    M_P_COMMAND_TYPE_RET_UINT32_ARG_UINT64,

    M_P_COMMAND_TYPE_RET_UINT64_ARG_VOID,
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
    M_P_COMMAND_TYPE_RET_UINT64_ARG_CHARS,
#endif
    M_P_COMMAND_TYPE_RET_UINT64_ARG_UINT32,
    M_P_COMMAND_TYPE_RET_UINT64_ARG_UINT64,
} m_p_command_types;


typedef struct {
	const char* command;
	const m_p_command_types type;
	const union {
        m_p_command_void_void     void_void;
        m_p_command_void_chars    void_chars;
        m_p_command_void_uint32   void_uint32;
        m_p_command_void_uint64   void_uint64;

        m_p_command_chars_void    chars_void;
        m_p_command_chars_chars   chars_chars;
        m_p_command_chars_uint32  chars_uint32;
		m_p_command_chars_uint64  chars_uint64;

        m_p_command_uint32_void   uint32_void;
        m_p_command_uint32_chars  uint32_chars;
        m_p_command_uint32_uint32 uint32_uint32;
        m_p_command_uint32_uint64 uint32_uint64;

		m_p_command_uint64_void   uint64_void;
        m_p_command_uint64_chars  uint64_chars;
        m_p_command_uint64_uint32 uint64_uint32;
        m_p_command_uint64_uint64 uint64_uint64;
	};
} m_p_command;


typedef struct {
	const char*        namespace;
	const m_p_command* commands;
} m_p_group;


#endif /* METAL_PROMPT_STRUCTURES_H_ */
