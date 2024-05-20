#include <iostream>

#include "config.h"

#include "tests.h"

int main() {
#if RUN_VISUAL_TESTS
	printf("VISUAL!");
#elif RUN_AUTOMATED_TESTS
	printf("AUTOMATED!");
#endif
}