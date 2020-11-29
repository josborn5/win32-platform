#include "geometry.cpp"

void RunGeometryTests()
{
	// LookAt / PointAt test
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


	// IntersectPlane test
	math::Vec3<float> plane = { 0.0f, 1.0f, 0.0f, 1.0f };
	math::Vec3<float> planeNormal = { 0.0f, -1.0f, 0.0f, 1.0f };
	math::Vec3<float> lineStart = { 0.0f, 0.0f, 0.0f, 1.0f };
	math::Vec3<float> lineEnd = { 0.0f, 2.0f, 0.0f, 1.0f };

	math::Vec3<float> result = IntersectPlane(plane, planeNormal, lineStart, lineEnd);

	assert(result.x == 0.0f);
	assert(result.y == 1.0f);
	assert(result.z == 0.0f);

}