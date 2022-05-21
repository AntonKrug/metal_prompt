/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#include "systick.h"
#include "config.h"

#include <stdlib.h>


#ifdef M_P_UPTIME
volatile uint32_t m_p_systick_uptime_ticks = 0;


// Call this systick handler from application's systick handler,
// preferably at 1000Hz frequency (to get 1ms timestamp accuracy)
M_P_FORCE_OPTIMIZATION
void m_p_systick_handler() {
    m_p_systick_uptime_ticks++;
}

#endif
