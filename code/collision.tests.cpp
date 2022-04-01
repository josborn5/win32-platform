#include <stdio.h>
#include <assert.h>

#include "collision.hpp"

using namespace gentle;

#define originalCollisionTime 10.0f
#define originPosition Vec2<float> { 0.0f, 0.0f }
#define oneByOneHalfSize Vec2<float> { 1.0f, 1.0f }

#define movingLeft Vec2<float> { -2.0f, 0.0f }
#define movingRight Vec2<float> { 2.0f, 0.0f }
#define movingUp Vec2<float> { 0.0f, 2.0f }
#define movingDown Vec2<float> { 0.0f, -2.0f }

#define blockRightOfOrigin Vec2<float> { 4.0f, 0.0f }
#define blockLeftOfOrigin Vec2<float> { -4.0f, 0.0f }
#define blockAboveOrigin Vec2<float> { 0.0f, 4.0f }
#define blockBelowOrigin Vec2<float> { 0.0f, -4.0f }

void RunCheckStaticAndMovingRectCollisionTests(Vec2<float> bPosition0, Vec2<float> bVelocity, float expectedCollisionTime, CollisionSide expectedCollisionResult, Vec2<float> expectedCollisionPosition)
{
	float collisionTime = originalCollisionTime;

	Rect<float> aRect;
	aRect.halfSize = oneByOneHalfSize;
	aRect.position = originPosition;
	aRect.velocity = Vec2<float> { 0.0f, 0.0f };

	Rect<float> bRect;
	bRect.halfSize = oneByOneHalfSize;
	bRect.position = bPosition0;
	bRect.velocity = bVelocity;

	CollisionResult<float> result = CheckCollisionBetweenRects(aRect, bRect, collisionTime);

	printf("actual collisionResult is %d\n", result.bRect.side);
	printf("expected collisionResult is %d\n", expectedCollisionResult);
	printf("actual collisionTime is %f\n", result.time);
	printf("expected collisionTime is %f\n", expectedCollisionTime);
	assert(result.bRect.side == expectedCollisionResult);
	assert(result.time == expectedCollisionTime);

	if (expectedCollisionResult != None)
	{
		printf("actual collisionPosition.x is %f\n", result.bRect.position.x);
		printf("expected collisionPosition.x is %f\n", expectedCollisionPosition.x);
		printf("actual collisionPosition.y is %f\n\n", result.bRect.position.y);
		printf("expected collisionPosition.y is %f\n", expectedCollisionPosition.y);
		assert(result.bRect.position.x == expectedCollisionPosition.x);
		assert(result.bRect.position.y == expectedCollisionPosition.y);
	}
}

void RunCheckCollisionBetweenMovingRectsTests(Vec2<float> aVelocity, Vec2<float> bPosition0, Vec2<float> bVelocity, float expectedCollisionTime, CollisionSide expectedCollisionResult, Vec2<float> expectedCollisionPosition)
{
	float collisionTime = originalCollisionTime;

	Rect<float> aRect;
	aRect.halfSize = oneByOneHalfSize;
	aRect.position = originPosition;
	aRect.velocity = aVelocity;

	Rect<float> bRect;
	bRect.halfSize = oneByOneHalfSize;
	bRect.position = bPosition0;
	bRect.velocity = bVelocity;

	CollisionResult<float> result = CheckCollisionBetweenRects(aRect, bRect, collisionTime);

	printf("actual collisionResult is %d\n", result.bRect.side);
	printf("expected collisionResult is %d\n", expectedCollisionResult);
	printf("actual collisionTime is %f\n", result.time);
	printf("expected collisionTime is %f\n", expectedCollisionTime);
	assert(result.bRect.side == expectedCollisionResult);
	assert(result.time == expectedCollisionTime);

	if (expectedCollisionResult != None)
	{
		printf("actual collisionPosition.x is %f\n", result.bRect.position.x);
		printf("expected collisionPosition.x is %f\n", expectedCollisionPosition.x);
		printf("actual collisionPosition.y is %f\n\n", result.bRect.position.y);
		printf("expected collisionPosition.y is %f\n", expectedCollisionPosition.y);
		assert(result.bRect.position.x == expectedCollisionPosition.x);
		assert(result.bRect.position.y == expectedCollisionPosition.y);
	}
}

