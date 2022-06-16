/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */

/*
 * Selection which command groups (and under what namespace) are enabled in the
 * metal prompt. This can be done by hand, by defining the defines or with
 * a script which can detect the KEYWORDS in the comments
 */


#include "commands_enabled.h"

#include "commands_bundled.h"
// ------ M_P_ADD_CUSTOM_COMMAND_INCLUDES_BEGIN -----
// Your script can edit this file and add here the includes,
// or not editing this file at all, but providing
// the M_P_ADD_CUSTOM_COMMAND_INCLUDE define.
#ifdef M_P_ADD_CUSTOM_COMMAND_INCLUDE
#define M_P_ADD_CUSTOM_COMMAND_ESCAPE(filename) <filename>
#include M_P_ADD_CUSTOM_COMMAND_ESCAPE(M_P_ADD_CUSTOM_COMMAND_INCLUDE)
#endif
// ----- M_P_ADD_CUSTOM_COMMAND_INCLUDES_END -----


#include <stdlib.h>

const m_p_group m_p_commands_enabled[] = {
        { "",               m_p_bundled_list },


        // ----- M_P_ADD_CUSTOM_COMMAND_LIST_FOR_ALL_CONFIGURATIONS_BEGIN -----
        // Your script can bind your commands to a name, by editing this file.
        // For example: { "gpio", m_p_gpio_list },
        // Or provide the corresponding defines to get it populated for you
#if defined(M_P_ADD_CUSTOM_COMMAND_LIST_FOR_ALL_CONFIGURATIONS_NAME) && defined(M_P_ADD_CUSTOM_COMMAND_LIST_FOR_ALL_CONFIGURATIONS_LIST)
        { M_P_ADD_CUSTOM_COMMAND_LIST_FOR_ALL_CONFIGURATIONS_NAME,  M_P_ADD_CUSTOM_COMMAND_LIST_FOR_ALL_CONFIGURATIONS_LIST},
#endif
        // ----- M_P_ADD_CUSTOM_COMMAND_LIST_FOR_ALL_CONFIGURATIONS_END -----


#ifdef NDEBUG
        // ----- M_P_ADD_CUSTOM_COMMAND_LIST_FOR_DEBUG_CONFIGURATIONS_BEGIN -----
        // Add additional commands here to be accessible only from Debug
        // configurations (but your build system needs to define the NDEBUG
        // accordingly)
#if defined(M_P_ADD_CUSTOM_COMMAND_LIST_FOR_DEBUG_CONFIGURATIONS_NAME) && defined(M_P_ADD_CUSTOM_COMMAND_LIST_FOR_DEBUG_CONFIGURATIONS_LIST)
        { M_P_ADD_CUSTOM_COMMAND_LIST_FOR_DEBUG_CONFIGURATIONS_NAME,  M_P_ADD_CUSTOM_COMMAND_LIST_FOR_DEBUG_CONFIGURATIONS_LIST},
#endif
        // ----- M_P_ADD_CUSTOM_COMMAND_LIST_FOR_DEBUG_CONFIGURATIONS_END -----


#endif // NDEBUG
        { NULL } // Terminator of the lists
};
