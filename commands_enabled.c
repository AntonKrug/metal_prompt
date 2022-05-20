/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */

#include "commands_enabled.h"

#include "commands_bundled.h"
#include "drivers/mss/mss_gpio_test_generic/mss_gpio_test_generic.h"

#include <stdlib.h>

const m_p_group m_p_commands_enabled[] = {
        { "",               m_p_bundled_list },
        { "test_namespace", m_p_bundled_list },

        // Add additional commands here to be accessible in all configurations
        { "gpio",           m_p_gpio_list },

#ifndef NDEBUG
        // Add additional commands here to be accessible only from Debug configurations

#endif
        { NULL } // Terminator of the group
};
