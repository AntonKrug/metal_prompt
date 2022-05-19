/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */

#include "commands_bundled.h"
#include "commands_enabled.h"

test_group tests_enabled[] = {
#ifndef NDEBUG
		TEST_BUNDLED_GROUP
#endif
};
