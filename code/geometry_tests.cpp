#include "geometry.cpp"

void RunGeometryTests()
{
	// LookAt / PointAt test
	math::Vec4<float> objectPosition = { 0.0f, 0.0f, 1.0f, 1.0f };
	math::Vec4<float> cameraPosition = { 0.0f, 0.0f, 0.0f, 1.0f };
	math::Vec4<float> cameraDirection = { 1.0f, 0.0f, 0.0f, 1.0f };
	math::Vec4<float> up = { 0.0f, 1.0f, 0.0f, 1.0f };
	
	// Camera matrix
	math::Vec4<float> target = AddVectors(cameraPosition, cameraDirection);
	math::Matrix4x4<float> cameraMatrix = PointAt(cameraPosition, target, up);

	// View matrix
	math::Matrix4x4<float> viewMatrix = LookAt(cameraMatrix);

	math::Vec4<float> objectPositionInViewSpace;
	math::MultiplyVectorWithMatrix(objectPosition, objectPositionInViewSpace, viewMatrix);

	assert(objectPositionInViewSpace.z == 0.0f);
	assert(objectPositionInViewSpace.y == 0.0f);
	assert(objectPositionInViewSpace.x == -1.0f);


	// IntersectPlane test
	math::Vec4<float> plane = { 0.0f, 1.0f, 0.0f, 1.0f };
	math::Vec4<float> planeNormal = { 0.0f, -1.0f, 0.0f, 1.0f };
	math::Vec4<float> lineStart = { 0.0f, 0.0f, 0.0f, 1.0f };
	math::Vec4<float> lineEnd = { 0.0f, 2.0f, 0.0f, 1.0f };

	math::Vec4<float> result = IntersectPlane(plane, planeNormal, lineStart, lineEnd);

	assert(result.x == 0.0f);
	assert(result.y == 1.0f);
	assert(result.z == 0.0f);

}