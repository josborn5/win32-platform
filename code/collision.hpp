#ifndef COLLISION_H
#define COLLISION_H

#include "math.hpp"
#include "geometry.hpp"

// I'm getting linker errors when using separate hpp & cpp files. I don't know why, but these links might provide some info-
// https://stackoverflow.com/a/33164552
// https://stackoverflow.com/questions/20150652
// https://stackoverflow.com/questions/3705740

namespace gentle
{
	enum CollisionSide
	{
		Top,
		Right,
		Bottom,
		Left,
		None
	};

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

	// Checks for collision with a horizontal (X) line for a rect moving with a positive vertical (Y) velocity
	template<typename T>
	void CheckRectAndXLineCollisionFromNegativeY(
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

	// Checks for collision with a horizontal (X) line for a rect moving with a negative vertical (Y) velocity
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
	void CheckRectAndXLineCollision(
		T wallYPos,
		Rect<T> &rect,
		float &maxCollisionTime,
		CollisionSide &collisionSide
	)
	{
		if (rect.velocity.y > 0) {
			CheckRectAndXLineCollisionFromNegativeY(wallYPos, rect.halfSize, rect.prevPosition, rect.velocity, &maxCollisionTime, &collisionSide, &rect.position);
		} else if (rect.velocity.y < 0) {
			CheckRectAndXLineCollisionFromPositiveY(wallYPos, rect.halfSize, rect.prevPosition, rect.velocity, &maxCollisionTime, &collisionSide, &rect.position);
		}
	}

	// Checks for collision with a veritcal (Y) line for a rect moving with a negative horizontal (X) velocity
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

	// Checks for collision with a vertical (Y) line for a rect moving with a positive horizontal (X) velocity
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
		CollisionSide horizontalCollisionResult = None;
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
		CollisionSide verticalCollisionResult = None;
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
	void CheckRectAndYLineCollision(
		T wallXPos,
		Rect<T> &rect,
		float &maxCollisionTime,
		CollisionSide &collisionSide
	)
	{
		if (rect.velocity.x > 0) {
			CheckRectAndYLineCollisionFromNegativeX(wallXPos, rect.halfSize, rect.prevPosition, rect.velocity, &maxCollisionTime, &collisionSide, &rect.position);
		} else if (rect.velocity.x < 0) {
			CheckRectAndYLineCollisionFromPositiveX(wallXPos, rect.halfSize, rect.prevPosition, rect.velocity, &maxCollisionTime, &collisionSide, &rect.position);
		}
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
		CollisionSide *collisionResult,
		Vec2<T> *bPosition1
	)
	{
		// Calculate relative velocity as between a & b, as if a is static. i.e. the origin of our co-ordinate system is fixed to whereever object 'a' is
		Vec2<T> aRelBVelocity = SubtractVectors(bVelocity, aVelocity);

		bool result = CheckStaticAndMovingRectCollision(aHalfSize, aPosition0, bHalfSize, bPosition0, aRelBVelocity, maxCollisionTime, collisionResult, bPosition1);

		// Translate bPosition1 from the co-ordinate system whose origin is on 'a' back to the static co-ordinate system
		if (result)
		{
			Vec2<T> deltaAPosition = MultiplyVectorByScalar(aVelocity, *maxCollisionTime);
			*bPosition1 = AddVectors(deltaAPosition, *bPosition1);
		}
		return result;
	}

	template<typename T>
	bool CheckCollisionBetweenRects(
		Rect<T> &aRect,
		Rect<T> &bRect,
		float &maxCollisionTime,
		CollisionSide &collisionResult
	)
	{
		// Calculate relative velocity as between a & b, as if a is static. i.e. the origin of our co-ordinate system is fixed to whereever object 'a' is
		Vec2<T> aRelBVelocity = SubtractVectors(bRect.velocity, aRect.velocity);

		bool result = CheckStaticAndMovingRectCollision(aRect.halfSize, aRect.prevPosition, bRect.halfSize, bRect.prevPosition, aRelBVelocity, &maxCollisionTime, &collisionResult, &bRect.position);

		// Translate bPosition1 from the co-ordinate system whose origin is on 'a' back to the static co-ordinate system
		if (result)
		{
			Vec2<T> deltaAPosition = MultiplyVectorByScalar(aRect.velocity, maxCollisionTime);
			bRect.position = AddVectors(deltaAPosition, bRect.position);
		}
		return result;
	}
}

#endif