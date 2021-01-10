#include "math.cpp"
#include <assert.h>

void run_Vec4_tests()
{
	// dot_product
	float dot = gentle::DotProduct(gentle::Vec4<float>{ 1.0f, 2.0f, 3.0f }, gentle::Vec4<float>{ 4.0f, 5.0f, 6.0f });
	assert(dot == (float)32);
}
