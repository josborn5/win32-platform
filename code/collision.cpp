#include "math.hpp"
#include "collision.hpp"

namespace gentle
{
	template<typename T>
	bool AABBCollideCornerToCorner(Vec2<T> aTopRight, Vec2<T> aBottomLeft, Vec2<T> bTopRight, Vec2<T> bBottomLeft)
	{
		bool verticalCollision = (bBottomLeft.y < aTopRight.y && bTopRight.y > aBottomLeft.y);
		bool horizontalCollision = (bBottomLeft.x < aTopRight.x && bTopRight.x > aBottomLeft.x);
		return verticalCollision && horizontalCollision;
	}

	template<typename T>
	bool AABBCollideRectToRect(Vec2<T> aHalfSize, Vec2<T> aPos, Vec2<T> bHalfSize, Vec2<T> bPos)
	{
		Vec2<T> bBottomLeft = SubtractVectors(bPos, bHalfSize);	// Bottom left co-ords of b
		Vec2<T> bTopRight = AddVectors(bPos, bHalfSize);		// Top rght co-ords of b
		Vec2<T> aBottomLeft = SubtractVectors(aPos, aHalfSize);	// Bottom left co-ords of a
		Vec2<T> aTopRight = AddVectors(aPos, aHalfSize);		// Top right co-ords of a

		return AABBCollideCornerToCorner(aTopRight, aBottomLeft, bTopRight, bBottomLeft);
	}

	template<typename T>
	bool AABBCollideCornerToRect(Vec2<T> aHalfSize, Vec2<T> aPos, Vec2<T> bTopRight, Vec2<T> bBottomLeft)
	{
		Vec2<T> aBottomLeft = SubtractVectors(aPos, aHalfSize);	// Bottom left co-ords of a
		Vec2<T> aTopRight = AddVectors(aPos, aHalfSize);		// Top right co-ords of a

		return AABBCollideCornerToCorner(aTopRight, aBottomLeft, bTopRight, bBottomLeft);
	}

	template<typename T>
	bool AABBCollideRectToVertical(Vec2<T> aHalfSize, Vec2<T> aPos, float horizontalPos)
	{
		return ((aPos.x + aHalfSize.x) > horizontalPos && (aPos.x - aHalfSize.x) < horizontalPos);
	}

	template<typename T>
	void CheckBlockAndXLineCollisionFromNegativeY(
		float wallYPos,
		Vec2<T> movingRectHalfSize,
		Vec2<T> prevMovingRectPosition,
		Vec2<T> movingRectVelocity,
		float *maxCollisionTime,
		CollisionSide *collisionSide,
		Vec2<T> *outMovingRectPosition
	)
	{
		if (movingRectVelocity.y <= 0) return;

		// Check for collision between ball and underside of wall
		float yCollisionCheckPos = wallYPos - movingRectHalfSize.y;

		float tCollision = (yCollisionCheckPos - prevMovingRectPosition.y) / movingRectVelocity.y;
		if (tCollision >= 0 && tCollision < *maxCollisionTime)
		{
			*maxCollisionTime = tCollision;
			*collisionSide = Bottom;
			float xCollisionPos = prevMovingRectPosition.x + (tCollision * movingRectVelocity.x);
			outMovingRectPosition->x = xCollisionPos;
			outMovingRectPosition->y = yCollisionCheckPos;
		}
	}

	template<typename T>
	void CheckRectAndXLineCollisionFromPositiveY(
		float wallYPos,
		Vec2<T> movingRectHalfSize,
		Vec2<T> prevMovingRectPosition,
		Vec2<T> movingRectVelocity,
		float *maxCollisionTime,
		CollisionSide *collisionSide,
		Vec2<T> *outMovingRectPosition
	)
	{
		if (movingRectVelocity.y >= 0) return;

		// Check for collision between ball and topside of wall
		float yCollisionCheckPos = wallYPos + movingRectHalfSize.y;

		float tCollision = (yCollisionCheckPos - prevMovingRectPosition.y) / movingRectVelocity.y;
		if (tCollision >= 0 && tCollision < *maxCollisionTime)
		{
			*maxCollisionTime = tCollision;
			*collisionSide = Top;
			float xCollisionPos = prevMovingRectPosition.x + (tCollision * movingRectVelocity.x);
			outMovingRectPosition->x = xCollisionPos;
			outMovingRectPosition->y = yCollisionCheckPos;
		}
	}

	template<typename T>
	void CheckRectAndYLineCollisionFromPositiveX(
		float wallXPos,
		Vec2<T> movingRectHalfSize,
		Vec2<T> prevMovingRectPosition,
		Vec2<T> movingRectVelocity,
		float *maxCollisionTime,
		CollisionSide *collisionSide,
		Vec2<T> *outMovingRectPosition
	)
	{
		if (movingRectVelocity.x >= 0) return;

		// Check for collision between ball and left wall
		float xCollisionCheckPos = wallXPos + movingRectHalfSize.x;

		float tCollision = (xCollisionCheckPos - prevMovingRectPosition.x) / movingRectVelocity.x;
		if (tCollision >= 0 && tCollision < *maxCollisionTime)
		{
			*maxCollisionTime = tCollision;
			*collisionSide = Right;
			float yCollisionPos = prevMovingRectPosition.y + (tCollision * movingRectVelocity.y);
			outMovingRectPosition->x = xCollisionCheckPos;
			outMovingRectPosition->y = yCollisionPos;
		}
	}

