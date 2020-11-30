#include "math.hpp"
#include <assert.h>

using namespace math;

void run_Vec4_tests()
{
	// dot_product
	float dot = DotProduct(Vec4<float>{ 1.0f, 2.0f, 3.0f }, Vec4<float>{ 4.0f, 5.0f, 6.0f });
	assert(dot == (float)32);
}
