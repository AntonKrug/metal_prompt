/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#ifndef METAL_PROMPT_SYSTICK_H_
#define METAL_PROMPT_SYSTICK_H_

#include "config.h"

#include <stdint.h>

#ifdef METAL_PROMPT_UPTIME
extern volatile uint64_t metal_prompt_systick_uptime_ticks;
extern void              (*test_systick_action)(void);
#endif


#endif /* METAL_PROMPT_SYSTICK_H_ */
