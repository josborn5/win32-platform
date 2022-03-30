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
	struct RectCollision
	{
		CollisionSide side = None;
		Vec2<T> position;
	};

	template<typename T>
	struct CollisionResult
	{
		float time = 0.0f;
		RectCollision<T> aRect;
		RectCollision<T> bRect;
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
	CollisionResult<T> CheckRectAndXLineCollisionFromNegativeY(
		float wallYPos,
		const Vec2<T> &halfSize,
		const Vec2<T> &position,
		const Vec2<T> &velocity,
		float maxCollisionTime
	)
	{
		CollisionResult<T> result = CollisionResult<T>();
		if (velocity.y <= 0) return result;

		// Check for collision between ball and underside of wall
		float yCollisionCheckPos = wallYPos - halfSize.y;

		float tCollision = (yCollisionCheckPos - position.y) / velocity.y;
		if (tCollision >= 0 && tCollision < maxCollisionTime)
		{
			result.time = tCollision;
			RectCollision<T> rectCollision = RectCollision<T>();
			rectCollision.side = Bottom;
			float xCollisionPos = position.x + (tCollision * velocity.x);
			rectCollision.position = Vec2<T> { xCollisionPos, yCollisionCheckPos };
			result.aRect = rectCollision;
		}

		return result;
	}

	// Checks for collision with a horizontal (X) line for a rect moving with a negative vertical (Y) velocity
	template<typename T>
	CollisionResult<T> CheckRectAndXLineCollisionFromPositiveY(
		float wallYPos,
		const Vec2<T> &halfSize,
		const Vec2<T> &position,
		const Vec2<T> &velocity,
		float maxCollisionTime
	)
	{
		CollisionResult<T> result = CollisionResult<T>();
		if (velocity.y >= 0) return result;

		// Check for collision between ball and topside of wall
		float yCollisionCheckPos = wallYPos + halfSize.y;
		float tCollision = (yCollisionCheckPos - position.y) / velocity.y;
		if (tCollision >= 0 && tCollision < maxCollisionTime)
		{
			result.time = tCollision;
			RectCollision<T> rectCollision = RectCollision<T>();
			rectCollision.side = Bottom;
			float xCollisionPos = position.x + (tCollision * velocity.x);
			rectCollision.position = Vec2<T> { xCollisionPos, yCollisionCheckPos };
			result.aRect = rectCollision;
		}
		return result;
	}

	template<typename T>
	CollisionResult<T> CheckRectAndXLineCollision(
		T wallYPos,
		const Rect<T> &rect,
		float maxCollisionTime
	)
	{
		if (rect.velocity.y > 0) {
			return CheckRectAndXLineCollisionFromNegativeY(wallYPos, rect.halfSize, rect.position, rect.velocity, maxCollisionTime);
		} else if (rect.velocity.y < 0) {
			return CheckRectAndXLineCollisionFromPositiveY(wallYPos, rect.halfSize, rect.position, rect.velocity, maxCollisionTime);
		}
		return CollisionResult<T>();
	}

	// Checks for collision with a veritcal (Y) line for a rect moving with a negative horizontal (X) velocity
	template<typename T>
	CollisionResult<T> CheckRectAndYLineCollisionFromPositiveX(
		float wallXPos,
		const Vec2<T> &halfSize,
		const Vec2<T> &position,
		const Vec2<T> &velocity,
		float maxCollisionTime
	)
	{
		CollisionResult<T> result = CollisionResult<T>;
		if (velocity.x >= 0) return result;

		// Check for collision between ball and left wall
		float xCollisionCheckPos = wallXPos + halfSize.x;

		float tCollision = (xCollisionCheckPos - position.x) / velocity.x;
		if (tCollision >= 0 && tCollision < maxCollisionTime)
		{
			result.time = tCollision;
			RectCollision<T> rectCollision = RectCollision<T>;
			rectCollision.side = Right;
			float yCollisionPos = position.y + (tCollision * velocity.y);
			rectCollision.position = Vec<T> { xCollisionCheckPos, yCollisionPos };
			result.aRect = rectCollision;
		}
		return result;
	}

	// Checks for collision with a vertical (Y) line for a rect moving with a positive horizontal (X) velocity
	template<typename T>
	CollisionResult<T> CheckRectAndYLineCollisionFromNegativeX(
		float wallXPos,
		Vec2<T> halfSize,
		Vec2<T> position,
		Vec2<T> velocity,
		float maxCollisionTime
	)
	{
		CollisionResult<T> result = CollisionResult<T>;
		if (movingRectVelocity.x <= 0) return;

		// Check for collision between ball and right wall
		float xCollisionCheckPos = wallXPos - halfSize.x;

		float tCollision = (xCollisionCheckPos - position.x) / velocity.x;
		if (tCollision >= 0 && tCollision < maxCollisionTime)
		{
			result.time = tCollision;
			RectCollision<T> rectCollision = RectCollision<T>;
			rectCollision.side = Left;
			float yCollisionPos = position.y + (tCollision * velocity.y);
			rectCollision.position = Vec<T> { xCollisionCheckPos, yCollisionPos };
			result.aRect = rectCollision;
		}
		return result;
	}

	template<typename T>
	CollisionResult<T> CheckRectAndYLineCollision(
		T wallXPos,
		const Rect<T> &rect,
		float maxCollisionTime
	)
	{
		if (rect.velocity.x > 0) {
			return CheckRectAndYLineCollisionFromNegativeX(wallXPos, rect.halfSize, rect.position, rect.velocity, maxCollisionTime);
		} else if (rect.velocity.x < 0) {
			return CheckRectAndYLineCollisionFromPositiveX(wallXPos, rect.halfSize, rect.position, rect.velocity, maxCollisionTime);
		}
		return CollisionResult<T>();
	}

	template<typename T>
	CollisionResult<T> CheckStaticAndMovingRectCollision(
		const Vec2<T> &staticRectHalfSize,
		const Vec2<T> &staticRectPosition,
		const Vec2<T> &movingRectHalfSize,
		const Vec2<T> &movingRectPosition,
		const Vec2<T> &movingRectVelocity,
		float maxCollisionTime
	)
	{
		CollisionResult<T> result = CollisionResult<T>();
		float blockTopSide = staticRectPosition.y + staticRectHalfSize.y + movingRectHalfSize.y;
		float blockBottomSide = staticRectPosition.y - staticRectHalfSize.y - movingRectHalfSize.y;
		float blockLeftSide = staticRectPosition.x - staticRectHalfSize.x - movingRectHalfSize.x;
		float blockRightSide = staticRectPosition.x + staticRectHalfSize.x + movingRectHalfSize.x;

		// Check for collision between block side and ball path
		// 1. Top/bottom side
		CollisionSide verCollisionResult = None;
		float yCollisionCheckPos = 0.0f;
		if (movingRectVelocity.y > 0)
		{
			yCollisionCheckPos = blockBottomSide;
			verCollisionResult = Bottom;
		}
		if (movingRectVelocity.y < 0)
		{
			yCollisionCheckPos = blockTopSide;
			verCollisionResult = Top;
		}

		if (movingRectVelocity.y != 0)
		{
			float tYCollision = (yCollisionCheckPos - movingRectPosition.y) / movingRectVelocity.y;
			if (tYCollision >= 0)
			{
				float ballXPosAtCollision = movingRectPosition.x + (tYCollision * movingRectVelocity.x);
				if (ballXPosAtCollision >= blockLeftSide && ballXPosAtCollision <= blockRightSide && tYCollision < maxCollisionTime)
				{
					result.time = tYCollision;
					RectCollision<T> movingRect = RectCollision<T>();
					movingRect.side = verCollisionResult;
					movingRect.position = Vec2<T> { ballXPosAtCollision, yCollisionCheckPos };
					result.bRect = movingRect;
				}
			}
		}

		// 2. Left/right side, 
		CollisionSide horCollisionResult = None;
		float xCollisionCheckPos = 0.0f;
		if (movingRectVelocity.x > 0)
		{
			xCollisionCheckPos = blockLeftSide;
			horCollisionResult = Left;
		}
		if (movingRectVelocity.x < 0)
		{
			xCollisionCheckPos = blockRightSide;
			horCollisionResult = Right;
		}

		if (movingRectVelocity.x != 0)
		{
			float tXCollision = (xCollisionCheckPos - movingRectPosition.x) / movingRectVelocity.x;
			if (tXCollision >= 0)
			{
				float ballYPosAtCollision = movingRectPosition.y + (tXCollision * movingRectVelocity.y);
				if (ballYPosAtCollision >= blockBottomSide && ballYPosAtCollision <= blockTopSide && tXCollision < maxCollisionTime)
				{
					result.time = tXCollision;
					RectCollision<T> movingRect = RectCollision<T>();
					movingRect.side = horCollisionResult;
					movingRect.position = Vec2<T> { xCollisionCheckPos, ballYPosAtCollision };
					result.bRect = movingRect;
				}
			}
		}
		return result;
	}



	template<typename T>
	CollisionResult<T> CheckCollisionBetweenMovingRects(
		const Vec2<T> &aHalfSize,
		const Vec2<T> &aPosition0,
		const Vec2<T> &aVelocity,
		const Vec2<T> &bHalfSize,
		const Vec2<T> &bPosition0,
		const Vec2<T> &bVelocity,
		float maxCollisionTime
	)
	{
		// Calculate relative velocity as between a & b, as if a is static. i.e. the origin of our co-ordinate system is fixed to whereever object 'a' is
		Vec2<T> aRelBVelocity = SubtractVectors(bVelocity, aVelocity);

		CollisionResult<T> result = CheckStaticAndMovingRectCollision(aHalfSize, aPosition0, bHalfSize, bPosition0, aRelBVelocity, maxCollisionTime);

		// Translate bPosition1 from the co-ordinate system whose origin is on 'a' back to the static co-ordinate system
		if (result.bRect.side != None)
		{
			Vec2<T> deltaAPosition = MultiplyVectorByScalar(aVelocity, result.time);
			result.bRect.position = AddVectors(deltaAPosition, result.bRect.position);
			// TODO: set the result.aRect value!
		}
		return result;
	}

	template<typename T>
	CollisionResult<T> CheckCollisionBetweenRects(
		const Rect<T> &aRect,
		const Rect<T> &bRect,
		float maxCollisionTime
	)
	{
		return CheckCollisionBetweenMovingRects(
			aRect.halfSize,
			aRect.position,
			aRect.velocity,
			bRect.halfSize,
			bRect.position,
			bRect.velocity,
			maxCollisionTime
		);
	}
}

#endif
