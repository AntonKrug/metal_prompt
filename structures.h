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


#pragma mark - Callback structs with void return type


typedef struct {
    void (*action)(void);
} m_p_command_void_void;


#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
typedef struct {
    void (*action)(const char*);
} m_p_command_void_chars;
#endif


#ifdef M_P_RETURN_AND_ARGUMENT_UINT_ENABLE
typedef struct {
    void (*action)(const unsigned int);
} m_p_command_void_uint;
#endif


#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
typedef struct {
    void (*action)(uint32_t);
} m_p_command_void_uint32;
#endif


#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
typedef struct {
    void (*action)(uint64_t);
} m_p_command_void_uint64;
#endif


#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE

#pragma mark - Callback structs with char* return type

typedef struct {
	char* (*action)(void);
} m_p_command_chars_void;


typedef struct {
    char* (*action)(const char*);
} m_p_command_chars_chars;


#ifdef M_P_RETURN_AND_ARGUMENT_UINT_ENABLE
typedef struct {
    char* (*action)(const unsigned int);
} m_p_command_chars_uint;
#endif


#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
typedef struct {
    char* (*action)(uint32_t);
} m_p_command_chars_uint32;
#endif


#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
typedef struct {
    char* (*action)(uint64_t);
} m_p_command_chars_uint64;
#endif

#endif // M_P_RETURN_AND_ARGUMENT_STRING_ENABLE


#ifdef M_P_RETURN_AND_ARGUMENT_UINT_ENABLE

#pragma mark - Callback structs with unsigned int return type


typedef struct {
    unsigned int (*action)(void);
} m_p_command_uint_void;


#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
typedef struct {
    unsigned int (*action)(const char*);
} m_p_command_uint_chars;
#endif


typedef struct {
    unsigned int (*action)(const unsigned int);
} m_p_command_uint_uint;


#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
typedef struct {
    unsigned int (*action)(uint32_t);
} m_p_command_uint_uint32;
#endif


#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
typedef struct {
    unsigned int (*action)(uint64_t);
} m_p_command_uint_uint64;
#endif

#endif // M_P_RETURN_AND_ARGUMENT_UINT_ENABLE


#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE

#pragma mark - Callback structs with uint32_t return type


typedef struct {
    uint32_t (*action)(void);
} m_p_command_uint32_void;


#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
typedef struct {
    uint32_t (*action)(const char*);
} m_p_command_uint32_chars;
#endif


#ifdef M_P_RETURN_AND_ARGUMENT_UINT_ENABLE
typedef struct {
    uint32_t (*action)(const unsigned int);
} m_p_command_uint32_uint;
#endif


typedef struct {
    uint32_t (*action)(uint32_t);
} m_p_command_uint32_uint32;


#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
typedef struct {
    uint32_t (*action)(uint64_t);
} m_p_command_uint32_uint64;
#endif

#endif // M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE



#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE

#pragma mark - Callback structs with uint64_t return type


typedef struct {
    uint64_t (*action)(void);
} m_p_command_uint64_void;


#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
typedef struct {
    uint64_t (*action)(const char*);
} m_p_command_uint64_chars;
#endif


#ifdef M_P_RETURN_AND_ARGUMENT_UINT_ENABLE
typedef struct {
    uint64_t (*action)(const unsigned int);
} m_p_command_uint64_uint;
#endif


#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
typedef struct {
    uint64_t (*action)(uint32_t);
} m_p_command_uint64_uint32;
#endif


typedef struct {
    uint64_t (*action)(uint64_t);
} m_p_command_uint64_uint64;

#endif // M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE


#pragma mark - Command enum and structs


