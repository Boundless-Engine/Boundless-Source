#include <iostream>

#include "config.h"

#include "tests.h"


int main() {
#if ENABLE_USER_CONTROLLED_TESTS
	return Boundless::TestFramework::RunUserTests(EventSystemTest::KeyEventsTest);
#else 
	return Boundless::TestFramework::RunAutomatedTests();
#endif
}