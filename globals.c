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

bool m_p_print_in_dec       = M_P_PRINT_DEC_ON_STARTUP;

#ifdef M_P_ALLOW_QUIT
bool m_p_keep_runnning      = true;
#endif

#ifdef M_P_UPTIME
bool m_p_benchmark_commands = false;
#endif

#ifdef M_P_RETURN_AND_ARGUMENT_STRING_ENABLE
char m_p_return_and_argument_buf[M_P_RETURN_AND_ARGUMENT_STRING_BUFFER_SIZE];
#endif
