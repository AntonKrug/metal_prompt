/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#ifndef METAL_PROMPT_SYSTICK_H_
#define METAL_PROMPT_SYSTICK_H_

#include "config.h"

#include <stdint.h>

#ifdef M_P_UPTIME
extern volatile uint32_t m_p_systick_uptime_ticks;
extern void              m_p_systick_handler(void);
#endif


#endif /* METAL_PROMPT_SYSTICK_H_ */
