/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */

#include "commands_enabled.h"

#include "commands_bundled.h"

m_p_group m_p_commands_enabled[] = {
#ifndef NDEBUG
		M_P_BUNDLED_GROUP
#endif
};
