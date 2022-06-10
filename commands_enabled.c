/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */

/*
 * Selection which command groups (and under what namespace) are enabled in the
 * metal prompt
 */

#include "commands_enabled.h"

#include "commands_bundled.h"
// M_P_ADD_CUSTOM_COMMAND_INCLUDES_BEGIN
#include "drivers/mss/mss_gpio_test_generic/mss_gpio_test_generic.h"
// M_P_ADD_CUSTOM_COMMAND_INCLUDES_END

#include <stdlib.h>

const m_p_group m_p_commands_enabled[] = {
        { "",               m_p_bundled_list },
        { "test_namespace", m_p_bundled_list },

        // M_P_ADD_CUSTOM_COMMAND_LIST_FOR_ALL_CONFIGURATIONS_BEGIN
        { "gpio",           m_p_gpio_list },
        // M_P_ADD_CUSTOM_COMMAND_LIST_FOR_ALL_CONFIGURATIONS_END

#ifdef NDEBUG
        // Add additional commands here to be accessible only from Debug configurations
        // M_P_ADD_CUSTOM_COMMAND_LIST_FOR_DEBUG_CONFIGURATIONS_BEGIN

        // M_P_ADD_CUSTOM_COMMAND_LIST_FOR_DEBUG_CONFIGURATIONS_END
#endif
        { NULL } // Terminator of the group
};
