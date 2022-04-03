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

	struct Collision
	{
		CollisionSide side = None;
		Vec2<float> position;
	};

	struct CollisionResult
	{
		float time = 0.0f;
		Collision collisions[2];
	};

	// Checks for collision with a horizontal (X) line for a rect moving with a positive vertical (Y) velocity
	CollisionResult CheckRectAndXLineCollisionFromNegativeY(
		float wallYPos,
		const Vec2<float> &halfSize,
		const Vec2<float> &position,
		const Vec2<float> &velocity,
		float maxCollisionTime
	)
	{
		CollisionResult result = CollisionResult();
		if (velocity.y <= 0) return result;

		// Check for collision between ball and underside of wall
		float yCollisionCheckPos = wallYPos - halfSize.y;

		float tCollision = (yCollisionCheckPos - position.y) / velocity.y;
		if (tCollision >= 0 && tCollision < maxCollisionTime)
		{
			result.time = tCollision;
			Collision rectCollision = Collision();
			rectCollision.side = Bottom;
			float xCollisionPos = position.x + (tCollision * velocity.x);
			rectCollision.position = Vec2<float> { xCollisionPos, yCollisionCheckPos };
			result.collisions[0] = rectCollision;
		}

		return result;
	}

	// Checks for collision with a horizontal (X) line for a rect moving with a negative vertical (Y) velocity
	CollisionResult CheckRectAndXLineCollisionFromPositiveY(
		float wallYPos,
		const Vec2<float> &halfSize,
		const Vec2<float> &position,
		const Vec2<float> &velocity,
		float maxCollisionTime
	)
	{
		CollisionResult result = CollisionResult();
		if (velocity.y >= 0) return result;

		// Check for collision between ball and topside of wall
		float yCollisionCheckPos = wallYPos + halfSize.y;
		float tCollision = (yCollisionCheckPos - position.y) / velocity.y;
		if (tCollision >= 0 && tCollision < maxCollisionTime)
		{
			result.time = tCollision;
			Collision rectCollision = Collision();
			rectCollision.side = Bottom;
			float xCollisionPos = position.x + (tCollision * velocity.x);
			rectCollision.position = Vec2<float> { xCollisionPos, yCollisionCheckPos };
			result.collisions[0] = rectCollision;
		}
		return result;
	}

	CollisionResult CheckRectAndXLineCollision(
		float wallYPos,
		const Rect<float> &rect,
		float maxCollisionTime
	)
	{
		if (rect.velocity.y > 0) {
			return CheckRectAndXLineCollisionFromNegativeY(wallYPos, rect.halfSize, rect.position, rect.velocity, maxCollisionTime);
		} else if (rect.velocity.y < 0) {
			return CheckRectAndXLineCollisionFromPositiveY(wallYPos, rect.halfSize, rect.position, rect.velocity, maxCollisionTime);
		}
		return CollisionResult();
	}

	// Checks for collision with a veritcal (Y) line for a rect moving with a negative horizontal (X) velocity
	CollisionResult CheckRectAndYLineCollisionFromPositiveX(
		float wallXPos,
		const Vec2<float> &halfSize,
		const Vec2<float> &position,
		const Vec2<float> &velocity,
		float maxCollisionTime
	)
	{
		CollisionResult result = CollisionResult();
		if (velocity.x >= 0) return result;

		// Check for collision between ball and left wall
		float xCollisionCheckPos = wallXPos + halfSize.x;

		float tCollision = (xCollisionCheckPos - position.x) / velocity.x;
		if (tCollision >= 0 && tCollision < maxCollisionTime)
		{
			result.time = tCollision;
			Collision rectCollision = Collision();
			rectCollision.side = Right;
			float yCollisionPos = position.y + (tCollision * velocity.y);
			rectCollision.position = Vec2<float> { xCollisionCheckPos, yCollisionPos };
			result.collisions[0] = rectCollision;
		}
		return result;
	}

	// Checks for collision with a vertical (Y) line for a rect moving with a positive horizontal (X) velocity
	CollisionResult CheckRectAndYLineCollisionFromNegativeX(
		float wallXPos,
		Vec2<float> halfSize,
		Vec2<float> position,
		Vec2<float> velocity,
		float maxCollisionTime
	)
	{
		CollisionResult result = CollisionResult();
		if (velocity.x <= 0) return result;

		// Check for collision between ball and right wall
		float xCollisionCheckPos = wallXPos - halfSize.x;

		float tCollision = (xCollisionCheckPos - position.x) / velocity.x;
		if (tCollision >= 0 && tCollision < maxCollisionTime)
		{
			result.time = tCollision;
			Collision rectCollision = Collision();
			rectCollision.side = Left;
			float yCollisionPos = position.y + (tCollision * velocity.y);
			rectCollision.position = Vec2<float> { xCollisionCheckPos, yCollisionPos };
			result.collisions[0] = rectCollision;
		}
		return result;
	}

	CollisionResult CheckRectAndYLineCollision(
		float wallXPos,
		const Rect<float> &rect,
		float maxCollisionTime
	)
	{
		if (rect.velocity.x > 0) {
			return CheckRectAndYLineCollisionFromNegativeX(wallXPos, rect.halfSize, rect.position, rect.velocity, maxCollisionTime);
		} else if (rect.velocity.x < 0) {
			return CheckRectAndYLineCollisionFromPositiveX(wallXPos, rect.halfSize, rect.position, rect.velocity, maxCollisionTime);
		}
		return CollisionResult();
	}

	CollisionResult CheckStaticAndMovingRectCollision(
		const Vec2<float> &staticRectHalfSize,
		const Vec2<float> &staticRectPosition,
		const Vec2<float> &movingRectHalfSize,
		const Vec2<float> &movingRectPosition,
		const Vec2<float> &movingRectVelocity,
		float maxCollisionTime
	)
	{
		CollisionResult result = CollisionResult();
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
					Collision movingRect = Collision();
					movingRect.side = verCollisionResult;
					movingRect.position = Vec2<float> { ballXPosAtCollision, yCollisionCheckPos };
					result.collisions[0] = movingRect;
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
					Collision movingRect = Collision();
					movingRect.side = horCollisionResult;
					movingRect.position = Vec2<float> { xCollisionCheckPos, ballYPosAtCollision };
					result.collisions[0] = movingRect;
				}
			}
		}
		return result;
	}

	CollisionResult CheckCollisionBetweenMovingRects(
		const Vec2<float> &aHalfSize,
		const Vec2<float> &aPosition0,
		const Vec2<float> &aVelocity,
		const Vec2<float> &bHalfSize,
		const Vec2<float> &bPosition0,
		const Vec2<float> &bVelocity,
		float maxCollisionTime
	)
	{
		// Calculate relative velocity as between a & b, as if a is static. i.e. the origin of our co-ordinate system is fixed to whereever object 'a' is
		Vec2<float> aRelBVelocity = SubtractVectors(bVelocity, aVelocity);

		CollisionResult result = CheckStaticAndMovingRectCollision(aHalfSize, aPosition0, bHalfSize, bPosition0, aRelBVelocity, maxCollisionTime);

		// 'a' is the static rect and 'b' is the moving rect in the relative coordinate system, so shift the result so that
		// collisions[0] refers to the 'a' rect
		// collisions[1] refers to the 'b' rect
		result.collisions[1] = result.collisions[0];
		// Translate bPosition from the co-ordinate system whose origin is on 'a' back to the static co-ordinate system
		if (result.collisions[1].side != None)
		{
			Vec2<float> deltaAPosition = MultiplyVectorByScalar(aVelocity, result.time);
			result.collisions[1].position = AddVectors(deltaAPosition, result.collisions[0].position);
			// TODO: set the collision result for aRect in result.collisions[0]!
		}
		return result;
	}

	CollisionResult CheckCollisionBetweenRects(
		const Rect<float> &aRect,
		const Rect<float> &bRect,
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