void RunCheckRectAndXLineCollisionTest(Vec2<float> blockPosition0, Vec2<float> blockVelocity, float expectedCollisionTime, int expectedCollisionResult, Vec2<float> expectedCollisionPosition)
{
	float wallYPos = 0.0f;
	float collisionTime = originalCollisionTime;
	Rect<float> rect;
	rect.velocity = blockVelocity;
	rect.halfSize = oneByOneHalfSize;
	rect.position = blockPosition0;

	CollisionResult<float> result = CheckRectAndXLineCollision(
		wallYPos,
		rect,
		collisionTime
	);

	printf("actual collisionResult is %d\n", result.aRect.side);
	printf("expected collisionResult is %d\n", expectedCollisionResult);
	printf("actual collisionTime is %f\n", result.time);
	printf("expected collisionTime is %f\n", expectedCollisionTime);
	
	assert(result.aRect.side == expectedCollisionResult);
	assert(result.time == expectedCollisionTime);
	if (expectedCollisionResult != None)
	{
		printf("actual collisionPosition.x is %f\n", result.aRect.position.x);
		printf("expected collisionPosition.x is %f\n", expectedCollisionPosition.x);
		printf("actual collisionPosition.y is %f\n", result.aRect.position.y);
		printf("expected collisionPosition.y is %f\n", expectedCollisionPosition.y);
		assert(result.aRect.position.x == expectedCollisionPosition.x);
		assert(result.aRect.position.y == expectedCollisionPosition.y);
	}
	printf("\n");
}