typedef enum {

    // The functions still can print to UART on their own while returning void
    M_P_CMD_RET_VOID_ARG_VOID,
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
    M_P_CMD_RET_VOID_ARG_CHARS,
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT_ENABLE
    M_P_CMD_RET_VOID_ARG_UINT,
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
    M_P_CMD_RET_VOID_ARG_UINT32,
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
    M_P_CMD_RET_VOID_ARG_UINT64,
#endif


#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
    // Set the METAL_PROMPT_RETURN_AND_ARGUMENT_STRING_BUFFER_SIZE to
    // configure the return (and argument input) buffer capacity so content
    // will fit into it
    M_P_CMD_RET_CHARS_ARG_VOID,
    M_P_CMD_RET_CHARS_ARG_CHARS,
#ifdef M_P_RETURN_AND_ARGUMENT_UINT_ENABLE
    M_P_CMD_RET_CHARS_ARG_UINT,
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
    M_P_CMD_RET_CHARS_ARG_UINT32,
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
    M_P_CMD_RET_CHARS_ARG_UINT64,
#endif
#endif // M_P_RETURN_AND_ARGUMENT_STRING_ENABLE


#ifdef M_P_RETURN_AND_ARGUMENT_UINT_ENABLE
    M_P_CMD_RET_UINT_ARG_VOID,
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
    M_P_CMD_RET_UINT_ARG_CHARS,
#endif
    M_P_CMD_RET_UINT_ARG_UINT,
#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
    M_P_CMD_RET_UINT_ARG_UINT32,
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
    M_P_CMD_RET_UINT_ARG_UINT64,
#endif
#endif // M_P_RETURN_AND_ARGUMENT_UINT_ENABLE


#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
    M_P_CMD_RET_UINT32_ARG_VOID,
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
    M_P_CMD_RET_UINT32_ARG_CHARS,
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT_ENABLE
    M_P_CMD_RET_UINT32_ARG_UINT,
#endif
    M_P_CMD_RET_UINT32_ARG_UINT32,
#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
    M_P_CMD_RET_UINT32_ARG_UINT64,
#endif
#endif // M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE


#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
    M_P_CMD_RET_UINT64_ARG_VOID,
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
    M_P_CMD_RET_UINT64_ARG_CHARS,
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT_ENABLE
    M_P_CMD_RET_UINT64_ARG_UINT,
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
    M_P_CMD_RET_UINT64_ARG_UINT32,
#endif
    M_P_CMD_RET_UINT64_ARG_UINT64,
#endif // M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE

} m_p_command_types;


typedef struct {
	const char* command;
	const m_p_command_types type;
	const union {
        m_p_command_void_void     void_void;
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
        m_p_command_void_chars    void_chars;
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT_ENABLE
        m_p_command_void_uint     void_uint;
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
        m_p_command_void_uint32   void_uint32;
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
        m_p_command_void_uint64   void_uint64;
#endif


#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
        m_p_command_chars_void    chars_void;
        m_p_command_chars_chars   chars_chars;
#ifdef M_P_RETURN_AND_ARGUMENT_UINT_ENABLE
        m_p_command_chars_uint    chars_uint;
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
        m_p_command_chars_uint32  chars_uint32;
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
		m_p_command_chars_uint64  chars_uint64;
#endif
#endif // M_P_RETURN_AND_ARGUMENT_STRING_ENABLE


#ifdef M_P_RETURN_AND_ARGUMENT_UINT_ENABLE
        m_p_command_uint_void   uint_void;
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
        m_p_command_uint_chars  uint_chars;
#endif
        m_p_command_uint_uint   uint_uint;
#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
        m_p_command_uint_uint32 uint_uint32;
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
        m_p_command_uint_uint64 uint_uint64;
#endif
#endif // M_P_RETURN_AND_ARGUMENT_UINT_ENABLE


#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
        m_p_command_uint32_void   uint32_void;
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
        m_p_command_uint32_chars  uint32_chars;
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT_ENABLE
        m_p_command_uint32_uint   uint32_uint;
#endif
        m_p_command_uint32_uint32 uint32_uint32;
#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
        m_p_command_uint32_uint64 uint32_uint64;
#endif
#endif // M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE


#ifdef M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE
		m_p_command_uint64_void   uint64_void;
#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
        m_p_command_uint64_chars  uint64_chars;
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT_ENABLE
        m_p_command_uint64_uint   uint64_uint;
#endif
#ifdef M_P_RETURN_AND_ARGUMENT_UINT32_ENABLE
        m_p_command_uint64_uint32 uint64_uint32;
#endif
        m_p_command_uint64_uint64 uint64_uint64;
#endif // M_P_RETURN_AND_ARGUMENT_UINT64_ENABLE

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

