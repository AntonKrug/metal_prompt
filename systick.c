/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#include "systick.h"

#include <stdlib.h>


#ifdef METAL_PROMPT_UPTIME
volatile uint32_t metal_prompt_systick_uptime_ticks = 0;


// Call this systick handler from application's systick handler
void metal_prompt_systick_handler() {
    metal_prompt_systick_uptime_ticks++;
}

#endif
