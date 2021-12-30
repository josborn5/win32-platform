#include <iostream>
#include <stdint.h>
#include "../math.tests.cpp"
#include "../software_rendering.tests.cpp"
#include "../geometry.tests.cpp"
#include "../collision.tests.cpp"

int main()
{
	std::cout << "Starting Vec4 tests.\n";
	run_Vec4_tests();
	std::cout << "Vec4 tests passed.\n";

 	std::cout << "Starting software_rendering tests.\n";
	RunSoftwareRenderingTests();
	std::cout << "software_rendering tests passed.\n";

	std::cout << "Starting geometry tests.\n";
	RunGeometryTests();
	std::cout << "geometry tests passed.\n";

	std::cout << "Starting collision tests.\n";
	gentle::RunCollisionTests();
	std::cout << "collision tests passed.\n";
}