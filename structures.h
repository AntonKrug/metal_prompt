/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */

/*
 * Structures and enums needed to construct the command's metadata/description
 */


#ifndef METAL_PROMPT_STRUCTURES_H_
#define METAL_PROMPT_STRUCTURES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"

#include <stdint.h>
#include <stddef.h> // To the NULL terminator for files who include this structure


#pragma mark - Callback structs with void return type


typedef struct {
    void (*action)(void);
} m_p_command_void_void;


#if defined(M_P_CFG_TYPE_CHARS)
typedef struct {
    void (*action)(const char*);
} m_p_command_void_chars;
#endif


#if defined(M_P_CFG_TYPE_UINT)
typedef struct {
    void (*action)(const unsigned int);
} m_p_command_void_uint;
#endif


#if defined(M_P_CFG_TYPE_UINT32)
typedef struct {
    void (*action)(uint32_t);
} m_p_command_void_uint32;
#endif


#if defined(M_P_CFG_TYPE_UINT64)
typedef struct {
    void (*action)(uint64_t);
} m_p_command_void_uint64;
#endif


#pragma mark - Callback structs with char* return type


#if defined(M_P_CFG_TYPE_CHARS)
typedef struct {
	char* (*action)(void);
} m_p_command_chars_void;
#endif


#if defined(M_P_CFG_TYPE_CHARS)
typedef struct {
    char* (*action)(const char*);
} m_p_command_chars_chars;
#endif


#if defined(M_P_CFG_TYPE_CHARS) && defined(M_P_CFG_TYPE_UINT)
typedef struct {
    char* (*action)(const unsigned int);
} m_p_command_chars_uint;
#endif


#if defined(M_P_CFG_TYPE_CHARS) && defined(M_P_CFG_TYPE_UINT32)
typedef struct {
    char* (*action)(uint32_t);
} m_p_command_chars_uint32;
#endif


#if defined(M_P_CFG_TYPE_CHARS) && defined(M_P_CFG_TYPE_UINT64)
typedef struct {
    char* (*action)(uint64_t);
} m_p_command_chars_uint64;
#endif


#pragma mark - Callback structs with unsigned int return type


#if defined(M_P_CFG_TYPE_UINT)
typedef struct {
    unsigned int (*action)(void);
} m_p_command_uint_void;
#endif


#if defined(M_P_CFG_TYPE_UINT) && defined(M_P_CFG_TYPE_CHARS)
typedef struct {
    unsigned int (*action)(const char*);
} m_p_command_uint_chars;
#endif


#if defined(M_P_CFG_TYPE_UINT) && defined(M_P_CFG_TYPE_UINT)
typedef struct {
    unsigned int (*action)(const unsigned int);
} m_p_command_uint_uint;
#endif


#if defined(M_P_CFG_TYPE_UINT) && defined(M_P_CFG_TYPE_UINT32)
typedef struct {
    unsigned int (*action)(uint32_t);
} m_p_command_uint_uint32;
#endif


#if defined(M_P_CFG_TYPE_UINT) && defined(M_P_CFG_TYPE_UINT64)
typedef struct {
    unsigned int (*action)(uint64_t);
} m_p_command_uint_uint64;
#endif


#pragma mark - Callback structs with uint32_t return type


#if defined(M_P_CFG_TYPE_UINT32)
typedef struct {
    uint32_t (*action)(void);
} m_p_command_uint32_void;
#endif


#if defined(M_P_CFG_TYPE_UINT32) && defined(M_P_CFG_TYPE_CHARS)
typedef struct {
    uint32_t (*action)(const char*);
} m_p_command_uint32_chars;
#endif


#if defined(M_P_CFG_TYPE_UINT32) && defined(M_P_CFG_TYPE_UINT)
typedef struct {
    uint32_t (*action)(const unsigned int);
} m_p_command_uint32_uint;
#endif


#if defined(M_P_CFG_TYPE_UINT32) && defined(M_P_CFG_TYPE_UINT32)
typedef struct {
    uint32_t (*action)(uint32_t);
} m_p_command_uint32_uint32;
#endif


#if defined(M_P_CFG_TYPE_UINT32) && defined(M_P_CFG_TYPE_UINT64)
typedef struct {
    uint32_t (*action)(uint64_t);
} m_p_command_uint32_uint64;
#endif


#pragma mark - Callback structs with uint64_t return type


#if defined(M_P_CFG_TYPE_UINT64)
typedef struct {
    uint64_t (*action)(void);
} m_p_command_uint64_void;
#endif


#if defined(M_P_CFG_TYPE_UINT64) && defined(M_P_CFG_TYPE_CHARS)
typedef struct {
    uint64_t (*action)(const char*);
} m_p_command_uint64_chars;
#endif


#if defined(M_P_CFG_TYPE_UINT64) && defined(M_P_CFG_TYPE_UINT)
typedef struct {
    uint64_t (*action)(const unsigned int);
} m_p_command_uint64_uint;
#endif


#if defined(M_P_CFG_TYPE_UINT64) && defined(M_P_CFG_TYPE_UINT32)
typedef struct {
    uint64_t (*action)(uint32_t);
} m_p_command_uint64_uint32;
#endif


#if defined(M_P_CFG_TYPE_UINT64) && defined(M_P_CFG_TYPE_UINT64)
typedef struct {
    uint64_t (*action)(uint64_t);
} m_p_command_uint64_uint64;
#endif


#pragma mark - Command enum and structs


