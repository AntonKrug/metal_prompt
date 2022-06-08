/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */

/*
 * Metal prompt systick handler, which can be used to determine application's
 * uptime and to run benchmarks on all commands.
 */


#include "systick.h"
#include "config.h"

#include <stdlib.h>


#ifdef M_P_CFG_UPTIME
volatile uint32_t m_p_systick_uptime_ticks = 0;


// Call this systick handler from application's systick handler,
// preferably at 1000Hz frequency (to get 1ms timestamp accuracy)
M_P_CFG_FORCE_OPTIMIZATION
void m_p_systick_handler() {
    m_p_systick_uptime_ticks++;
}

#endif
