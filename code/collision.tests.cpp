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
	Vec2<float> bPosition1;
	bPosition1.x = bPosition0.x;
	bPosition1.y = bPosition0.y;
	float collisionTime = originalCollisionTime;
	CollisionSide collisionResult = None;

	Rect<float> aRect;
	aRect.halfSize = oneByOneHalfSize;
	aRect.prevPosition = originPosition;
	aRect.velocity = Vec2<float> { 0.0f, 0.0f };

	Rect<float> bRect;
	bRect.halfSize = oneByOneHalfSize;
	bRect.prevPosition = bPosition0;
	bRect.velocity = bVelocity;
	bRect.position = bPosition1;

	bool result = CheckCollisionBetweenRects(aRect, bRect, collisionTime, collisionResult);

	bool expectedCollision = (expectedCollisionResult != None);
	printf("collisionResult is %d\n", collisionResult);
	printf("collisionTime is %f\n", collisionTime);
	printf("collisionPosition.x is %f\n", bRect.position.x);
	printf("collisionPosition.y is %f\n\n", bRect.position.y);
	assert(result == expectedCollision);
	assert(collisionResult == expectedCollisionResult);
	assert(collisionTime == expectedCollisionTime);
	assert(bRect.position.x == expectedCollisionPosition.x);
	assert(bRect.position.y == expectedCollisionPosition.y);
}

void RunCheckCollisionBetweenMovingRectsTests(Vec2<float> aVelocity, Vec2<float> bPosition0, Vec2<float> bVelocity, float expectedCollisionTime, CollisionSide expectedCollisionResult, Vec2<float> expectedCollisionPosition)
{
	Vec2<float> bPosition1;
	bPosition1.x = bPosition0.x;
	bPosition1.y = bPosition0.y;
	float collisionTime = originalCollisionTime;
	CollisionSide collisionResult = None;

	Rect<float> aRect;
	aRect.halfSize = oneByOneHalfSize;
	aRect.prevPosition = originPosition;
	aRect.velocity = aVelocity;

	Rect<float> bRect;
	bRect.halfSize = oneByOneHalfSize;
	bRect.prevPosition = bPosition0;
	bRect.velocity = bVelocity;
	bRect.position = bPosition1;

	bool result = CheckCollisionBetweenRects(aRect, bRect, collisionTime, collisionResult);
	bool expectedCollision = (expectedCollisionResult != None);
	printf("collisionResult is %d\n", collisionResult);
	printf("collisionTime is %f\n", collisionTime);
	printf("collisionPosition.x is %f\n", bRect.position.x);
	printf("collisionPosition.y is %f\n\n", bRect.position.y);
	assert(collisionTime == expectedCollisionTime);
	assert(collisionResult == expectedCollisionResult);
	assert(result == expectedCollision);
	assert(bRect.position.x == expectedCollisionPosition.x);
	assert(bRect.position.y == expectedCollisionPosition.y);
}