// Callbacks can output to the transport while returning any type
// (including void). When updating this enum, make sure the interate.c
//  m_p_command_type_string is updated accordingly and verify that the
// config.h  M_P_CFG_AUTOCOMPLETE_TYPE_MAX_STRING is big enough.
typedef enum {
    M_P_TYPE_VOID,
#ifdef M_P_CFG_TYPE_CHARS
    M_P_TYPE_CHARS,
#endif
#ifdef M_P_CFG_TYPE_UINT
    M_P_TYPE_UINT,
#endif
#ifdef M_P_CFG_TYPE_UINT32
    M_P_TYPE_UINT32,
#endif
#ifdef M_P_CFG_TYPE_UINT64
    M_P_TYPE_UINT64,
#endif

    M_P_TYPE_LAST
} m_p_command_type;


#define M_P_CMD_MASK_RET                          ( (1u << M_P_TYPE_LAST)-1u )
#define M_P_CMD_MASK_ARG                          ( ~((1u << M_P_TYPE_LAST)-1u) & ((1u << (2*M_P_TYPE_LAST))-1u) )

#define M_P_CMD_GET_RET_TYPE(return_type)         ( 1u << (return_type))
#define M_P_CMD_GET_ARG_TYPE(argument_type)       ( 1u << (M_P_TYPE_LAST+(argument_type)) )
#define M_P_CMD_TYPES(return_type, argument_type) ( M_P_CMD_GET_RET_TYPE(return_type) | M_P_CMD_GET_ARG_TYPE(argument_type) )


typedef struct {
	const char* command;
	const unsigned int type;  // example M_P_CMD_TYPES(M_P_TYPE_VOID, M_P_TYPE_VOID)
	const union {
        m_p_command_void_void     void_void;
#if defined(M_P_CFG_TYPE_CHARS)
        m_p_command_void_chars    void_chars;
#endif
#if defined(M_P_CFG_TYPE_UINT)
        m_p_command_void_uint     void_uint;
#endif
#if defined(M_P_CFG_TYPE_UINT32)
        m_p_command_void_uint32   void_uint32;
#endif
#if defined(M_P_CFG_TYPE_UINT64)
        m_p_command_void_uint64   void_uint64;
#endif


#if defined(M_P_CFG_TYPE_CHARS)
        m_p_command_chars_void    chars_void;
#endif
#if defined(M_P_CFG_TYPE_CHARS) && defined(M_P_CFG_TYPE_CHARS)
        m_p_command_chars_chars   chars_chars;
#endif
#if defined(M_P_CFG_TYPE_CHARS) && defined(M_P_CFG_TYPE_UINT)
        m_p_command_chars_uint    chars_uint;
#endif
#if defined(M_P_CFG_TYPE_CHARS) && defined(M_P_CFG_TYPE_UINT32)
        m_p_command_chars_uint32  chars_uint32;
#endif
#if defined(M_P_CFG_TYPE_CHARS) && defined(M_P_CFG_TYPE_UINT64)
		m_p_command_chars_uint64  chars_uint64;
#endif


#if defined(M_P_CFG_TYPE_UINT)
        m_p_command_uint_void   uint_void;
#endif
#if defined(M_P_CFG_TYPE_UINT) && defined(M_P_CFG_TYPE_CHARS)
        m_p_command_uint_chars  uint_chars;
#endif
#if defined(M_P_CFG_TYPE_UINT) && defined(M_P_CFG_TYPE_UINT)
        m_p_command_uint_uint   uint_uint;
#endif
#if defined(M_P_CFG_TYPE_UINT) && defined(M_P_CFG_TYPE_UINT32)
        m_p_command_uint_uint32 uint_uint32;
#endif
#if defined(M_P_CFG_TYPE_UINT) && defined(M_P_CFG_TYPE_UINT64)
        m_p_command_uint_uint64 uint_uint64;
#endif


#if defined(M_P_CFG_TYPE_UINT32)
        m_p_command_uint32_void   uint32_void;
#endif
#if defined(M_P_CFG_TYPE_UINT32) && defined(M_P_CFG_TYPE_CHARS)
        m_p_command_uint32_chars  uint32_chars;
#endif
#if defined(M_P_CFG_TYPE_UINT32) && defined(M_P_CFG_TYPE_UINT)
        m_p_command_uint32_uint   uint32_uint;
#endif
#if defined(M_P_CFG_TYPE_UINT32) && defined(M_P_CFG_TYPE_UINT32)
        m_p_command_uint32_uint32 uint32_uint32;
#endif
#if defined(M_P_CFG_TYPE_UINT32) && defined(M_P_CFG_TYPE_UINT64)
        m_p_command_uint32_uint64 uint32_uint64;
#endif


#if defined(M_P_CFG_TYPE_UINT64)
		m_p_command_uint64_void   uint64_void;
#endif
#if defined(M_P_CFG_TYPE_UINT64) && defined(M_P_CFG_TYPE_CHARS)
        m_p_command_uint64_chars  uint64_chars;
#endif
#if defined(M_P_CFG_TYPE_UINT64) && defined(M_P_CFG_TYPE_UINT)
        m_p_command_uint64_uint   uint64_uint;
#endif
#if defined(M_P_CFG_TYPE_UINT64) && defined(M_P_CFG_TYPE_UINT32)
        m_p_command_uint64_uint32 uint64_uint32;
#endif
#if defined(M_P_CFG_TYPE_UINT64) && defined(M_P_CFG_TYPE_UINT64)
        m_p_command_uint64_uint64 uint64_uint64;
#endif


	};
} m_p_command;


typedef struct {
	const char*        namespace;
	const m_p_command* commands;
} m_p_group;


#ifdef __cplusplus
}
#endif

#endif /* METAL_PROMPT_STRUCTURES_H_ */

