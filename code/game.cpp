#include <math.h>
#include <list>
#include "win32_platform.cpp"
#include "software_rendering.cpp"

#include "math.hpp"
#include "geometry.cpp"

#include "obj_file_reader.cpp"

gentle::Camera<float> camera;
gentle::Mesh<float> mesh;
gentle::Matrix4x4<float> projectionMatrix;

float theta = 0.0f;
float cameraYaw = 0.0f;

bool isTeapot = true;

void GameInitialize(const GameMemory &gameMemory, const RenderBuffer &renderBuffer)
{
	ReadObjFileToVec4("teapot.obj", mesh.triangles);
	
	// Using a clockwise winding convention
	if (!isTeapot)
	{
		mesh.triangles = {
			// SOUTH
			/*{ 0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f },

			// EAST
			{ 1.0f, 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f, 1.0f },*/

			// NORTH
			{ 1.0f, 0.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f },

			// WEST
			{ 0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f },
			{ 0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 0.0f, 0.0f, 1.0f },

			// TOP
			/*{ 0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f },

			// BOTTOM
			{ 1.0f, 0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f, 1.0f }*/
		};
	}

	// Initialize the projection matrix
	projectionMatrix = gentle::MakeProjectionMatrix(90.0f, 1.0f, 0.1f, 1000.0f);

	// Initialize the camera
	camera.up = { 0.0f, 1.0f, 0.0f };
	camera.position = { 0.0f, 0.0f, 0.0f };
	camera.direction = { 0.0f, 0.0f, 1.0f };
}

void GameUpdateAndRender(const GameMemory &gameMemory, const Input &input, const RenderBuffer &renderBuffer, float dt)
{
	const uint32_t BACKGROUND_COLOR = 0x000000;

	float positionIncrement = 1.0f;
	if (!isTeapot) positionIncrement = 0.1f;
	float yawIncrement = 0.05f;
	float zOffset = 150.0f;
	if (!isTeapot) zOffset = 15.0f;

	// First process any change in yaw and update the camera direction
	if (input.buttons[KEY_D].isDown)
	{
		cameraYaw -= yawIncrement;
	}
	else if (input.buttons[KEY_A].isDown)
	{
		cameraYaw += yawIncrement;
	}

	// Apply the camera yaw to the camera.direction vector
	gentle::Vec4<float> target = { 0.0f, 0.0f, 1.0f };
	gentle::Matrix4x4<float> cameraYawMatrix = gentle::MakeYAxisRotationMatrix(cameraYaw);
	gentle::MultiplyVectorWithMatrix(target, camera.direction, cameraYawMatrix);

	// Next process any forwards or backwards movement
	gentle::Vec4<float> cameraPositionForwardBack = MultiplyVectorByScalar(camera.direction, positionIncrement);
	if (input.buttons[KEY_S].isDown)
	{
		camera.position = SubtractVectors(camera.position, cameraPositionForwardBack);
	}
	else if (input.buttons[KEY_W].isDown)
	{
		camera.position = AddVectors(camera.position, cameraPositionForwardBack);
	}

	// Strafing - use the cross product between the camera direction and up to get a normal vector to the direction being faced
	gentle::Vec4<float> cameraPositionStrafe = CrossProduct(camera.up, camera.direction);
	if (input.buttons[KEY_LEFT].isDown)
	{
		camera.position = SubtractVectors(camera.position, cameraPositionStrafe);
	}
	else if (input.buttons[KEY_RIGHT].isDown)
	{
		camera.position = AddVectors(camera.position, cameraPositionStrafe);
	}

	// Simply move the camera position vertically with up/down keypress
	if (input.buttons[KEY_DOWN].isDown)
	{
		camera.position.y -= positionIncrement;
	}
	else if (input.buttons[KEY_UP].isDown)
	{
		camera.position.y += positionIncrement;
	}

	render::ClearScreen(renderBuffer, BACKGROUND_COLOR);

	theta += dt;
	// Initialize the rotation matrices
	gentle::Matrix4x4<float> rotationMatrixX = gentle::MakeXAxisRotationMatrix(theta);
	gentle::Matrix4x4<float> rotationMatrixY = gentle::MakeYAxisRotationMatrix(theta);
	gentle::Matrix4x4<float> rotationMatrixZ = gentle::MakeZAxisRotationMatrix(theta);

	// Initialize the translation matrix
	// Push back away from the camera which is implicitly located at z: 0. This ensures we're not trying to render trinagles behind the camera
	gentle::Matrix4x4<float> translationMatrix = gentle::MakeTranslationMatrix(0.0f, 0.0f, zOffset);

	// Combine all the rotation and translation matrices into a single world transfomration matrix
	gentle::Matrix4x4<float> worldMatrix;
	// worldMatrix = MultiplyMatrixWithMatrix(rotationMatrixZ, rotationMatrixX);
	worldMatrix = gentle::MakeIdentityMatrix<float>();
	worldMatrix = gentle::MultiplyMatrixWithMatrix(worldMatrix, translationMatrix);

	render::TransformAndRenderMesh(renderBuffer, mesh, camera, worldMatrix);
}
