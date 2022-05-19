/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */


#include "systick.h"

#include <stdlib.h>


#ifdef METAL_PROMPT_UPTIME
volatile uint64_t metal_prompt_systick_uptime_ticks = 0;
void (*test_systick_action)(void) = NULL;
#endif


//void SysTick_Handler(void) {
//  test_systick_uptime_ticks++;
//  if (test_systick_action != NULL) {
//      test_systick_action();
//  }
//}
//
