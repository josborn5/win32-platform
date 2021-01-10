#include <iostream>
#include <stdint.h>
#include "math.tests.cpp"
#include "software_rendering.tests.cpp"
#include "geometry.tests.cpp"

int main()
{
	run_Vec4_tests();
	std::cout << "Vec4 tests passed.\n";

	RunSoftwareRenderingTests();
	std::cout << "software_rendering tests passed.\n";

	RunGeometryTests();
	std::cout << "geometry tests passed.\n";
}