void RunCheckRectAndXLineCollisionTest(Vec2<float> blockPosition0, Vec2<float> blockVelocity, float expectedCollisionTime, int expectedCollisionResult, Vec2<float> expectedCollisionPosition)
{
	float wallYPos = 0.0f;
	float collisionTime = originalCollisionTime;
	CollisionSide collisionResult = None;
	Rect<float> rect;
	rect.velocity = blockVelocity;
	rect.halfSize = oneByOneHalfSize;
	rect.prevPosition = blockPosition0;
	rect.position = Vec2<float> { blockPosition0.x, blockPosition0.y };

	CheckRectAndXLineCollision(
		wallYPos,
		rect,
		collisionTime,
		collisionResult
	);

	printf("collisionResult is %d\n", collisionResult);
	printf("collisionTime is %f\n", collisionTime);
	printf("collisionPosition.x is %f\n", rect.position.x);
	printf("collisionPosition.y is %f\n\n", rect.position.y);
	assert(collisionResult == expectedCollisionResult);
	assert(collisionTime == expectedCollisionTime);
	assert(rect.position.x == expectedCollisionPosition.x);
	assert(rect.position.y == expectedCollisionPosition.y);
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
	RunCheckStaticAndMovingRectCollisionTests(blockRightOfOrigin, Vec2<float> { -0.20000001f, 0.0f }, originalCollisionTime, None, Vec2<float> { 4.0f, 0.0f });

	// Collision on x-axis when moving slow to collide on collision time
	RunCheckStaticAndMovingRectCollisionTests(blockRightOfOrigin, Vec2<float> { -0.20000002f, 0.0f }, 9.99999905f, Right, Vec2<float> { 2.0f, 0.0f });

	// No collision on x axis when moving away from each other
	RunCheckStaticAndMovingRectCollisionTests(blockRightOfOrigin, movingRight, originalCollisionTime, None, Vec2<float> { 4.0f, 0.0f });

	// Collision on x axis when moving toward each other off center at boundary
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { 4.0f, 2.0f }, movingLeft, 1, Right, Vec2<float> { 2.0f, 2.0f });

	// No Collision on x axis when moving toward each other off center at boundary
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { 4.0f, 2.000001f }, movingLeft, originalCollisionTime, None, Vec2<float> { 4.0f, 2.000001f });

	// Collision when already touching and moving toward each other
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { 2.0f, 0.0f }, movingLeft, 0.0f, Right, Vec2<float> { 2.0f, 0.0f });

	// No collision when already touching and moving away from each other
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { 2.0f, 0.0f }, movingRight, originalCollisionTime, None, Vec2<float> { 2.0f, 0.0f });


	/* <--B--> A Left Hand Side collisions */
	// collision on x-axis
	RunCheckStaticAndMovingRectCollisionTests(blockLeftOfOrigin, movingRight, 1, Left, Vec2<float> { -2.0f, 0.0f });

	// No collision on x-axis when moving too slow to collide before collision time boundary
	RunCheckStaticAndMovingRectCollisionTests(blockLeftOfOrigin, Vec2<float> { 0.20000001f, 0.0f }, originalCollisionTime, None, Vec2<float> { -4.0f, 0.0f });

	// Collision on x-axis when moving slow to collide on collision time
	RunCheckStaticAndMovingRectCollisionTests(blockLeftOfOrigin, Vec2<float> { 0.20000002f, 0.0f }, 9.99999905f, Left, Vec2<float> { -2.0f, 0.0f });

	// No collision on x axis when moving away from each other
	RunCheckStaticAndMovingRectCollisionTests(blockLeftOfOrigin, movingLeft, originalCollisionTime, None, Vec2<float> { -4.0f, 0.0f });

	// Collision on x axis when moving toward each other off center at boundary
	RunCheckStaticAndMovingRectCollisionTests(blockLeftOfOrigin, movingRight, 1, Left, Vec2<float> { -2.0f, 0.0f });

	// No Collision on x axis when moving toward each other off center at boundary
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { -4.0f, 2.000001f }, movingRight, originalCollisionTime, None, Vec2<float> { -4.0f, 2.000001f });

	// Collision when already touching and moving toward each other
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { -2.0f, 0.0f }, movingRight, 0.0f, Left, Vec2<float> { -2.0f, 0.0f });

	// No collision when already touching and moving away from each other
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { -2.0f, 0.0f }, movingLeft, originalCollisionTime, None, Vec2<float> { -2.0f, 0.0f });

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
	RunCheckStaticAndMovingRectCollisionTests(blockBelowOrigin, Vec2<float> { 0.0f, 0.20000001f }, originalCollisionTime, None, Vec2<float> { 0.0f, -4.0f });

	// Collision on y-axis when moving slow to collide on collision time
	RunCheckStaticAndMovingRectCollisionTests(blockBelowOrigin, Vec2<float> { 0.0f, 0.20000002f }, 9.99999905f, Bottom, Vec2<float> { 0.0f, -2.0f });

	// No collision on y axis when moving away from each other
	RunCheckStaticAndMovingRectCollisionTests(blockBelowOrigin, movingDown, originalCollisionTime, None, Vec2<float> { 0.0f, -4.0f });

	// Collision on y axis when moving toward each other off center at boundary
	RunCheckStaticAndMovingRectCollisionTests(blockBelowOrigin, movingUp, 1, Bottom, Vec2<float> { 0.0f, -2.0f });

	// No Collision on y axis when moving toward each other off center at boundary
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { 2.000001f, -4.0f }, movingUp, originalCollisionTime, None, Vec2<float> { 2.000001f, -4.0f });

	// Collision when already touching and moving toward each other
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { 0.0f, -2.0f }, movingUp, 0.0f, Bottom, Vec2<float> { 0.0f, -2.0f });

	// No collision when already touching and moving away from each other
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { 0.0f, -2.0f }, movingDown, originalCollisionTime, None, Vec2<float> { 0.0f, -2.0f });

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
	RunCheckStaticAndMovingRectCollisionTests(blockAboveOrigin, Vec2<float> { 0.0f, -0.20000001f }, originalCollisionTime, None, Vec2<float> { 0.0f, 4.0f });

	// Collision on y-axis when moving slow to collide on collision time
	RunCheckStaticAndMovingRectCollisionTests(blockAboveOrigin, Vec2<float> { 0.0f, -0.20000002f }, 9.99999905f, Top, Vec2<float> { 0.0f, 2.0f });

	// No collision on y axis when moving away from each other
	RunCheckStaticAndMovingRectCollisionTests(blockAboveOrigin, movingUp, originalCollisionTime, None, Vec2<float> { 0.0f, 4.0f });

	// Collision on y axis when moving toward each other off center at boundary
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { 2.0f, 4.0f }, movingDown, 1, Top, Vec2<float> { 2.0f, 2.0f });

	// No Collision on y axis when moving toward each other off center at boundary
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { 2.000001f, 4.0f }, movingDown, originalCollisionTime, None, Vec2<float> { 2.000001f, 4.0f });

	// Collision when already touching and moving toward each other
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { 0.0f, 2.0f }, movingDown, 0.0f, Top, Vec2<float> { 0.0f, 2.0f });

	// No collision when already touching and moving away from each other
	RunCheckStaticAndMovingRectCollisionTests(Vec2<float> { 0.0f, 2.0f }, movingUp, originalCollisionTime, None, Vec2<float> { 0.0f, 2.0f });

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
	RunCheckRectAndXLineCollisionTest(Vec2<float> { 10.0f, 4.0f }, movingDown, 1.5f, Top, Vec2<float> { 10.0f, 1.0f });

	/* B
	 *---
	 * |
	 * V
	 */
	RunCheckRectAndXLineCollisionTest(Vec2<float> { -10.0f, 1.0f }, movingDown, 0.0f, Top, Vec2<float> { -10.0f, 1.0f });

	/* Λ
	 * |
	 * B
	 *---
	 */
	RunCheckRectAndXLineCollisionTest(Vec2<float> { 0.0f, 1.0f }, movingUp, originalCollisionTime, None, Vec2<float> { 0.0f, 1.0f });

	/* B-->
	 *------
	 */
	RunCheckRectAndXLineCollisionTest(Vec2<float> { 0.0f, 1.0f }, movingRight, originalCollisionTime, None, Vec2<float> { 0.0f, 1.0f });

	/* <--B
	 *------
	 */
	RunCheckRectAndXLineCollisionTest(Vec2<float> { 0.0f, 1.0f }, movingLeft, originalCollisionTime, None, Vec2<float> { 0.0f, 1.0f });


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
	RunCheckRectAndXLineCollisionTest(Vec2<float> { 0.0f, -1.0f }, movingDown, originalCollisionTime, None, Vec2<float> { 0.0f, -1.0f });

	/*------
	*B-->
	*/
	RunCheckRectAndXLineCollisionTest(Vec2<float> { 0.0f, -1.0f }, movingRight, originalCollisionTime, None, Vec2<float> { 0.0f, -1.0f });

	/*------
	* <--B
	*/
	RunCheckRectAndXLineCollisionTest(Vec2<float> { 0.0f, -1.0f }, movingLeft, originalCollisionTime, None, Vec2<float> { 0.0f, -1.0f });
}
