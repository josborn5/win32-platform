#include <iostream>
#include <stdint.h>
#include "math.tests.cpp"
#include "software_rendering.tests.cpp"

int main()
{
	run_vec3_tests();
	std::cout << "vec3 tests passed.\n";

	RunSoftwareRenderingTests();
	std::cout << "software_rendering tests passed.\n";
}