void RunCollisionTests()
{
	/*
	* CheckBlockAndBallCollision
	*
	*/

	/* A <--B--> Right Hand Side collisions */
	// collision on x-axis
	RunCheckStaticAndMovingRectCollisionTests(blockRightOfOrigin, movingLeft, 1, Right, Vec2<float> { 2.0f, 0.0f });

	// No collision on x-axis when moving too slow to collide before collision time boundary
	RunCheckStaticAndMovingRectCollisionTests(blockRightOfOrigin, Vec2<float> { -0.20000001f, 0.0f }, 0.0f, None, Vec2<float> { 4.0f, 0.0f });

	// Collision on x-axis when moving slow to collide on collision time
	RunCheckStaticAndMovingRectCollisionTests(blockRightOfOrigin, Vec2<float> { -0.20000002f, 0.0f }, 9.99999905f, Right, Vec2<float> { 2.0f, 0.0f });

	// No collision on x axis when moving away from each other
	RunCheckStaticAndMovingRectCollisionTests(blockRightOfOrigin, movingRight, 0.0f, None, Vec2<float> { 4.0f, 0.0f });

	// Collision on x axis when moving toward each other off center at boundary
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { 4.0f, 2.0f }, movingLeft, 1, Right, Vec2<float> { 2.0f, 2.0f });

	// No Collision on x axis when moving toward each other off center at boundary
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { 4.0f, 2.000001f }, movingLeft, 0.0f, None, Vec2<float> { 4.0f, 2.000001f });

	// Collision when already touching and moving toward each other
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { 2.0f, 0.0f }, movingLeft, 0.0f, Right, Vec2<float> { 2.0f, 0.0f });

	// No collision when already touching and moving away from each other
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { 2.0f, 0.0f }, movingRight, 0.0f, None, Vec2<float> { 2.0f, 0.0f });


	/* <--B--> A Left Hand Side collisions */
	// collision on x-axis
	RunCheckStaticAndMovingRectCollisionTests(blockLeftOfOrigin, movingRight, 1, Left, Vec2<float> { -2.0f, 0.0f });

	// No collision on x-axis when moving too slow to collide before collision time boundary
	RunCheckStaticAndMovingRectCollisionTests(blockLeftOfOrigin, Vec2<float> { 0.20000001f, 0.0f }, 0.0f, None, Vec2<float> { -4.0f, 0.0f });

	// Collision on x-axis when moving slow to collide on collision time
	RunCheckStaticAndMovingRectCollisionTests(blockLeftOfOrigin, Vec2<float> { 0.20000002f, 0.0f }, 9.99999905f, Left, Vec2<float> { -2.0f, 0.0f });

	// No collision on x axis when moving away from each other
	RunCheckStaticAndMovingRectCollisionTests(blockLeftOfOrigin, movingLeft, 0.0f, None, Vec2<float> { -4.0f, 0.0f });

	// Collision on x axis when moving toward each other off center at boundary
	RunCheckStaticAndMovingRectCollisionTests(blockLeftOfOrigin, movingRight, 1, Left, Vec2<float> { -2.0f, 0.0f });

	// No Collision on x axis when moving toward each other off center at boundary
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { -4.0f, 2.000001f }, movingRight, 0.0f, None, Vec2<float> { -4.0f, 2.000001f });

	// Collision when already touching and moving toward each other
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { -2.0f, 0.0f }, movingRight, 0.0f, Left, Vec2<float> { -2.0f, 0.0f });

	// No collision when already touching and moving away from each other
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { -2.0f, 0.0f }, movingLeft, 0.0f, None, Vec2<float> { -2.0f, 0.0f });

	/* A	Bottom Side collisions
	*
	* Λ
	* |
	* B
	* |
	* V
	*/
	// collision on y-axis
	RunCheckStaticAndMovingRectCollisionTests(blockBelowOrigin, movingUp, 1, Bottom, Vec2<float> { 0.0f, -2.0f });

	// No collision on y-axis when moving too slow to collide before collision time boundary
	RunCheckStaticAndMovingRectCollisionTests(blockBelowOrigin, Vec2<float> { 0.0f, 0.20000001f }, 0.0f, None, Vec2<float> { 0.0f, -4.0f });

	// Collision on y-axis when moving slow to collide on collision time
	RunCheckStaticAndMovingRectCollisionTests(blockBelowOrigin, Vec2<float> { 0.0f, 0.20000002f }, 9.99999905f, Bottom, Vec2<float> { 0.0f, -2.0f });

	// No collision on y axis when moving away from each other
	RunCheckStaticAndMovingRectCollisionTests(blockBelowOrigin, movingDown, 0.0f, None, Vec2<float> { 0.0f, -4.0f });

	// Collision on y axis when moving toward each other off center at boundary
	RunCheckStaticAndMovingRectCollisionTests(blockBelowOrigin, movingUp, 1, Bottom, Vec2<float> { 0.0f, -2.0f });

	// No Collision on y axis when moving toward each other off center at boundary
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { 2.000001f, -4.0f }, movingUp, 0.0f, None, Vec2<float> { 2.000001f, -4.0f });

	// Collision when already touching and moving toward each other
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { 0.0f, -2.0f }, movingUp, 0.0f, Bottom, Vec2<float> { 0.0f, -2.0f });

	// No collision when already touching and moving away from each other
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { 0.0f, -2.0f }, movingDown, 0.0f, None, Vec2<float> { 0.0f, -2.0f });

	/* Λ	Top Side collisions
	* |
	* B
	* |
	* V
	*
	* A
	*/
	// collision on y-axis
	RunCheckStaticAndMovingRectCollisionTests(blockAboveOrigin, movingDown, 1, Top, Vec2<float> { 0.0f, 2.0f });

	// No collision on y-axis when moving too slow to collide before collision time boundary
	RunCheckStaticAndMovingRectCollisionTests(blockAboveOrigin, Vec2<float> { 0.0f, -0.20000001f }, 0.0f, None, Vec2<float> { 0.0f, 4.0f });

	// Collision on y-axis when moving slow to collide on collision time
	RunCheckStaticAndMovingRectCollisionTests(blockAboveOrigin, Vec2<float> { 0.0f, -0.20000002f }, 9.99999905f, Top, Vec2<float> { 0.0f, 2.0f });

	// No collision on y axis when moving away from each other
	RunCheckStaticAndMovingRectCollisionTests(blockAboveOrigin, movingUp, 0.0f, None, Vec2<float> { 0.0f, 4.0f });

	// Collision on y axis when moving toward each other off center at boundary
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { 2.0f, 4.0f }, movingDown, 1, Top, Vec2<float> { 2.0f, 2.0f });

	// No Collision on y axis when moving toward each other off center at boundary
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { 2.000001f, 4.0f }, movingDown, 0.0f, None, Vec2<float> { 2.000001f, 4.0f });

	// Collision when already touching and moving toward each other
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { 0.0f, 2.0f }, movingDown, 0.0f, Top, Vec2<float> { 0.0f, 2.0f });

	// No collision when already touching and moving away from each other
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { 0.0f, 2.0f }, movingUp, 0.0f, None, Vec2<float> { 0.0f, 2.0f });

	/*
	* CheckCollisionBetweenMovingObjects
	*
	*/

	// collision on x-axis: A--> <--B Right Hand Side collisions
	RunCheckCollisionBetweenMovingRectsTests(movingRight, blockRightOfOrigin, movingLeft, 0.5f, Right, Vec2<float> { 3.0f, 0.0f });

	// collision on x-axis: <-A <--B Right Hand Side collisions
	RunCheckCollisionBetweenMovingRectsTests(Vec2<float> { -1.0f, 0.0f }, blockRightOfOrigin, movingLeft, 2.0f, Right, Vec2<float> { 0.0f, 0.0f });

	// collision on x-axis: B--> <--A Left Hand Side collisions
	RunCheckCollisionBetweenMovingRectsTests(movingLeft, blockLeftOfOrigin, movingRight, 0.5f, Left, Vec2<float> { -3.0f, 0.0f });

	// collision on x-axis: B--> A-> Left Hand Side collisions
	RunCheckCollisionBetweenMovingRectsTests(Vec2<float> { 1.0f, 0.0f }, blockLeftOfOrigin, movingRight, 2.0f, Left, Vec2<float> { 0.0f, 0.0f });

	/* A	Bottom Side collisions
	* |
	* V
	*
	* Λ
	* |
	* B
	*/
	// collision on y-axis
	RunCheckCollisionBetweenMovingRectsTests(movingDown, blockBelowOrigin, movingUp, 0.5f, Bottom, Vec2<float> { 0.0f, -3.0f });

	/* Λ	Bottom Side collisions
	* |
	* A
	*
	* Λ
	* |
	* |
	* B
	*/
	// collision on y-axis
	RunCheckCollisionBetweenMovingRectsTests(Vec2<float> { 0.0f, 1.0f }, blockBelowOrigin, movingUp, 2.0f, Bottom, Vec2<float> { 0.0f, 0.0f });

	/* B	Top Side collisions
	* |
	* V
	*
	* Λ
	* |
	* A
	*/
	// collision on y-axis
	RunCheckCollisionBetweenMovingRectsTests(movingUp, blockAboveOrigin, movingDown, 0.5f, Top, Vec2<float> { 0.0f, 3.0f });

	/* B	Top Side collisions
	* |
	* |
	* V
	*
	* A
	* |
	* V
	*/
	// collision on y-axis
	RunCheckCollisionBetweenMovingRectsTests(Vec2<float> { 0.0f, -1.0f }, blockAboveOrigin, movingDown, 2.0f, Top, Vec2<float> { 0.0f, 0.0f });


	/*
	* CheckBlockAndTopsideOfWallCollision
	*
	*/

	/* B
	* |
	* V
	*---
	*/
	RunCheckRectAndXLineCollisionTest(Vec2<float> { 10.0f, 4.0f }, movingDown, 1.5f, Bottom, Vec2<float> { 10.0f, 1.0f });

	/* B
	 *---
	 * |
	 * V
	 */
	RunCheckRectAndXLineCollisionTest(Vec2<float> { -10.0f, 1.0f }, movingDown, 0.0f, Bottom, Vec2<float> { -10.0f, 1.0f });

	/* Λ
	 * |
	 * B
	 *---
	 */
	RunCheckRectAndXLineCollisionTest(Vec2<float> { 0.0f, 1.0f }, movingUp, 0.0f, None, Vec2<float> { 0.0f, 1.0f });

	/* B-->
	 *------
	 */
	RunCheckRectAndXLineCollisionTest(Vec2<float> { 0.0f, 1.0f }, movingRight, 0.0f, None, Vec2<float> { 0.0f, 1.0f });

	/* <--B
	 *------
	 */
	RunCheckRectAndXLineCollisionTest(Vec2<float> { 0.0f, 1.0f }, movingLeft, 0.0f, None, Vec2<float> { 0.0f, 1.0f });


	/*
	* RunCheckRectAndXLineCollisionFromNegativeYTest
	*
	*/

	/*---
	* Λ
	* |
	* B
	*/
	RunCheckRectAndXLineCollisionTest(Vec2<float> { 10.0f, -4.0f }, movingUp, 1.5f, Bottom, Vec2<float> { 10.0f, -1.0f });

	/* Λ
	* |
	*---
	* B
	*/
	RunCheckRectAndXLineCollisionTest(Vec2<float> { -10.0f, -1.0f }, movingUp, 0.0f, Bottom, Vec2<float> { -10.0f, -1.0f });

	/*---
	* B
	* |
	* V
	*/
	RunCheckRectAndXLineCollisionTest(Vec2<float> { 0.0f, -1.0f }, movingDown, 0.0f, None, Vec2<float> { 0.0f, -1.0f });

	/*------
	*B-->
	*/
	RunCheckRectAndXLineCollisionTest(Vec2<float> { 0.0f, -1.0f }, movingRight, 0.0f, None, Vec2<float> { 0.0f, -1.0f });

	/*------
	* <--B
	*/
	RunCheckRectAndXLineCollisionTest(Vec2<float> { 0.0f, -1.0f }, movingLeft, 0.0f, None, Vec2<float> { 0.0f, -1.0f });
}
