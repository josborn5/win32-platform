#ifndef COLLISION_H
#define COLLISION_H

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
	void CheckBlockAndXLineCollisionFromNegativeY(
		float wallYPos,
		Vec2<T> movingRectHalfSize,
		Vec2<T> prevMovingRectPosition,
		Vec2<T> movingRectVelocity,
		float *maxCollisionTime,
		CollisionSide *collisionSide,
		Vec2<T> *outMovingRectPosition
	);

	template<typename T>
	void CheckRectAndXLineCollisionFromPositiveY(
		float wallYPos,
		Vec2<T> movingRectHalfSize,
		Vec2<T> prevMovingRectPosition,
		Vec2<T> movingRectVelocity,
		float *maxCollisionTime,
		CollisionSide *collisionSide,
		Vec2<T> *outMovingRectPosition
	);

	template<typename T>
	void CheckRectAndYLineCollisionFromPositiveX(
		float wallXPos,
		Vec2<T> movingRectHalfSize,
		Vec2<T> prevMovingRectPosition,
		Vec2<T> movingRectVelocity,
		float *maxCollisionTime,
		CollisionSide *collisionSide,
		Vec2<T> *outMovingRectPosition
	);

	template<typename T>
	void CheckRectAndYLineCollisionFromNegativeX(
		float wallXPos,
		Vec2<T> movingRectHalfSize,
		Vec2<T> prevMovingRectPosition,
		Vec2<T> movingRectVelocity,
		float *maxCollisionTime,
		CollisionSide *collisionSide,
		Vec2<T> *outMovingRectPosition
	);

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
	);

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
	);
}

#endif