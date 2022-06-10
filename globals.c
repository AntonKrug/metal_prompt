/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 20/05/2022
 */

/*
 * Set of variables which is meant to be accessible globally
 */

#include "globals.h"


#pragma mark - Public variables

char m_p_command_prompt[M_P_CFG_WHOLE_PROMPT_SIZE];

bool m_p_print_in_dec       = M_P_CFG_PRINT_DEC_DEFAULT;

#ifdef M_P_CFG_ALLOW_QUIT
bool m_p_keep_runnning      = true;
#endif

#ifdef M_P_CFG_UPTIME
bool m_p_benchmark_commands = false;
#endif

#ifdef M_P_CFG_TYPE_CHARS
char m_p_return_and_argument_buf[M_P_CFG_TYPE_CHARS_BUFFER_SIZE];
#endif
