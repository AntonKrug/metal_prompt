/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */

#include "commands_enabled.h"

#include "commands_bundled.h"

metal_prompt_group metal_prompt_commands_enabled[] = {
#ifndef NDEBUG
		METAL_PROMPT_BUNDLED_GROUP
#endif
};
