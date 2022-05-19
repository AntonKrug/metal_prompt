/*
 * Author:  Anton Krug
 * License: MIT
 * Created: 24/03/2018
 */

#include "commands_enabled.h"

#include "commands_bundled.h"

test_group tests_enabled[] = {
#ifndef NDEBUG
		TEST_BUNDLED_GROUP
#endif
};