	template<typename T>
	void CheckRectAndYLineCollisionFromNegativeX(
		float wallXPos,
		Vec2<T> movingRectHalfSize,
		Vec2<T> prevMovingRectPosition,
		Vec2<T> movingRectVelocity,
		float *maxCollisionTime,
		CollisionSide *collisionSide,
		Vec2<T> *outMovingRectPosition
	)
	{
		if (movingRectVelocity.x <= 0) return;

		// Check for collision between ball and right wall
		float xCollisionCheckPos = wallXPos - movingRectHalfSize.x;

		float tCollision = (xCollisionCheckPos - prevMovingRectPosition.x) / movingRectVelocity.x;
		if (tCollision >= 0 && tCollision < *maxCollisionTime)
		{
			*maxCollisionTime = tCollision;
			*collisionSide = Left;
			float yCollisionPos = prevMovingRectPosition.y + (tCollision * movingRectVelocity.y);
			outMovingRectPosition->x = xCollisionCheckPos;
			outMovingRectPosition->y = yCollisionPos;
		}
	}

	template<typename T>
	bool CheckStaticAndMovingRectCollision(
		Vec2<T> staticRectHalfSize,
		Vec2<T> staticRectPosition,
		Vec2<T> movingRectHalfSize,
		Vec2<T> prevMovingRectPosition,
		Vec2<T> movingRectVelocity,
		float *maxCollisionTime,
		CollisionSide *staticRectCollisionSide,
		Vec2<T> *outMovingRectPosition
	)
	{
		bool collided = false;
		float blockTopSide = staticRectPosition.y + staticRectHalfSize.y + movingRectHalfSize.y;
		float blockBottomSide = staticRectPosition.y - staticRectHalfSize.y - movingRectHalfSize.y;
		float blockLeftSide = staticRectPosition.x - staticRectHalfSize.x - movingRectHalfSize.x;
		float blockRightSide = staticRectPosition.x + staticRectHalfSize.x + movingRectHalfSize.x;

		// Check for collision between block side and ball path
		// 1. Top/bottom side
		int horizontalCollisionResult = None;
		float yCollisionCheckPos = 0.0f;
		if (movingRectVelocity.y > 0)
		{
			yCollisionCheckPos = blockBottomSide;
			horizontalCollisionResult = Bottom;
		}
		if (movingRectVelocity.y < 0)
		{
			yCollisionCheckPos = blockTopSide;
			horizontalCollisionResult = Top;
		}

		if (movingRectVelocity.y != 0)
		{
			float tYCollision = (yCollisionCheckPos - prevMovingRectPosition.y) / movingRectVelocity.y;
			if (tYCollision >= 0)
			{
				float ballXPosAtCollision = prevMovingRectPosition.x + (tYCollision * movingRectVelocity.x);
				if (ballXPosAtCollision >= blockLeftSide && ballXPosAtCollision <= blockRightSide && tYCollision < *maxCollisionTime)
				{
					*maxCollisionTime = tYCollision;
					*staticRectCollisionSide = horizontalCollisionResult;
					collided = true;
					outMovingRectPosition->x = ballXPosAtCollision;
					outMovingRectPosition->y = yCollisionCheckPos;
				}
			}
		}

		// 2. Left/right side, 
		int verticalCollisionResult = None;
		float xCollisionCheckPos = 0.0f;
		if (movingRectVelocity.x > 0)
		{
			xCollisionCheckPos = blockLeftSide;
			verticalCollisionResult = Left;
		}
		if (movingRectVelocity.x < 0)
		{
			xCollisionCheckPos = blockRightSide;
			verticalCollisionResult = Right;
		}

		if (movingRectVelocity.x != 0)
		{
			float tXCollision = (xCollisionCheckPos - prevMovingRectPosition.x) / movingRectVelocity.x;
			if (tXCollision >= 0)
			{
				float ballYPosAtCollision = prevMovingRectPosition.y + (tXCollision * movingRectVelocity.y);
				if (ballYPosAtCollision >= blockBottomSide && ballYPosAtCollision <= blockTopSide && tXCollision < *maxCollisionTime)
				{
					*maxCollisionTime = tXCollision;
					*staticRectCollisionSide = verticalCollisionResult;
					collided = true;
					outMovingRectPosition->x = xCollisionCheckPos;
					outMovingRectPosition->y = ballYPosAtCollision;
				}
			}
		}
		return collided;
	}

	template<typename T>
	bool CheckCollisionBetweenMovingRects(
		Vec2<T> aHalfSize,
		Vec2<T> aPosition0,
		Vec2<T> aVelocity,
		Vec2<T> bHalfSize,
		Vec2<T> bPosition0,
		Vec2<T> bVelocity,
		float *maxCollisionTime,
		int *collisionResult,
		Vec2<T> *bPosition1
	)
	{
		// Calculate relative velocity as between a & b, as if a is static. i.e. the origin of our co-ordinate system is fixed to whereever object 'a' is
		Vec2<T> aRelBVelocity = SubtractVectors(bVelocity, aVelocity);

		bool result = CheckBlockAndBallCollision(aHalfSize, aPosition0, bHalfSize, bPosition0, aRelBVelocity, maxCollisionTime, collisionResult, bPosition1);

		// Translate bPosition1 from the co-ordinate system whose origin is on 'a' back to the static co-ordinate system
		if (result)
		{
			Vec2<T> deltaAPosition = MultiplyVectorByScalar(aVelocity, *maxCollisionTime);
			*bPosition1 = AddVectors(deltaAPosition, *bPosition1);
		}
		return result;
	}
}