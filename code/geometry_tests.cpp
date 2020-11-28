#include "geometry.cpp"

void RunGeometryTests()
{
	math::Vec3<float> objectPosition = { 0.0f, 0.0f, 1.0f, 1.0f };
	math::Vec3<float> cameraPosition = { 0.0f, 0.0f, 0.0f, 1.0f };
	math::Vec3<float> cameraDirection = { 1.0f, 0.0f, 0.0f, 1.0f };
	math::Vec3<float> up = { 0.0f, 1.0f, 0.0f, 1.0f };
	
	// Camera matrix
	math::Vec3<float> target = AddVectors(cameraPosition, cameraDirection);
	math::Matrix4x4<float> cameraMatrix = PointAt(cameraPosition, target, up);

	// View matrix
	math::Matrix4x4<float> viewMatrix = LookAt(cameraMatrix);

	math::Vec3<float> objectPositionInViewSpace;
	math::MultiplyVectorWithMatrix(objectPosition, objectPositionInViewSpace, viewMatrix);

	assert(objectPositionInViewSpace.z == 0.0f);
	assert(objectPositionInViewSpace.y == 0.0f);
	assert(objectPositionInViewSpace.x == -1.0f);
}